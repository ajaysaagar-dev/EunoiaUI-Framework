#include "EunoiaUI/InGame/InGameElements.h"
#include "EunoiaUI/Draw/UICanvas.h"
#include "EunoiaUI/Core/UIStyle.h"
#include "EunoiaUI/Core/UIInput.h"
#include "EunoiaUI/Text/UIFont.h"
#include <cstdio>
#include <algorithm>

namespace EunoiaUI {

// 1. InGamePanel
InGamePanel::InGamePanel(const std::string& name) : InGameWidget(name) {
    m_size = UIVector(200.0f, 150.0f);
}

void InGamePanel::Render() {
    if (!m_visible || m_alpha <= 0.0f) return;
    if (HasShadow) {
        Canvas->Shadow(m_cachedBounds, Style->Shade.Fade(m_alpha), CornerRadius, Style->Softness);
    }
    Canvas->Rectangle(m_cachedBounds, BackgroundColor.Fade(m_alpha), CornerRadius);
    if (BorderThickness > 0.0f) {
        Canvas->Border(m_cachedBounds, BorderColor.Fade(m_alpha), CornerRadius, BorderThickness);
    }
    InGameWidget::Render();
}

// 2. InGameHealthBar
InGameHealthBar::InGameHealthBar(const std::string& name) : InGameWidget(name) {
    m_size = UIVector(240.0f, 20.0f);
    m_anchor = UIAnchor::BottomLeft;
    m_pivot = UIVector(0.0f, 1.0f);
}

void InGameHealthBar::SetValue(float current, float maxVal) {
    CurrentValue = std::clamp(current, 0.0f, maxVal);
    MaxValue = (maxVal > 0.0f) ? maxVal : 1.0f;
}

void InGameHealthBar::Update(float dt) {
    // Smooth lerp to current value
    float speed = 8.0f;
    DisplayValue += (CurrentValue - DisplayValue) * std::clamp(dt * speed, 0.0f, 1.0f);
    InGameWidget::Update(dt);
}

void InGameHealthBar::Render() {
    if (!m_visible || m_alpha <= 0.0f) return;

    // Background track
    Canvas->Shadow(m_cachedBounds, Style->Shade.Fade(m_alpha), CornerRadius, 10.0f);
    Canvas->Rectangle(m_cachedBounds, BackgroundColor.Fade(m_alpha), CornerRadius);
    Canvas->Border(m_cachedBounds, BorderColor.Fade(m_alpha), CornerRadius, 1.0f);

    // Fill
    float fraction = std::clamp(DisplayValue / MaxValue, 0.0f, 1.0f);
    float fillW = m_cachedBounds.Width * fraction;
    if (fillW > 0.0f) {
        UIRectangle fillRect(m_cachedBounds.Left, m_cachedBounds.Top, fillW, m_cachedBounds.Height);
        Canvas->Rectangle(fillRect, FillColor.Fade(m_alpha), CornerRadius);
    }

    // Text Label (e.g. "HP: 85 / 100")
    char buf[64];
    if (!Label.empty()) {
        std::snprintf(buf, sizeof(buf), "%s: %.0f / %.0f", Label.c_str(), CurrentValue, MaxValue);
    } else {
        std::snprintf(buf, sizeof(buf), "%.0f / %.0f", CurrentValue, MaxValue);
    }
    UIVector txtSize = Font->Measure(buf);
    UIVector txtPos = m_cachedBounds.Center() - UIVector(txtSize.Horizontal * 0.5f, Font->LineHeight() * 0.5f);
    Canvas->Text(Font.get(), buf, txtPos, UIColor::White().Fade(m_alpha));

    InGameWidget::Render();
}

// 3. InGameCrosshair
InGameCrosshair::InGameCrosshair(const std::string& name) : InGameWidget(name) {
    m_size = UIVector(32.0f, 32.0f);
    m_anchor = UIAnchor::Center;
    m_pivot = UIVector(0.5f, 0.5f);
}

void InGameCrosshair::Render() {
    if (!m_visible || m_alpha <= 0.0f) return;
    UIVector center = m_cachedBounds.Center();
    UIColor c = Color.Fade(m_alpha);

    if (ShowDot) {
        Canvas->Circle(center, Thickness, c);
    }

    // 4 reticle ticks: Left, Right, Top, Bottom
    Canvas->Line(UIVector(center.Horizontal - Gap - Size, center.Vertical), UIVector(center.Horizontal - Gap, center.Vertical), c, Thickness);
    Canvas->Line(UIVector(center.Horizontal + Gap, center.Vertical), UIVector(center.Horizontal + Gap + Size, center.Vertical), c, Thickness);
    Canvas->Line(UIVector(center.Horizontal, center.Vertical - Gap - Size), UIVector(center.Horizontal, center.Vertical - Gap), c, Thickness);
    Canvas->Line(UIVector(center.Horizontal, center.Vertical + Gap), UIVector(center.Horizontal, center.Vertical + Gap + Size), c, Thickness);

    InGameWidget::Render();
}

// 4. InGameButton
InGameButton::InGameButton(const std::string& text, const std::string& name) : InGameWidget(name), m_text(text) {
    m_size = UIVector(180.0f, 40.0f);
}

void InGameButton::Render() {
    if (!m_visible || m_alpha <= 0.0f) return;

    bool hovered = m_cachedBounds.Contains(Input->MousePosition);
    bool pressed = hovered && Input->MouseDown(0);

    UIColor bg = pressed ? PressedColor : (hovered ? HoverColor : NormalColor);
    Canvas->Rectangle(m_cachedBounds, bg.Fade(m_alpha), CornerRadius);
    Canvas->Border(m_cachedBounds, hovered ? Style->Highlight.Fade(m_alpha) : Style->Outline.Fade(m_alpha), CornerRadius, 1.0f);

    UIVector txtSize = Font->Measure(m_text.c_str());
    UIVector txtPos = m_cachedBounds.Center() - UIVector(txtSize.Horizontal * 0.5f, Font->LineHeight() * 0.5f);
    Canvas->Text(Font.get(), m_text.c_str(), txtPos, TextColor.Fade(m_alpha));

    InGameWidget::Render();
}

// 5. InGameLabel
InGameLabel::InGameLabel(const std::string& text, const std::string& name) : InGameWidget(name), m_text(text) {
    m_size = UIVector(100.0f, 24.0f);
}

void InGameLabel::Render() {
    if (!m_visible || m_alpha <= 0.0f) return;

    if (HasShadow) {
        Canvas->Text(Font.get(), m_text.c_str(), m_cachedBounds.Origin() + UIVector(1.0f, 1.0f), UIColor::Black().Fade(m_alpha * 0.75f));
    }
    Canvas->Text(Font.get(), m_text.c_str(), m_cachedBounds.Origin(), Color.Fade(m_alpha));

    InGameWidget::Render();
}

} // namespace EunoiaUI
