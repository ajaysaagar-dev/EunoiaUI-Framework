#pragma once
#include <memory>
#include <string>
#include <vector>
#include "EunoiaUI/Core/UIGeometry.h"

namespace EunoiaUI {

class UIWidgets {
public:
    void Destroy();
    void Sweep();

    // Text Display
    void Label(const char* message);
    void Faint(const char* message);
    void Heading(const char* message);
    void Section(const char* message);
    void Wrapped(const char* message);
    void Colored(const UIColor& tint, const char* message);

    // Buttons
    bool Button(const char* title, float width = 0.0f);
    bool Small(const char* title);
    bool ImageButton(const char* title, unsigned long long image, const UIVector& size);

    // Booleans
    bool Check(const char* title, bool& state);
    bool Toggle(const char* title, bool& state);
    bool Radio(const char* title, int& selected, int option);

    // Numbers & Sliders
    bool Slider(const char* title, float& current, float minimum, float maximum);
    bool SliderWhole(const char* title, int& current, int minimum, int maximum);
    bool Drag(const char* title, float& current, float speed, float minimum, float maximum, const char* format = "%.3f");
    bool Number(const char* title, int& value, int step = 1);
    bool Decimal(const char* title, float& value, float step = 0.0f, const char* format = "%.3f");

    // Text Input
    bool Field(const char* title, char* buffer, int capacity, const char* hint = nullptr);

    // Selection & Dropdowns
    bool Choice(const char* title, int& selected, const char* const* names, int count);
    bool Segments(const char* title, int& selected, const char* const* names, int count);
    bool Selectable(const char* title, bool selected);

    // Progress Bar (Crucial for In-Game HUDs: Health, Shield, Stamina, Loading)
    void ProgressBar(float fraction, float width = 0.0f, float height = 0.0f, const char* overlayText = nullptr, const UIColor* customFill = nullptr);

    // Visual Elements
    void Separator();
    void Tooltip(const char* message);
    void ColorBox(const char* title, const UIColor& color);
};

inline auto Widgets = std::make_unique<UIWidgets>();

} // namespace EunoiaUI
