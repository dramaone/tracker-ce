#include "UsbMassStorage.h"

#ifdef USB_MASS_STORAGE

#include "usb_dev.h"
#include "usb_mem.h"
#include "fsl_sd.h"
#include "fsl_sd_disk.h"
#include <string.h>

namespace
{
constexpr uint32_t kCbwSignature = 0x43425355UL;
constexpr uint32_t kCswSignature = 0x53425355UL;
constexpr uint32_t kSectorSize = 512;

uint32_t readLe32(const uint8_t* p)
{
    return static_cast<uint32_t>(p[0]) |
           (static_cast<uint32_t>(p[1]) << 8) |
           (static_cast<uint32_t>(p[2]) << 16) |
           (static_cast<uint32_t>(p[3]) << 24);
}

uint32_t readBe32(const uint8_t* p)
{
    return (static_cast<uint32_t>(p[0]) << 24) |
           (static_cast<uint32_t>(p[1]) << 16) |
           (static_cast<uint32_t>(p[2]) << 8) |
           static_cast<uint32_t>(p[3]);
}

uint16_t readBe16(const uint8_t* p)
{
    return static_cast<uint16_t>((static_cast<uint16_t>(p[0]) << 8) | p[1]);
}

void writeBe32(uint8_t* p, uint32_t value)
{
    p[0] = static_cast<uint8_t>(value >> 24);
    p[1] = static_cast<uint8_t>(value >> 16);
    p[2] = static_cast<uint8_t>(value >> 8);
    p[3] = static_cast<uint8_t>(value);
}
}

void UsbMassStorage::init()
{
    resetTransport();
    mediaReady = false;
}

void UsbMassStorage::setMediaReady(bool ready)
{
    if (mediaReady != ready)
    {
        mediaReady = ready;
        resetTransport();
    }
}

void UsbMassStorage::resetTransport()
{
    state = State::kWaitCbw;
    cbwUsed = 0;
    commandFailed = false;
    tag = 0;
    residue = 0;
    dataLength = 0;
    dataOffset = 0;
    lba = 0;
    blocksRemaining = 0;
    sectorOffset = kSectorSize;
    responseLength = 0;
    command = 0;
    dataInFromSector = false;
    writeCommand = false;
}

void UsbMassStorage::failCommand(uint8_t key, uint8_t asc, uint8_t ascq)
{
    senseKey = key;
    senseAsc = asc;
    senseAscq = ascq;
    commandFailed = true;
    dataLength = 0;
    dataOffset = 0;
    state = State::kSendCsw;
}

void UsbMassStorage::processCbw()
{
    if (readLe32(cbw) != kCbwSignature || cbw[13] != 0 ||
        cbw[14] == 0 || cbw[14] > 16)
    {
        // BOT requires reset recovery after a malformed CBW. Do not interpret
        // subsequent OUT data as a new command until the host resets the pipe.
        resetTransport();
        return;
    }

    tag = readLe32(cbw + 4);
    residue = readLe32(cbw + 8);
    command = cbw[15];
    commandFailed = false;
    dataLength = 0;
    dataOffset = 0;
    responseLength = 0;
    blocksRemaining = 0;
    sectorOffset = kSectorSize;
    dataInFromSector = false;
    writeCommand = false;
    prepareCommand();
}

