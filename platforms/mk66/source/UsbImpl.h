#ifndef USBIMPL_H_
#define USBIMPL_H_

#include "Platform.h"
#include "config.h"
#include "UsbMassStorage.h"

class FileSystemImpl;

class UsbImpl final : public Usb
{
  public:
    void setFileSystem(FileSystemImpl* fileSystem) { this->fileSystem = fileSystem; }
    virtual void init() override;
    virtual void reset(void) override;
    virtual void update(void) override;
    virtual void setClassState(Usb::ClassType type, bool enable) override;
    virtual bool getClassState(Usb::ClassType type) override;
    virtual bool isAttached() override;

  private:
    FileSystemImpl* fileSystem = nullptr;
    UsbMassStorage massStorage;
    bool storageEnabled = false;
    bool storageOwned = false;
};

#endif /* USBIMPL_H_ */
