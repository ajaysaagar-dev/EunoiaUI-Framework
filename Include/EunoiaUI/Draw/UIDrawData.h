#pragma once
#include <vector>
#include "EunoiaUI/Core/UIGeometry.h"

namespace EunoiaUI {

// Render flags
inline constexpr unsigned int UIFlagTextured = 1 << 0;
inline constexpr unsigned int UIFlagRounded  = 1 << 1;
inline constexpr unsigned int UIFlagBorder   = 1 << 2;
inline constexpr unsigned int UIFlagShadow   = 1 << 3;
inline constexpr unsigned int UIFlagGradient = 1 << 4;
inline constexpr unsigned int UIFlagCircle   = 1 << 5;
inline constexpr unsigned int UIFlagLine     = 1 << 6;

class UIInstance {
public:
    UIRectangle Bounds;

    float ClipLeft = 0.0f;
    float ClipTop = 0.0f;
    float ClipRight = 10000.0f;
    float ClipBottom = 10000.0f;

    UIRectangle Source; // Texture coordinates

    unsigned int ColorStart = 0xFFFFFFFF;
    unsigned int ColorFinish = 0xFFFFFFFF;
    unsigned long long ImageHandle = 0;

    float Rounding = 0.0f;
    float Thickness = 0.0f;

    unsigned int Flags = 0;
    float Softness = 0.75f;
    float Inflate = 0.0f;
    float Spare = 0.0f;
};

class UIBatch {
public:
    int Offset = 0;
    int Count = 0;

    unsigned long long Image = 0;
    unsigned int Effect = 0;

    void (*Callback)(void* stream, void* detail) = nullptr;
    void* Detail = nullptr;
};

class UIChannel {
public:
    std::vector<UIInstance> Items;
    std::vector<UIBatch> Marks;
};

class UIDrawData {
public:
    const UIInstance* Items = nullptr;
    int Count = 0;

    const UIBatch* Batches = nullptr;
    int BatchCount = 0;

    UIVector Extent{1920.0f, 1080.0f};
    float Moment = 0.0f;
};

} // namespace EunoiaUI
