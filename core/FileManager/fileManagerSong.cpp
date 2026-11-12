#include "fileManagerDefs.h"
#include "mtStructs.h"

#include "core/interfacePopups.h"

#include "fileManager.h"
#include <cstring>

// uint8_t patternToLoad = 0;

//__NOINIT(EXTERNAL_RAM) uint8_t undo_Bank[1024*1024];
__NOINIT(EXTERNAL_RAM) Sequencer::strPattern songTrackCopy[2];

void setPatternBitmask(int patternIdx, int trackIdx, bool value);

AUDIO_CORE_RAM uint8_t cFileManager::getNextSongPattern()
{
    strSong* song = &mtProject.song;
    return song->playlist[song->playlistPos + 1] > 0 ? song->playlist[song->playlistPos + 1]
                                                     : song->playlist[0];
}
uint8_t cFileManager::getSongPattern(uint8_t pos)
{
    strSong* song = &mtProject.song;
    return song->playlist[pos];
}

std::optional<int> cFileManager::getPatternPosInSong(uint8_t patternToSearch)
{
    strSong* song = &mtProject.song;
    int startFromPos = song->playlistPos;

    for (int tryIdx = 0; tryIdx < SONG_MAX; tryIdx++)
    {
        int songPosition = (startFromPos + tryIdx) % SONG_MAX;
        if (song->playlist[songPosition] == patternToSearch)
        {
            return songPosition;
        }
    }

    return std::nullopt;
}

void cFileManager::setSongPos(uint8_t pos)
{
    strSong* song = &mtProject.song;
    song->playlistPos = pos;
}

bool cFileManager::switchNextPatternInSong()
{
    strSong* song = &mtProject.song;
    if (song->playlist[++(song->playlistPos)] != PLAYLIST_EMPTY_SLOT)
    {
        return 1;
    }
    else
    {
        song->playlistPos = 0;
        return 0;
    }

    return 0;
}


void cFileManager::setSongBlockLabel(uint8_t patternIdx, uint8_t trackIdx, const char label[4])
{
    if (patternIdx == 0 || patternIdx > PATTERN_INDEX_MAX || trackIdx >= kAllTracks)
    {
        return;
    }

    char path[PATCH_SIZE];
    snprintf(path, sizeof(path), "Workspace/patterns/pattern_%02d.mtp", patternIdx);
    Sequencer::strPattern* referencePattern = &songTrackCopy[0];
    if (!readPatternFile(path, referencePattern))
    {
        return;
    }

    const auto& referenceTrack = referencePattern->track[trackIdx];

    // Compare the actual musical fields, not the raw struct bytes. strStep can
    // contain padding bytes, and those bytes are not copied consistently by
    // the normal song copy routine. A raw memcmp therefore misses musically
    // identical blocks (especially older copied blocks).
    auto tracksMatch = [](const Sequencer::strPattern::strTrack& first,
                          const Sequencer::strPattern::strTrack& second) -> bool
    {
        if (first.length != second.length)
        {
            return false;
        }

        for (uint16_t stepIdx = 0; stepIdx <= Sequencer::MAXSTEP; stepIdx++)
        {
            const auto& a = first.step[stepIdx];
            const auto& b = second.step[stepIdx];

            if (a.note != b.note || a.instrument != b.instrument ||
                a.volume != b.volume || a.micromove != b.micromove)
            {
                return false;
            }

            for (uint8_t fxIdx = 0; fxIdx < kStepFxSlots; fxIdx++)
            {
                if (a.fx[fxIdx].type != b.fx[fxIdx].type ||
                    a.fx[fxIdx].value != b.fx[fxIdx].value)
                {
                    return false;
                }
            }
        }

        return true;
    };

    auto trackHasData = [](const Sequencer::strPattern::strTrack& track) -> bool
    {
        for (uint16_t stepIdx = 0; stepIdx <= track.length; stepIdx++)
        {
            const auto& step = track.step[stepIdx];
            if (step.note != Sequencer::STEP_NOTE_EMPTY ||
                step.instrument >= MIDI_INSTRUMENTS_START_IDX || step.volume >= 0 ||
                step.micromove >= 0)
            {
                return true;
            }

            for (uint8_t fxIdx = 0; fxIdx < kStepFxSlots; fxIdx++)
            {
                if (step.fx[fxIdx].type != 0)
                {
                    return true;
                }
            }
        }

        return false;
    };

    if (trackHasData(referenceTrack))
    {
        for (uint8_t candidatePattern = 1; candidatePattern <= PATTERN_INDEX_MAX; candidatePattern++)
        {
            snprintf(path, sizeof(path), "Workspace/patterns/pattern_%02d.mtp", candidatePattern);
            if (!readPatternFile(path, &songTrackCopy[1]))
            {
                continue;
            }

            for (uint8_t candidateTrack = 0; candidateTrack < kAllTracks; candidateTrack++)
            {
                const auto& candidate = songTrackCopy[1].track[candidateTrack];
                if (trackHasData(candidate) && tracksMatch(candidate, referenceTrack))
                {
                    memcpy(songBlockLabels[(candidatePattern - 1) * kAllTracks + candidateTrack],
                           label,
                           4);
                }
            }
        }
    }

    memcpy(songBlockLabels[(patternIdx - 1) * kAllTracks + trackIdx], label, 4);
    setProjectStructChanged();
}