void UsbMassStorage::prepareCommand()
{
    const uint8_t* cdb = cbw + 15;
    const uint32_t hostLength = readLe32(cbw + 8);
    const bool hostReads = (cbw[12] & 0x80) != 0;

    switch (command)
    {
        case 0x00: // TEST UNIT READY
            if (!mediaReady)
                failCommand(0x02, 0x3A, 0x00);
            else
                state = State::kSendCsw;
            return;

        case 0x12: // INQUIRY
            memset(response, 0, sizeof(response));
            response[0] = 0x00;
            response[1] = 0x80; // removable medium
            response[2] = 0x04; // SPC-2
            response[3] = 0x02;
            response[4] = 31;
            memcpy(response + 8, "POLYEND ", 8);
            memcpy(response + 16, "TRACKER SD      ", 16);
            memcpy(response + 32, "0001", 4);
            responseLength = 36;
            break;

        case 0x03: // REQUEST SENSE
            memset(response, 0, sizeof(response));
            response[0] = 0x70;
            response[2] = senseKey;
            response[7] = 10;
            response[12] = senseAsc;
            response[13] = senseAscq;
            responseLength = 18;
            senseKey = 0;
            senseAsc = 0;
            senseAscq = 0;
            break;

        case 0x25: // READ CAPACITY (10)
            if (!mediaReady)
            {
                failCommand(0x02, 0x3A, 0x00);
                return;
            }
            memset(response, 0, sizeof(response));
            writeBe32(response, g_sd.blockCount ? g_sd.blockCount - 1 : 0);
            writeBe32(response + 4, kSectorSize);
            responseLength = 8;
            break;

        case 0x28: // READ (10)
        case 0x2A: // WRITE (10)
        {
            if (!mediaReady)
            {
                failCommand(0x02, 0x3A, 0x00);
                return;
            }
            lba = readBe32(cdb + 2);
            blocksRemaining = readBe16(cdb + 7);
            const uint64_t bytes = static_cast<uint64_t>(blocksRemaining) * kSectorSize;
            if (blocksRemaining == 0)
            {
                state = State::kSendCsw;
                return;
            }
            if (lba >= g_sd.blockCount ||
                static_cast<uint64_t>(lba) + blocksRemaining > g_sd.blockCount ||
                bytes > 0xFFFFFFFFULL || hostLength != bytes)
            {
                failCommand(0x05, 0x21, 0x00); // logical block address out of range
                return;
            }
            dataLength = static_cast<uint32_t>(bytes);
            dataInFromSector = command == 0x28;
            writeCommand = command == 0x2A;
            if (writeCommand)
                sectorOffset = 0;
            if (writeCommand && hostReads)
            {
                failCommand(0x05, 0x24, 0x00);
                return;
            }
            if (dataInFromSector && !hostReads)
            {
                failCommand(0x05, 0x24, 0x00);
                return;
            }
            state = dataInFromSector ? State::kDataIn : State::kDataOut;
            return;
        }

        case 0x1A: // MODE SENSE (6)
            memset(response, 0, sizeof(response));
            response[0] = 3;
            response[2] = mediaReady ? 0 : 0x80; // write-protect when unavailable
            responseLength = 4;
            break;

        case 0x1B: // START STOP UNIT
        case 0x1E: // PREVENT/ALLOW MEDIUM REMOVAL
        case 0x35: // SYNCHRONIZE CACHE (10)
            state = State::kSendCsw;
            return;

        default:
            failCommand(0x05, 0x20, 0x00); // invalid command operation code
            return;
    }

    if (!hostReads && hostLength != 0)
    {
        failCommand(0x05, 0x24, 0x00);
        return;
    }

    dataLength = responseLength < hostLength ? responseLength : hostLength;
    residue = hostLength - dataLength;
    state = dataLength ? State::kDataIn : State::kSendCsw;
}

bool UsbMassStorage::queueInPacket(const uint8_t* data, uint8_t length)
{
    usb_packet_t* packet = usb_malloc();
    if (packet == nullptr)
        return false;

    packet->len = length;
    packet->index = 0;
    packet->next = nullptr;
    if (length)
        memcpy(packet->buf, data, length);
    usb_tx(MSC_TX_ENDPOINT, packet);
    return true;
}

bool UsbMassStorage::readSector()
{
    if (SD_IsAsyncBusy(&g_sd))
        SD_WaitAsyncRead(&g_sd);

    return SD_ReadBlocks(&g_sd, sectorBuffer, lba, 1) == kStatus_Success;
}

bool UsbMassStorage::writeSector()
{
    if (SD_IsAsyncBusy(&g_sd))
        SD_WaitAsyncRead(&g_sd);

    return SD_WriteBlocks(&g_sd, sectorBuffer, lba, 1) == kStatus_Success;
}

void UsbMassStorage::queueCsw()
{
    csw[0] = static_cast<uint8_t>(kCswSignature);
    csw[1] = static_cast<uint8_t>(kCswSignature >> 8);
    csw[2] = static_cast<uint8_t>(kCswSignature >> 16);
    csw[3] = static_cast<uint8_t>(kCswSignature >> 24);
    csw[4] = static_cast<uint8_t>(tag);
    csw[5] = static_cast<uint8_t>(tag >> 8);
    csw[6] = static_cast<uint8_t>(tag >> 16);
    csw[7] = static_cast<uint8_t>(tag >> 24);
    csw[8] = static_cast<uint8_t>(residue);
    csw[9] = static_cast<uint8_t>(residue >> 8);
    csw[10] = static_cast<uint8_t>(residue >> 16);
    csw[11] = static_cast<uint8_t>(residue >> 24);
    csw[12] = commandFailed ? 1 : 0;
    if (queueInPacket(csw, sizeof(csw)))
    {
        state = State::kWaitCbw;
        cbwUsed = 0;
    }
}

