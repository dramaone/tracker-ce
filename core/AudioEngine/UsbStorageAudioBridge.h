#ifndef AUDIOENGINE_USBSTORAGEAUDIOBRIDGE_H_
#define AUDIOENGINE_USBSTORAGEAUDIOBRIDGE_H_

// Narrow C++17-compatible boundary between the MK66 platform and audio engine.
bool prepareSdCardForUsbStorage();
void resumeSdCardAfterUsbStorage(bool mounted);

#endif