void cFileManager::copySongTracks(char* currentProjectPath,
                                  uint8_t srcPatternIdx,
                                  uint8_t destPaternIdx,
                                  uint8_t trackStartSrc,
                                  uint8_t trackStartDest,
                                  uint8_t tracksNum)
{
    bool status = false;
    char currentPath[PATCH_SIZE];
    Sequencer::strPattern* srcPattern = &songTrackCopy[0];
    Sequencer::strPattern* destPattern = &songTrackCopy[1];

    snprintf(currentPath,
             sizeof(currentPath),
             "%s/patterns/pattern_%02d.mtp",
             currentProjectPath,
             srcPatternIdx);
    status = readPatternFile(currentPath, srcPattern);

    if (!status)
    {
        return;
    }

    newFileManager.storeSongUndoRevision(destPaternIdx);

    snprintf(currentPath,
             sizeof(currentPath),
             "%s/patterns/pattern_%02d.mtp",
             currentProjectPath,
             destPaternIdx);
    status = readPatternFile(currentPath, destPattern);
    if (status == false)
    {
        sequencer.clearPattern(destPattern);
    }

    while ((trackStartSrc + tracksNum) > kAllTracks || (trackStartDest + tracksNum) > kAllTracks)
    {
        tracksNum--;

        if (tracksNum == 0)
        {
            return;
        }
    }

    for (int trackIdx = 0; trackIdx < tracksNum; trackIdx++)
    {
        if (&destPattern->track[trackStartDest + trackIdx] ==
            &srcPattern->track[trackStartSrc + trackIdx]) // memcpy restrict protection
        {
            continue;
        }

        int srcTrackIdx = trackStartSrc + trackIdx;
        int destTrackIdx = trackStartDest + trackIdx;
        Sequencer::strPattern::strTrack* srcTrack = &srcPattern->track[srcTrackIdx];
        Sequencer::strPattern::strTrack* destTrack = &destPattern->track[destTrackIdx];

        for (int stepIdx = 0; stepIdx <= Sequencer::MAXSTEP; stepIdx++)
        {
            sequencer.copyStep(
              &srcTrack->step[stepIdx], &destTrack->step[stepIdx], !isSampleTrack(destTrackIdx));
        }

        // A pasted musical block inherits the source block's label as well.
        memcpy(songBlockLabels[(destPaternIdx - 1) * kAllTracks + destTrackIdx],
               songBlockLabels[(srcPatternIdx - 1) * kAllTracks + srcTrackIdx],
               4);
    }

    uint8_t destBitmaskIdx = destPaternIdx - 1;
    bool isDestPatternEmpty =
      !status || (mtProject.values.allPatternsBitmask[destBitmaskIdx] == 0 &&
                  mtProject.values.allPatternsBitmaskMidi[destBitmaskIdx] == 0);

    if (tracksNum == kAllTracks || isDestPatternEmpty)
    {
        destPattern->track[0].length = srcPattern->track[0].length;
    }
    else
    {
        destPattern->track[0].length =
          std::max(srcPattern->track[0].length, destPattern->track[0].length);
    }

    sequencer.updateTrackLengths(destPattern);

    if (updatePatternBitmask(destPaternIdx, destPattern))
    {
        setProjectStructChanged();
    }

    writePatternFile(currentPath, destPattern);
}