void UsbMassStorage::update()
{
    if (!usb_configuration)
        return;

    if (usb_msc_reset_requested)
    {
        usb_msc_reset_requested = 0;
        resetTransport();
    }

    if (state == State::kWaitCbw)
    {
        usb_packet_t* packet = usb_rx(MSC_RX_ENDPOINT);
        if (packet == nullptr)
            return;

        if (packet->len > sizeof(cbw) - cbwUsed)
        {
            usb_free(packet);
            resetTransport();
            return;
        }

        memcpy(cbw + cbwUsed, packet->buf, packet->len);
        cbwUsed = static_cast<uint8_t>(cbwUsed + packet->len);
        usb_free(packet);

        if (cbwUsed == sizeof(cbw))
            processCbw();
        return;
    }

    if (state == State::kDataIn)
    {
        if (usb_tx_packet_count(MSC_TX_ENDPOINT) > 1)
            return;

        uint8_t packet[64];
        uint8_t count = 0;
        if (dataInFromSector)
        {
            if (sectorOffset >= kSectorSize)
            {
                if (!readSector())
                {
                    failCommand(0x03, 0x11, 0x00); // unrecovered read error
                    return;
                }
                sectorOffset = 0;
            }

            count = static_cast<uint8_t>(
              (dataLength - dataOffset) < sizeof(packet)
                ? (dataLength - dataOffset)
                : sizeof(packet));
            if (count > kSectorSize - sectorOffset)
                count = static_cast<uint8_t>(kSectorSize - sectorOffset);

            memcpy(packet, sectorBuffer + sectorOffset, count);
            sectorOffset = static_cast<uint16_t>(sectorOffset + count);
            if (sectorOffset == kSectorSize)
            {
                ++lba;
                --blocksRemaining;
            }
        }
        else
        {
            count = static_cast<uint8_t>(
              (dataLength - dataOffset) < sizeof(packet)
                ? (dataLength - dataOffset)
                : sizeof(packet));
            memcpy(packet, response + dataOffset, count);
        }

        if (!queueInPacket(packet, count))
            return;
        dataOffset += count;
        residue -= count;

        if (dataOffset >= dataLength)
            state = State::kSendCsw;
        return;
    }

    if (state == State::kDataOut)
    {
        usb_packet_t* packet = usb_rx(MSC_RX_ENDPOINT);
        if (packet == nullptr)
            return;

        uint16_t offset = 0;
        while (offset < packet->len && dataOffset < dataLength)
        {
            uint16_t available = static_cast<uint16_t>(kSectorSize - sectorOffset);
            uint16_t remaining = static_cast<uint16_t>(packet->len - offset);
            uint16_t transfer = remaining < available ? remaining : available;
            memcpy(sectorBuffer + sectorOffset, packet->buf + offset, transfer);
            sectorOffset = static_cast<uint16_t>(sectorOffset + transfer);
            offset = static_cast<uint16_t>(offset + transfer);
            dataOffset += transfer;
            residue -= transfer;

            if (sectorOffset == kSectorSize)
            {
                if (!writeSector())
                {
                    usb_free(packet);
                    failCommand(0x03, 0x0C, 0x02); // write error
                    return;
                }
                ++lba;
                --blocksRemaining;
                sectorOffset = 0;
            }
        }

        usb_free(packet);

        if (dataOffset >= dataLength)
        {
            if (sectorOffset != 0 || blocksRemaining != 0)
                failCommand(0x05, 0x24, 0x00);
            else
                state = State::kSendCsw;
        }
        return;
    }

    if (state == State::kSendCsw && usb_tx_packet_count(MSC_TX_ENDPOINT) <= 1)
        queueCsw();
}

#else

void UsbMassStorage::init() {}
void UsbMassStorage::update() {}
void UsbMassStorage::setMediaReady(bool) {}
void UsbMassStorage::resetTransport() {}
void UsbMassStorage::processCbw() {}
void UsbMassStorage::prepareCommand() {}
bool UsbMassStorage::queueInPacket(const uint8_t*, uint8_t) { return false; }
void UsbMassStorage::queueCsw() {}
void UsbMassStorage::failCommand(uint8_t, uint8_t, uint8_t) {}
bool UsbMassStorage::readSector() { return false; }
bool UsbMassStorage::writeSector() { return false; }

#endif
