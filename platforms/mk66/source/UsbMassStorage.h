#ifndef SOURCE_USBMASSSTORAGE_H_
#define SOURCE_USBMASSSTORAGE_H_

#include <stdint.h>

// USB Mass Storage Bulk-Only Transport. The media handover remains explicitly
// gated: no SCSI block command is accepted until the filesystem owner grants
// the card to USB.
class UsbMassStorage
{
  public:
    void init();
    void update();
    void setMediaReady(bool ready);
    bool isMediaReady() const { return mediaReady; }
    bool takeEjectRequest();
    bool takeLoadRequest();

  private:
    enum class State : uint8_t
    {
        kWaitCbw,
        kDataIn,
        kDataOut,
        kSendCsw
    };

    void resetTransport();
    void processCbw();
    void prepareCommand();
    bool queueInPacket(const uint8_t* data, uint8_t length);
    void queueCsw();
    void failCommand(uint8_t senseKey, uint8_t asc, uint8_t ascq);
    bool readSector();
    bool writeSector();

    State state = State::kWaitCbw;
    bool mediaReady = false;
    bool commandFailed = false;
    uint8_t cbw[31]{};
    uint8_t cbwUsed = 0;
    uint8_t csw[13]{};
    uint8_t response[36]{};
    uint32_t tag = 0;
    uint32_t residue = 0;
    uint32_t dataLength = 0;
    uint32_t dataOffset = 0;
    uint32_t lba = 0;
    uint32_t blocksRemaining = 0;
    uint16_t sectorOffset = 0;
    uint8_t responseLength = 0;
    uint8_t senseKey = 0x00;
    uint8_t senseAsc = 0x00;
    uint8_t senseAscq = 0x00;
    uint8_t command = 0;
    bool dataInFromSector = false;
    bool writeCommand = false;
    bool ejectPending = false;
    bool ejectReady = false;
    bool loadPending = false;
    bool loadReady = false;
    uint8_t sectorBuffer[512] __attribute__((aligned(32))){};
};

#endif