void cFileManager::deleteTracks(char* currentProjectPath,
                                uint8_t srcPatternIdx,
                                uint8_t trackStartSrc,
                                uint8_t tracksNum)
{
    bool status = false;
    char currentPath[PATCH_SIZE];
    Sequencer::strPattern* srcPattern = &songTrackCopy[0];

    snprintf(currentPath,
             sizeof(currentPath),
             "%s/patterns/pattern_%02d.mtp",
             currentProjectPath,
             srcPatternIdx);
    status = readPatternFile(currentPath, srcPattern);

    if (status)
    {
        storeSongUndoRevision(srcPatternIdx);

        for (uint8_t track = 0; track < tracksNum; track++)
        {
            sequencer.clearSingleTrack(&srcPattern->track[track + trackStartSrc]);
        }

        if (updatePatternBitmask(srcPatternIdx, srcPattern))
        {
            setProjectStructChanged();
        }

        writePatternFile(currentPath, srcPattern);
    }
}

void cFileManager::updateActualPatternBitmask()
{
    assert(mtProject.values.actualPattern > 0);

    if (updatePatternBitmask(mtProject.values.actualPattern, sequencer.getPatternToSaveToFile()))
    {
        setProjectStructChanged();
    }
}

bool cFileManager::updatePatternBitmask(uint8_t patternIdx, Sequencer::strPattern* sourcePattern)
{
    assert(patternIdx > 0);

    uint8_t bitmaskIdx = patternIdx - 1;

    uint16_t dataBefore = mtProject.values.allPatternsBitmask[bitmaskIdx] |
                          (mtProject.values.allPatternsBitmaskMidi[bitmaskIdx] << 8);

    uint8_t length = sourcePattern->track[0].length;

    for (uint8_t trackNum = 0; trackNum < kAllTracks; trackNum++)
    {
        bool isAnyActive = false;

        for (uint8_t stepIdx = 0; stepIdx <= length; stepIdx++)
        {
            Sequencer::strPattern::strTrack::strStep* step =
              &((Sequencer::strPattern*)sourcePattern)->track[trackNum].step[stepIdx];

            if (step->note != Sequencer::STEP_NOTE_EMPTY || step->hasAnyFx() ||
                step->instrument >= MIDI_INSTRUMENTS_START_IDX)
            {
                isAnyActive = true;
                break;
            }
        }

        setPatternBitmask(bitmaskIdx, trackNum, isAnyActive);
    }

    uint16_t dataAfter = mtProject.values.allPatternsBitmask[bitmaskIdx] |
                         (mtProject.values.allPatternsBitmaskMidi[bitmaskIdx] << 8);

    if (dataBefore == dataAfter)
    {
        return false;
    }

    return true;
}

inline void setPatternBitmask(int bitmaskIdx, int trackIdx, bool value)
{
    assert(bitmaskIdx <= PATTERN_INDEX_MAX);
    assert(trackIdx >= 0);

    if (trackIdx < kSampleTracks)
    {
        if (value)
        {
            mtProject.values.allPatternsBitmask[bitmaskIdx] |= (1 << trackIdx);
        }
        else
        {
            mtProject.values.allPatternsBitmask[bitmaskIdx] &= ~(1 << trackIdx);
        }
    }
    else if (trackIdx < kSampleTracks + kMidiTracks)
    {
        trackIdx = trackIdx - kSampleTracks;

        if (value)
        {
            mtProject.values.allPatternsBitmaskMidi[bitmaskIdx] |= (1 << trackIdx);
        }
        else
        {
            mtProject.values.allPatternsBitmaskMidi[bitmaskIdx] &= ~(1 << trackIdx);
        }
    }
}
