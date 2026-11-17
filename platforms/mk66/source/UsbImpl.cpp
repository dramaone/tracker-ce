#include "UsbImpl.h"

#include "FileSystemImpl.h"
#include "../../../core/interface/core/interface.h"
#include "../../../core/AudioEngine/UsbStorageAudioBridge.h"
#include "usb_dev.h"
#include "log.h"



void UsbImpl::init()
{
    massStorage.init();
}

void UsbImpl::reset()
{
    // USB descriptors are fixed at compile time on this platform. Do not
    // tear down the USB controller while the host is connected.
}

void UsbImpl::update()
{
#ifdef USB_MASS_STORAGE
    if (usb_configuration == 0)
    {
        if (storageOwned)
        {
            // A cable pull can interrupt a BOT command mid-transfer. Reset
            // that command before handing the SD card back to FatFs.
            LOG("USB MSC: disconnect detected; resetting transport.");
            massStorage.onUsbDisconnect();

            LOG("USB MSC: SD remount begin.");
            const bool mounted = fileSystem != nullptr && fileSystem->endUsbStorage();
            if (mounted)
                LOG("USB MSC: SD remount succeeded.");
            else
                LOG("USB MSC: SD remount failed.");
            resumeSdCardAfterUsbStorage(mounted);
            LOG("USB MSC: audio stream scheduler resumed.");
            storageOwned = false;
        }
        storageEjected = false;
        return;
    }

    if (storageEnabled && !storageOwned && !storageEjected && fileSystem != nullptr)
    {
        if (prepareSdCardForUsbStorage())
        {
            if (fileSystem->beginUsbStorage())
            {
                storageOwned = true;
                massStorage.setMediaReady(true);
            }
            else
            {
                // A failed handover must not leave audio streaming suspended.
                resumeSdCardAfterUsbStorage(fileSystem->isSdCardInserted());
            }
        }
    }

    massStorage.update();

    // Windows/macOS issue START STOP UNIT with LOEJ=1, START=0 for safe
    // removal. Release FatFs only after the successful command status is queued.
    if (storageOwned && massStorage.takeEjectRequest())
    {
        massStorage.setMediaReady(false);
        const bool mounted = fileSystem->endUsbStorage();
        resumeSdCardAfterUsbStorage(mounted);
        storageOwned = false;
        storageEjected = true;
        mtInterface.onUsbStorageEjected();
    }

    // A host may explicitly load the medium again after ejecting it.
    if (storageEnabled && storageEjected && !storageOwned && massStorage.takeLoadRequest())
    {
        if (prepareSdCardForUsbStorage() && fileSystem->beginUsbStorage())
        {
            storageOwned = true;
            storageEjected = false;
            massStorage.setMediaReady(true);
        }
        else
        {
            resumeSdCardAfterUsbStorage(fileSystem->isSdCardInserted());
        }
    }
#endif
}

void UsbImpl::setClassState(Usb::ClassType type, bool enable)
{
#ifdef USB_MASS_STORAGE
    if (type == Usb::ClassType::kStorage)
    {
        storageEnabled = enable;
    }
#else
    (void)type;
    (void)enable;
#endif
}

bool UsbImpl::getClassState(Usb::ClassType type)
{
    if (type == Usb::ClassType::kMidi)
    {
        return true;
    }
#ifdef USB_MASS_STORAGE
    if (type == Usb::ClassType::kStorage)
    {
        return storageEnabled;
    }
#endif
    return false;
}

bool UsbImpl::isAttached()
{
    return usb_configuration != 0;
}
