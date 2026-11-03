#pragma once
#include "EunoiaUI/Draw/UIDrawData.h"

namespace EunoiaUI {

class UIGraphics {
public:
    virtual ~UIGraphics() = default;

    virtual void Destroy() = 0;
    virtual void Render(const UIDrawData& data, void* stream = nullptr) = 0;

    virtual unsigned long long CreateImage(const unsigned char* pixels, int width, int height) = 0;
    virtual void UpdateImage(unsigned long long image, const unsigned char* pixels) = 0;
    virtual unsigned long long AdoptImage(void* nativeResource) = 0;
    virtual void DestroyImage(unsigned long long image) = 0;
};

} // namespace EunoiaUI
