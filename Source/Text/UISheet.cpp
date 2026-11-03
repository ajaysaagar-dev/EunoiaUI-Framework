#include "EunoiaUI/Text/UISheet.h"
#include <cstring>

namespace EunoiaUI {

bool UISheet::Create(int width, int height) {
    Width = width;
    Height = height;
    Pixels.assign(width * height * 4, 0);
    Dirty = true;
    return true;
}

void UISheet::Destroy() {
    Pixels.clear();
    Width = 0;
    Height = 0;
    ImageHandle = 0;
    Dirty = false;
}

} // namespace EunoiaUI
