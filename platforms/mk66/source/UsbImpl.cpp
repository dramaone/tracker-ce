#include "UsbImpl.h"

#include "FileSystemImpl.h"
#include "mtAudioEngine.h"
#include "usb_dev.h"

extern audioEngine engine;

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
            massStorage.setMediaReady(false);
            const bool mounted = fileSystem != nullptr && fileSystem->endUsbStorage();
            engine.resumeSdCardAfterUsb(mounted);
            storageOwned = false;
        }
        return;
    }

    if (storageEnabled && !storageOwned && fileSystem != nullptr)
    {
        if (engine.prepareSdCardForUsb())
        {
            if (fileSystem->beginUsbStorage())
            {
                storageOwned = true;
                massStorage.setMediaReady(true);
            }
            else
            {
                // A failed handover must not leave audio streaming suspended.
                engine.resumeSdCardAfterUsb(true);
            }
        }
    }

    // Once the host owns the medium, keep ownership until USB disconnect.
    // A UI toggle must not remount FatFs underneath an active host session.
    massStorage.update();
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
