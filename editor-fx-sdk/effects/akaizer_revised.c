// PTFX_NAME: Akaizer Revised
// PTFX_AUTHOR: DRAMA1
// PTFX_FLAGS: EFFECT, STEREO, VARIABLE_LENGTH
// PTFX_SCRATCH: 0
//
// Independent DRAMA1 cyclic-grain implementation based on published
// Akai-style cyclic time-stretch principles. No Akaizer/OpenKaizer source
// code is incorporated.
//
// Two overlapping read heads provide a stable 60/40 cyclic crossfade.
// Time Factor controls rendered duration; Transpose controls pitch.

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

static float s_grain_start[2];
static float s_phase[2];
static uint8_t s_active;
static float s_grain_size;
static float s_grain_hop;
static float s_grain_trigger;
static float s_crossfade_length;

static float clampf(float x, float lo, float hi)
{
    if (x < lo) return lo;
    if (x > hi) return hi;
    return x;
}

static float read_linear(const int16_t* src, uint32_t length, float pos)
{
    if (!src || length == 0) return 0.0f;
    if (pos <= 0.0f)
        return (float)src[0] * (1.0f / 32768.0f);
    if (pos >= (float)(length - 1))
        return (float)src[length - 1] * (1.0f / 32768.0f);

    uint32_t i0 = (uint32_t)pos;
    float frac = pos - (float)i0;
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

    float out = (float)s_input_length * s_time_factor;
    if (out < 1.0f) out = 1.0f;
    s_output_length = (uint32_t)(out + 0.5f);
    s_host->set_output_samples(s_output_length);
}

static void reset_grains(void)
{
    s_grain_size = clampf(s_cycle, 20.0f, 2000.0f);
    if (s_input_length > 0 && s_grain_size > (float)s_input_length)
        s_grain_size = (float)s_input_length;
    if (s_grain_size < 4.0f) s_grain_size = 4.0f;

    // Grain starts advance by 60% of the cycle, compensated for stretch
    // and transpose. The overlap is the remaining 40%.
    float divisor = s_time_factor * s_pitch_ratio;
    if (divisor < 0.0001f) divisor = 0.0001f;
    s_grain_hop = (s_grain_size * 0.60f) / divisor;
    if (s_grain_hop < 1.0f) s_grain_hop = 1.0f;

    s_grain_trigger = s_grain_size * 0.60f;
    s_crossfade_length = s_grain_size * 0.40f;
    if (s_crossfade_length < 1.0f) s_crossfade_length = 1.0f;

    s_grain_start[0] = 0.0f;
    s_grain_start[1] = 0.0f;
    s_phase[0] = 0.0f;
    s_phase[1] = -1.0f;
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
        uint8_t a = s_active;
        uint8_t b = (uint8_t)(1 - a);

        float phase_a = s_phase[a];
        float phase_b = s_phase[b];
        float mix = 0.0f;
        if (phase_b >= 0.0f)
            mix = clampf(phase_b / s_crossfade_length, 0.0f, 1.0f);

        float pos_a = s_grain_start[a] + phase_a;
        float pos_b = (phase_b >= 0.0f)
                        ? s_grain_start[b] + phase_b : pos_a;

        for (uint8_t ch = 0; ch < ch_count; ++ch) {
            if (!out[ch]) continue;
            const int16_t* src = in[ch] ? in[ch] : in[0];
            float va = read_linear(src, s_input_length, pos_a);
            float vb = read_linear(src, s_input_length, pos_b);
            float y = (phase_b >= 0.0f)
                        ? (va * (1.0f - mix) + vb * mix) : va;
            int32_t pcm = (int32_t)(y * 32767.0f);
            out[ch][i] = (int16_t)SATURATE_16BIT(pcm);
        }

        s_phase[a] += s_pitch_ratio;
        if (s_phase[b] >= 0.0f)
            s_phase[b] += s_pitch_ratio;

        // Begin the next grain when the active head reaches its 60% point.
        if (s_phase[a] >= s_grain_trigger && s_phase[b] < 0.0f) {
            s_grain_start[b] = s_grain_start[a] + s_grain_hop;
            s_phase[b] = 0.0f;
        }
        // Promote the new head after its 40% crossfade has completed.
        else if (s_phase[b] >= s_crossfade_length) {
            s_active = b;
            s_phase[a] = -1.0f;
        }
    }

    return (int32_t)samples;
}
