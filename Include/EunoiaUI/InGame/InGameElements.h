#pragma once
#include "EunoiaUI/InGame/InGameWidget.h"
#include <string>

namespace EunoiaUI {

// 1. HUD Panel Container
class InGamePanel : public InGameWidget {
public:
    InGamePanel(const std::string& name = "HUDPanel");
    void Render() override;

    UIColor BackgroundColor{20, 24, 30, 200};
    UIColor BorderColor{50, 60, 75, 255};
    float CornerRadius = 8.0f;
    float BorderThickness = 1.0f;
    bool HasShadow = true;
};

// 2. Health / Stamina Bar
class InGameHealthBar : public InGameWidget {
public:
    InGameHealthBar(const std::string& name = "HealthBar");

    void SetValue(float current, float maxVal);
    void SetFillColor(const UIColor& color) { FillColor = color; }
    void SetLabel(const std::string& label) { Label = label; }

    void Update(float dt) override;
    void Render() override;

    float CurrentValue = 100.0f;
    float MaxValue = 100.0f;
    float DisplayValue = 100.0f; // Smoothed value

    UIColor FillColor{225, 45, 55, 255}; // Crimson Red
    UIColor BackgroundColor{25, 28, 35, 220};
    UIColor BorderColor{60, 70, 85, 255};
    float CornerRadius = 6.0f;
    std::string Label = "";
};

// 3. In-Game Crosshair
class InGameCrosshair : public InGameWidget {
public:
    InGameCrosshair(const std::string& name = "Crosshair");

    void Render() override;

    UIColor Color{255, 255, 255, 220};
    float Size = 12.0f;
    float Gap = 4.0f;
    float Thickness = 2.0f;
    bool ShowDot = true;
};

// 4. In-Game Interactive Button
class InGameButton : public InGameWidget {
public:
    InGameButton(const std::string& text = "Button", const std::string& name = "InGameButton");

    void SetText(const std::string& text) { m_text = text; }
    void Render() override;

    UIColor NormalColor{35, 40, 52, 230};
    UIColor HoverColor{50, 60, 80, 255};
    UIColor PressedColor{25, 30, 40, 255};
    UIColor TextColor{240, 243, 250, 255};
    float CornerRadius = 6.0f;

private:
    std::string m_text;
};

// 5. In-Game Label
class InGameLabel : public InGameWidget {
public:
    InGameLabel(const std::string& text = "Label", const std::string& name = "InGameLabel");

    void SetText(const std::string& text) { m_text = text; }
    void Render() override;

    UIColor Color{255, 255, 255, 255};
    bool HasShadow = true;

private:
    std::string m_text;
};

} // namespace EunoiaUI
