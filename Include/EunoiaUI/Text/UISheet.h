#pragma once
#include <memory>
#include <vector>

namespace EunoiaUI {

class UISheet {
public:
    bool Create(int width = 512, int height = 512);
    void Destroy();

    unsigned long long ImageHandle = 0;
    int Width = 0;
    int Height = 0;
    std::vector<unsigned char> Pixels; // RGBA 32-bit pixel buffer
    bool Dirty = true;
};

inline auto Sheet = std::make_unique<UISheet>();

} // namespace EunoiaUI
