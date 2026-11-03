#include "EunoiaUI/Core/UIStyle.h"

namespace EunoiaUI {

void UIStyle::Dark() {
    m_preset = 0;
    Backdrop   = UIColor(16, 18, 22, 255);
    Surface    = UIColor(24, 27, 34, 255);
    Elevated   = UIColor(32, 36, 46, 255);
    Header     = UIColor(20, 22, 28, 255);

    Outline    = UIColor(46, 52, 66, 255);
    Highlight  = UIColor(62, 72, 92, 255);

    Text       = UIColor(240, 243, 250, 255);
    Faint      = UIColor(135, 145, 165, 255);

    Accent     = UIColor(45, 125, 245, 255);
    AccentSoft = UIColor(45, 125, 245, 60);

    Control    = UIColor(36, 41, 52, 255);
    Selected   = UIColor(45, 125, 245, 255);

    Hovered    = UIColor(48, 56, 72, 255);
    Pressed    = UIColor(28, 32, 42, 255);

    Groove     = UIColor(26, 30, 38, 255);
    Knob       = UIColor(240, 243, 250, 255);

    Shade      = UIColor(0, 0, 0, 120);
    Focus      = UIColor(45, 125, 245, 180);

    Tab        = UIColor(24, 27, 34, 255);
    TabActive  = UIColor(36, 41, 52, 255);

    Popup      = UIColor(26, 30, 38, 255);
    Danger     = UIColor(235, 55, 65, 255);

    ScrollTrack = UIColor(18, 20, 26, 180);
    ScrollThumb = UIColor(60, 68, 86, 255);
}

void UIStyle::Light() {
    m_preset = 1;
    Backdrop   = UIColor(245, 247, 250, 255);
    Surface    = UIColor(255, 255, 255, 255);
    Elevated   = UIColor(238, 242, 248, 255);
    Header     = UIColor(240, 243, 248, 255);

    Outline    = UIColor(215, 222, 232, 255);
    Highlight  = UIColor(195, 205, 220, 255);

    Text       = UIColor(25, 30, 40, 255);
    Faint      = UIColor(115, 125, 140, 255);

    Accent     = UIColor(35, 115, 235, 255);
    AccentSoft = UIColor(35, 115, 235, 50);

    Control    = UIColor(242, 245, 250, 255);
    Selected   = UIColor(35, 115, 235, 255);

    Hovered    = UIColor(230, 236, 245, 255);
    Pressed    = UIColor(215, 222, 235, 255);

    Groove     = UIColor(225, 230, 240, 255);
    Knob       = UIColor(255, 255, 255, 255);

    Shade      = UIColor(0, 0, 0, 40);
    Focus      = UIColor(35, 115, 235, 180);

    Tab        = UIColor(240, 243, 248, 255);
    TabActive  = UIColor(255, 255, 255, 255);

    Popup      = UIColor(255, 255, 255, 255);
    Danger     = UIColor(225, 45, 55, 255);

    ScrollTrack = UIColor(240, 242, 248, 180);
    ScrollThumb = UIColor(190, 200, 215, 255);
}

void UIStyle::Rescale(float factor) {
    if (factor <= 0.0f) return;
    Scale = factor;
    Rounding = 12.0f * factor;
    ControlRounding = 8.0f * factor;
    PaddingWide = 16.0f * factor;
    PaddingTall = 12.0f * factor;
    Spacing = 10.0f * factor;
    Thickness = 1.0f * factor;
    TitleHeight = 38.0f * factor;
    ControlHeight = 32.0f * factor;
    TabHeight = 34.0f * factor;
    CheckSize = 18.0f * factor;
}

} // namespace EunoiaUI
