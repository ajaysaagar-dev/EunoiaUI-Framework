#pragma once
#include <memory>
#include "EunoiaUI/Core/UIGeometry.h"

namespace EunoiaUI {

class UIStyle {
public:
    void Dark();
    void Light();
    void Rescale(float factor);

    UIColor Backdrop{18, 20, 24, 255};
    UIColor Surface{26, 29, 36, 255};
    UIColor Elevated{34, 38, 48, 255};
    UIColor Header{22, 24, 30, 255};

    UIColor Outline{48, 54, 68, 255};
    UIColor Highlight{65, 75, 95, 255};

    UIColor Text{240, 243, 250, 255};
    UIColor Faint{140, 150, 170, 255};

    UIColor Accent{45, 125, 245, 255};
    UIColor AccentSoft{45, 125, 245, 60};

    UIColor Control{38, 43, 54, 255};
    UIColor Selected{45, 125, 245, 255};

    UIColor Hovered{50, 58, 74, 255};
    UIColor Pressed{30, 34, 44, 255};

    UIColor Groove{30, 34, 44, 255};
    UIColor Knob{240, 243, 250, 255};

    UIColor Shade{0, 0, 0, 100};
    UIColor Focus{45, 125, 245, 180};

    UIColor Tab{26, 29, 36, 255};
    UIColor TabActive{38, 43, 54, 255};

    UIColor Popup{28, 32, 40, 255};
    UIColor Danger{235, 55, 65, 255};

    UIColor ScrollTrack{20, 22, 28, 180};
    UIColor ScrollThumb{65, 75, 95, 255};

    // Radii & Padding
    float Rounding = 12.0f;
    float ControlRounding = 8.0f;
    float PaddingWide = 16.0f;
    float PaddingTall = 12.0f;
    float Spacing = 10.0f;
    float Thickness = 1.0f;

    float Softness = 20.0f;
    float Glow = 18.0f;

    float TitleHeight = 38.0f;
    float TitleAlign = 0.0f;

    float ControlHeight = 32.0f;
    float TabHeight = 34.0f;

    float SplitterSize = 5.0f;
    float GripSize = 16.0f;
    float CheckSize = 18.0f;
    float KnobSize = 6.0f;

    float Scale = 1.0f;
    bool Shadows = true;
    bool Borders = true;

private:
    int m_preset = 0; // 0 = dark, 1 = light
};

inline auto Style = std::make_unique<UIStyle>();

} // namespace EunoiaUI
