// PTFX_NAME: Akaizer Revised
// PTFX_AUTHOR: DRAMA1
// PTFX_FLAGS: EFFECT, STEREO, VARIABLE_LENGTH
// PTFX_SCRATCH: 0
//
// DRAMA1 independent cyclic-grain implementation inspired by the published
// Akai-style time-stretch technique. This is not copied from Akaizer/OpenKaizer.
//
// REVISED-style timing: fixed output length derived from Time Factor, with
// two overlapping, linearly crossfaded read heads. Pitch changes the read
// speed inside each grain while grain placement compensates for it.

#include "tracker_plugin.h"

const PluginParam PLUGIN_PARAMS[3] = {
    PTFX_PARAM_INT("Time", 25.0f, 2000.0f, 100.0f, 1.0f, "%"),
    PTFX_PARAM_INT("Cycle", 20.0f, 2000.0f, 1000.0f, 1.0f, "smpl"),
    PTFX_PARAM_INT("Trans", -36.0f, 36.0f, 0.0f, 1.0f, "st")
};

static const PtfxHostApi* s_host = 0;
static float s_time_factor = 1.0f;
static float s_pitch_ratio = 1.0f;
static float s_cycle = 1000.0f;
static uint32_t s_input_length = 0;
static uint32_t s_output_length = 0;

// Two cyclic read heads. Positions are absolute sample positions within the
// selected source region; the input base pointer remains stable for all blocks.
static double s_grain_start[2];
static double s_phase[2];
static uint8_t s_active;
static double s_grain_size;
static double s_grain_hop;
static double s_crossfade_end;

static float clampf(float x, float lo, float hi)
{
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

static float read_linear(const int16_t* src, uint32_t length, double pos)
{
    if (!src || length == 0) return 0.0f;
    if (pos <= 0.0) return (float)src[0] * (1.0f / 32768.0f);
    if (pos >= (double)(length - 1))
        return (float)src[length - 1] * (1.0f / 32768.0f);

    uint32_t i0 = (uint32_t)pos;
    float frac = (float)(pos - (double)i0);
    float a = (float)src[i0] * (1.0f / 32768.0f);
    float b = (float)src[i0 + 1] * (1.0f / 32768.0f);
    return a + (b - a) * frac;
}

static void update_output_length(void)
{
    if (!s_host || !s_host->get_input_samples || !s_host->set_output_samples)
        return;

    s_input_length = s_host->get_input_samples();
    if (s_input_length == 0) {
        s_output_length = 0;
        s_host->set_output_samples(0);
        return;
    }

    double out = (double)s_input_length * (double)s_time_factor;
    if (out < 1.0) out = 1.0;
    if (out > 2147483000.0) out = 2147483000.0;
    s_output_length = (uint32_t)(out + 0.5);
    s_host->set_output_samples(s_output_length);
}

static void reset_grains(void)
{
    s_grain_size = clampf(s_cycle, 20.0f, 2000.0f);
    if (s_input_length > 4 && s_grain_size > (double)s_input_length)
        s_grain_size = (double)s_input_length;
    if (s_grain_size < 4.0) s_grain_size = 4.0;

    // 60% hop / 40% overlap. Compensate for both stretch and pitch so
    // changing transpose does not change the requested rendered duration.
    s_grain_hop = (s_grain_size * 0.60) /
                  ((double)s_time_factor * (double)s_pitch_ratio);
    if (s_grain_hop < 1.0) s_grain_hop = 1.0;
    s_crossfade_end = s_grain_size * 0.60;

    s_grain_start[0] = 0.0;
    s_grain_start[1] = s_grain_hop;
    s_phase[0] = 0.0;
    s_phase[1] = -1.0;
    s_active = 0;
}

void plugin_init(void* scratch_mem, uint32_t scratch_size, const PtfxHostApi* host)
{
    (void)scratch_mem;
    (void)scratch_size;
    s_host = host;
    s_time_factor = 1.0f;
    s_pitch_ratio = 1.0f;
    s_cycle = 1000.0f;
    s_input_length = 0;
    s_output_length = 0;
    reset_grains();
}

void plugin_set_param(uint8_t index, float value)
{
    if (index == 0) {
        s_time_factor = clampf(value, 25.0f, 2000.0f) * 0.01f;
    } else if (index == 1) {
        s_cycle = clampf(value, 20.0f, 2000.0f);
    } else if (index == 2) {
        float semitones = clampf(value, -36.0f, 36.0f);
        float exponent = semitones / 12.0f;
        s_pitch_ratio = (s_host && s_host->powf)
                          ? s_host->powf(2.0f, exponent)
                          : 1.0f;
    } else {
        return;
    }

    update_output_length();
    reset_grains();
}

int32_t plugin_process_block(const int16_t** in, int16_t** out,
                             uint32_t samples, uint8_t channels)
{
    if (!in || !out || !in[0] || !out[0] ||
        s_input_length == 0 || s_output_length == 0)
        return 0;

    uint8_t ch_count = channels > 2 ? 2 : channels;
    for (uint32_t i = 0; i < samples; ++i) {
        // The active grain advances at the requested pitch. Its next partner
        // begins at a compensated source offset to preserve the time factor.
        uint8_t a = s_active;
        uint8_t b = (uint8_t)(1 - a);

        if (s_phase[a] < 0.0) {
            s_phase[a] = 0.0;
            s_grain_start[a] = s_grain_start[b] + s_grain_hop;
        }

        double phase_a = s_phase[a];
        double phase_b = s_phase[b];
        float mix = 0.0f;
        if (phase_b >= 0.0 && phase_a < s_crossfade_end) {
            mix = (float)(phase_a / s_crossfade_end);
            mix = clampf(mix, 0.0f, 1.0f);
        }

        double pos_a = s_grain_start[a] + phase_a;
        double pos_b = (phase_b >= 0.0)
                         ? s_grain_start[b] + phase_b : pos_a;

        for (uint8_t ch = 0; ch < ch_count; ++ch) {
            if (!out[ch]) continue;
            const int16_t* src = in[ch] ? in[ch] : in[0];
            float va = read_linear(src, s_input_length, pos_a);
            float vb = read_linear(src, s_input_length, pos_b);
            float y = (phase_b >= 0.0)
                        ? (va * (1.0f - mix) + vb * mix) : va;
            int32_t pcm = (int32_t)(y * 32767.0f);
            out[ch][i] = (int16_t)SATURATE_16BIT(pcm);
        }

        s_phase[a] += (double)s_pitch_ratio;
        if (s_phase[b] >= 0.0)
            s_phase[b] += (double)s_pitch_ratio;

        // When the active grain reaches its overlap point, switch heads and
        // initialise the next grain at the compensated cyclic source offset.
        if (s_phase[a] >= s_crossfade_end) {
            s_active = b;
            uint8_t next = a;
            s_grain_start[next] = s_grain_start[b] + s_grain_hop;
            s_phase[next] = 0.0;
            s_phase[b] = 0.0;
        }
    }

    return (int32_t)samples;
}
