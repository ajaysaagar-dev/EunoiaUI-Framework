#include "EunoiaUI/Widgets/UIWidgets.h"
#include "EunoiaUI/Core/UIContext.h"
#include "EunoiaUI/Core/UIInput.h"
#include "EunoiaUI/Core/UIStyle.h"
#include "EunoiaUI/Draw/UICanvas.h"
#include "EunoiaUI/Layout/UILayout.h"
#include "EunoiaUI/Text/UIFont.h"
#include <cstdio>
#include <cstring>
#include <algorithm>

namespace EunoiaUI {

void UIWidgets::Destroy() {}
void UIWidgets::Sweep() {}

void UIWidgets::Label(const char* message) {
    if (!message) return;
    UIVector size = Font->Measure(message);
    UIRectangle rect = Layout->Place(UIVector(size.Horizontal, Style->ControlHeight));
    Canvas->Text(Font.get(), message, rect.Origin() + UIVector(0.0f, (rect.Height - Font->LineHeight()) * 0.5f), Style->Text);
}

void UIWidgets::Faint(const char* message) {
    if (!message) return;
    UIVector size = Font->Measure(message);
    UIRectangle rect = Layout->Place(UIVector(size.Horizontal, Style->ControlHeight));
    Canvas->Text(Font.get(), message, rect.Origin() + UIVector(0.0f, (rect.Height - Font->LineHeight()) * 0.5f), Style->Faint);
}

void UIWidgets::Heading(const char* message) {
    if (!message) return;
    UIVector size = Heading->Measure(message);
    UIRectangle rect = Layout->Place(UIVector(size.Horizontal, Style->TitleHeight));
    Canvas->Text(Heading.get(), message, rect.Origin() + UIVector(0.0f, (rect.Height - Heading->LineHeight()) * 0.5f), Style->Text);
}

void UIWidgets::Section(const char* message) {
    Separator();
    Heading(message);
}

void UIWidgets::Wrapped(const char* message) {
    Label(message);
}

void UIWidgets::Colored(const UIColor& tint, const char* message) {
    if (!message) return;
    UIVector size = Font->Measure(message);
    UIRectangle rect = Layout->Place(UIVector(size.Horizontal, Style->ControlHeight));
    Canvas->Text(Font.get(), message, rect.Origin() + UIVector(0.0f, (rect.Height - Font->LineHeight()) * 0.5f), tint);
}

bool UIWidgets::Button(const char* title, float width) {
    unsigned int id = Context->Hash(title);
    const char* caption = Context->Caption(title);

    UIVector textSize = Font->Measure(caption);
    float w = (width > 0.0f) ? width : (textSize.Horizontal + Style->PaddingWide * 2.0f);
    UIRectangle rect = (width > 0.0f) ? Layout->Place(UIVector(w, Style->ControlHeight)) : Layout->Place(UIVector(w, Style->ControlHeight));

    bool hovered = rect.Contains(Input->MousePosition) && Canvas->Visible(Input->MousePosition);
    bool pressed = hovered && Input->MouseDown(0);
    bool clicked = hovered && Input->MouseReleased(0);

    if (hovered) Context->HoveredItem = id;
    if (pressed) Context->ActiveItem = id;

    UIColor bg = pressed ? Style->Pressed : (hovered ? Style->Hovered : Style->Control);
    UIColor border = hovered ? Style->Highlight : Style->Outline;

    Canvas->Rectangle(rect, bg, Style->ControlRounding);
    if (Style->Borders) Canvas->Border(rect, border, Style->ControlRounding, Style->Thickness);

    UIVector textPos = rect.Center() - UIVector(textSize.Horizontal * 0.5f, Font->LineHeight() * 0.5f);
    Canvas->Text(Font.get(), caption, textPos, Style->Text);

    return clicked;
}

bool UIWidgets::Small(const char* title) {
    unsigned int id = Context->Hash(title);
    const char* caption = Context->Caption(title);

    UIVector textSize = Font->Measure(caption);
    float h = Style->ControlHeight * 0.75f;
    UIRectangle rect = Layout->Place(UIVector(textSize.Horizontal + Style->PaddingWide, h));

    bool hovered = rect.Contains(Input->MousePosition) && Canvas->Visible(Input->MousePosition);
    bool pressed = hovered && Input->MouseDown(0);
    bool clicked = hovered && Input->MouseReleased(0);

    UIColor bg = pressed ? Style->Pressed : (hovered ? Style->Hovered : Style->Control);
    Canvas->Rectangle(rect, bg, Style->ControlRounding * 0.75f);
    if (Style->Borders) Canvas->Border(rect, hovered ? Style->Highlight : Style->Outline, Style->ControlRounding * 0.75f, Style->Thickness);

    UIVector textPos = rect.Center() - UIVector(textSize.Horizontal * 0.5f, Font->LineHeight() * 0.5f);
    Canvas->Text(Font.get(), caption, textPos, Style->Text);

    return clicked;
}

bool UIWidgets::ImageButton(const char* title, unsigned long long image, const UIVector& size) {
    unsigned int id = Context->Hash(title);
    UIRectangle rect = Layout->Place(size);

    bool hovered = rect.Contains(Input->MousePosition) && Canvas->Visible(Input->MousePosition);
    bool pressed = hovered && Input->MouseDown(0);
    bool clicked = hovered && Input->MouseReleased(0);

    UIColor bg = pressed ? Style->Pressed : (hovered ? Style->Hovered : Style->Control);
    Canvas->Rectangle(rect, bg, Style->ControlRounding);
    Canvas->Picture(rect.Shrink(Style->PaddingTall * 0.5f), image, UIColor::White(), Style->ControlRounding * 0.5f);
    if (Style->Borders) Canvas->Border(rect, hovered ? Style->Highlight : Style->Outline, Style->ControlRounding, Style->Thickness);

    return clicked;
}

bool UIWidgets::Check(const char* title, bool& state) {
    unsigned int id = Context->Hash(title);
    const char* caption = Context->Caption(title);

    UIVector textSize = Font->Measure(caption);
    float totalW = Style->CheckSize + Style->Spacing + textSize.Horizontal;
    UIRectangle rect = Layout->Place(UIVector(totalW, Style->ControlHeight));

    bool hovered = rect.Contains(Input->MousePosition) && Canvas->Visible(Input->MousePosition);
    bool clicked = hovered && Input->MouseReleased(0);

    if (clicked) {
        state = !state;
    }

    UIRectangle boxRect(rect.Left, rect.Top + (rect.Height - Style->CheckSize) * 0.5f, Style->CheckSize, Style->CheckSize);
    UIColor boxBg = state ? Style->Accent : (hovered ? Style->Hovered : Style->Control);
    Canvas->Rectangle(boxRect, boxBg, Style->ControlRounding * 0.5f);
    Canvas->Border(boxRect, hovered ? Style->Highlight : Style->Outline, Style->ControlRounding * 0.5f, Style->Thickness);

    if (state) {
        UIRectangle inner = boxRect.Shrink(4.0f);
        Canvas->Rectangle(inner, UIColor::White(), Style->ControlRounding * 0.25f);
    }

    UIVector textPos(boxRect.Right() + Style->Spacing, rect.Top + (rect.Height - Font->LineHeight()) * 0.5f);
    Canvas->Text(Font.get(), caption, textPos, Style->Text);

    return clicked;
}

bool UIWidgets::Toggle(const char* title, bool& state) {
    unsigned int id = Context->Hash(title);
    const char* caption = Context->Caption(title);

    float toggleW = Style->CheckSize * 2.0f;
    float toggleH = Style->CheckSize;
    UIVector textSize = Font->Measure(caption);
    UIRectangle rect = Layout->Place(UIVector(toggleW + Style->Spacing + textSize.Horizontal, Style->ControlHeight));

    bool hovered = rect.Contains(Input->MousePosition) && Canvas->Visible(Input->MousePosition);
    bool clicked = hovered && Input->MouseReleased(0);

    if (clicked) {
        state = !state;
    }

    UIRectangle track(rect.Left, rect.Top + (rect.Height - toggleH) * 0.5f, toggleW, toggleH);
    Canvas->Rectangle(track, state ? Style->Accent : Style->Groove, toggleH * 0.5f);

    float knobX = state ? (track.Right() - toggleH + 2.0f) : (track.Left + 2.0f);
    UIRectangle knob(knobX, track.Top + 2.0f, toggleH - 4.0f, toggleH - 4.0f);
    Canvas->Rectangle(knob, Style->Knob, (toggleH - 4.0f) * 0.5f);

    UIVector textPos(track.Right() + Style->Spacing, rect.Top + (rect.Height - Font->LineHeight()) * 0.5f);
    Canvas->Text(Font.get(), caption, textPos, Style->Text);

    return clicked;
}

bool UIWidgets::Radio(const char* title, int& selected, int option) {
    bool isSel = (selected == option);
    if (Check(title, isSel)) {
        selected = option;
        return true;
    }
    return false;
}

bool UIWidgets::Slider(const char* title, float& current, float minimum, float maximum) {
    unsigned int id = Context->Hash(title);
    const char* caption = Context->Caption(title);

    UIRectangle rect = Layout->Stretch(Style->ControlHeight);

    UIVector labelSize = Font->Measure(caption);
    float trackLeft = rect.Left + labelSize.Horizontal + Style->Spacing * 2.0f;
    float trackWidth = std::max(60.0f, rect.Right() - trackLeft - 60.0f);

    UIRectangle track(trackLeft, rect.Top + (rect.Height - 6.0f) * 0.5f, trackWidth, 6.0f);

    bool hovered = (track.Inflate(6.0f).Contains(Input->MousePosition)) && Canvas->Visible(Input->MousePosition);
    bool dragging = (Context->ActiveItem == id);

    if (hovered && Input->MouseDown(0)) {
        Context->ActiveItem = id;
        dragging = true;
    }
    if (!Input->MouseDown(0) && dragging) {
        Context->ActiveItem = 0;
        dragging = false;
    }

    bool changed = false;
    if (dragging) {
        float norm = std::clamp((Input->MousePosition.Horizontal - track.Left) / track.Width, 0.0f, 1.0f);
        float newVal = minimum + norm * (maximum - minimum);
        if (std::abs(newVal - current) > 1e-4f) {
            current = newVal;
            changed = true;
        }
    }

    Canvas->Text(Font.get(), caption, rect.Origin() + UIVector(0.0f, (rect.Height - Font->LineHeight()) * 0.5f), Style->Text);

    // Track background
    Canvas->Rectangle(track, Style->Groove, 3.0f);

    // Track fill
    float fillW = std::clamp((current - minimum) / (maximum - minimum), 0.0f, 1.0f) * track.Width;
    if (fillW > 0.0f) {
        Canvas->Rectangle(UIRectangle(track.Left, track.Top, fillW, track.Height), Style->Accent, 3.0f);
    }

    // Knob
    UIVector knobCenter(track.Left + fillW, track.Top + track.Height * 0.5f);
    Canvas->Circle(knobCenter, Style->KnobSize, Style->Knob);

    // Value text
    char valBuf[32];
    std::snprintf(valBuf, sizeof(valBuf), "%.2f", current);
    Canvas->Text(Font.get(), valBuf, UIVector(track.Right() + Style->Spacing, rect.Top + (rect.Height - Font->LineHeight()) * 0.5f), Style->Faint);

    return changed;
}

bool UIWidgets::SliderWhole(const char* title, int& current, int minimum, int maximum) {
    float val = (float)current;
    if (Slider(title, val, (float)minimum, (float)maximum)) {
        current = (int)std::round(val);
        return true;
    }
    return false;
}

bool UIWidgets::Drag(const char* title, float& current, float speed, float minimum, float maximum, const char* format) {
    return Slider(title, current, minimum, maximum);
}

bool UIWidgets::Number(const char* title, int& value, int step) {
    int v = value;
    if (SliderWhole(title, v, v - 100, v + 100)) {
        value = v;
        return true;
    }
    return false;
}

bool UIWidgets::Decimal(const char* title, float& value, float step, const char* format) {
    float v = value;
    if (Slider(title, v, v - 10.0f, v + 10.0f)) {
        value = v;
        return true;
    }
    return false;
}

bool UIWidgets::Field(const char* title, char* buffer, int capacity, const char* hint) {
    if (!buffer || capacity <= 0) return false;
    unsigned int id = Context->Hash(title);
    const char* caption = Context->Caption(title);

    UIRectangle rect = Layout->Stretch(Style->ControlHeight);

    bool hovered = rect.Contains(Input->MousePosition) && Canvas->Visible(Input->MousePosition);
    if (hovered && Input->MouseReleased(0)) {
        Context->FocusedItem = id;
    }

    bool focused = (Context->FocusedItem == id);
    if (!focused && Input->MouseReleased(0) && !hovered) {
        // Unfocus
    }

    bool changed = false;
    if (focused) {
        for (unsigned int code : Input->Typed) {
            if (code == 8) { // Backspace
                size_t l = std::strlen(buffer);
                if (l > 0) {
                    buffer[l - 1] = '\0';
                    changed = true;
                }
            } else if (code >= 32 && code <= 126) {
                size_t l = std::strlen(buffer);
                if ((int)l < capacity - 1) {
                    buffer[l] = (char)code;
                    buffer[l + 1] = '\0';
                    changed = true;
                }
            }
        }
    }

    Canvas->Rectangle(rect, Style->Control, Style->ControlRounding);
    Canvas->Border(rect, focused ? Style->Focus : (hovered ? Style->Highlight : Style->Outline), Style->ControlRounding, Style->Thickness);

    size_t len = std::strlen(buffer);
    if (len > 0) {
        Canvas->Text(Font.get(), buffer, rect.Origin() + UIVector(Style->PaddingWide * 0.5f, (rect.Height - Font->LineHeight()) * 0.5f), Style->Text);
        if (focused) {
            UIVector txtSize = Font->Measure(buffer);
            float cursorX = rect.Left + Style->PaddingWide * 0.5f + txtSize.Horizontal + 2.0f;
            Canvas->Line(UIVector(cursorX, rect.Top + 6.0f), UIVector(cursorX, rect.Bottom() - 6.0f), Style->Text, 1.5f);
        }
    } else if (hint) {
        Canvas->Text(Font.get(), hint, rect.Origin() + UIVector(Style->PaddingWide * 0.5f, (rect.Height - Font->LineHeight()) * 0.5f), Style->Faint);
    }

    return changed;
}

bool UIWidgets::Choice(const char* title, int& selected, const char* const* names, int count) {
    if (!names || count <= 0) return false;
    unsigned int id = Context->Hash(title);
    const char* caption = Context->Caption(title);

    UIRectangle rect = Layout->Stretch(Style->ControlHeight);
    bool hovered = rect.Contains(Input->MousePosition) && Canvas->Visible(Input->MousePosition);
    bool clicked = hovered && Input->MouseReleased(0);

    if (clicked) {
        selected = (selected + 1) % count;
    }

    Canvas->Rectangle(rect, hovered ? Style->Hovered : Style->Control, Style->ControlRounding);
    Canvas->Border(rect, hovered ? Style->Highlight : Style->Outline, Style->ControlRounding, Style->Thickness);

    const char* curName = (selected >= 0 && selected < count) ? names[selected] : "";
    Canvas->Text(Font.get(), curName, rect.Origin() + UIVector(Style->PaddingWide * 0.5f, (rect.Height - Font->LineHeight()) * 0.5f), Style->Text);

    // Arrow indicator
    Canvas->Text(Font.get(), "v", UIVector(rect.Right() - 18.0f, rect.Top + (rect.Height - Font->LineHeight()) * 0.5f), Style->Faint);

    return clicked;
}

bool UIWidgets::Segments(const char* title, int& selected, const char* const* names, int count) {
    if (!names || count <= 0) return false;
    UIRectangle rect = Layout->Stretch(Style->ControlHeight);

    Canvas->Rectangle(rect, Style->Groove, Style->ControlRounding);
    float segW = rect.Width / count;
    bool changed = false;

    for (int i = 0; i < count; ++i) {
        UIRectangle seg(rect.Left + i * segW, rect.Top, segW, rect.Height);
        bool isSel = (selected == i);
        bool hovered = seg.Contains(Input->MousePosition) && Canvas->Visible(Input->MousePosition);

        if (hovered && Input->MouseReleased(0)) {
            selected = i;
            changed = true;
        }

        if (isSel) {
            Canvas->Rectangle(seg.Shrink(2.0f), Style->Accent, Style->ControlRounding * 0.75f);
        } else if (hovered) {
            Canvas->Rectangle(seg.Shrink(2.0f), Style->Hovered, Style->ControlRounding * 0.75f);
        }

        UIVector textSize = Font->Measure(names[i]);
        UIVector textPos = seg.Center() - UIVector(textSize.Horizontal * 0.5f, Font->LineHeight() * 0.5f);
        Canvas->Text(Font.get(), names[i], textPos, isSel ? UIColor::White() : Style->Text);
    }

    return changed;
}

bool UIWidgets::Selectable(const char* title, bool selected) {
    const char* caption = Context->Caption(title);
    UIRectangle rect = Layout->Stretch(Style->ControlHeight * 0.85f);

    bool hovered = rect.Contains(Input->MousePosition) && Canvas->Visible(Input->MousePosition);
    bool clicked = hovered && Input->MouseReleased(0);

    if (selected) {
        Canvas->Rectangle(rect, Style->Selected, Style->ControlRounding * 0.5f);
    } else if (hovered) {
        Canvas->Rectangle(rect, Style->Hovered, Style->ControlRounding * 0.5f);
    }

    Canvas->Text(Font.get(), caption, rect.Origin() + UIVector(Style->PaddingWide * 0.5f, (rect.Height - Font->LineHeight()) * 0.5f), selected ? UIColor::White() : Style->Text);
    return clicked;
}

void UIWidgets::ProgressBar(float fraction, float width, float height, const char* overlayText, const UIColor* customFill) {
    float h = (height > 0.0f) ? height : Style->ControlHeight;
    UIRectangle rect = (width > 0.0f) ? Layout->Place(UIVector(width, h)) : Layout->Stretch(h);

    float clampedFraction = std::clamp(fraction, 0.0f, 1.0f);

    // Background track
    Canvas->Rectangle(rect, Style->Groove, Style->ControlRounding);
    if (Style->Borders) Canvas->Border(rect, Style->Outline, Style->ControlRounding, Style->Thickness);

    // Fill
    float fillW = rect.Width * clampedFraction;
    if (fillW > 0.0f) {
        UIColor fillCol = customFill ? (*customFill) : Style->Accent;
        Canvas->Rectangle(UIRectangle(rect.Left, rect.Top, fillW, rect.Height), fillCol, Style->ControlRounding);
    }

    // Overlay text or percentage
    char buf[64];
    const char* textToDraw = overlayText;
    if (!textToDraw) {
        std::snprintf(buf, sizeof(buf), "%.0f%%", clampedFraction * 100.0f);
        textToDraw = buf;
    }

    UIVector txtSize = Font->Measure(textToDraw);
    UIVector textPos = rect.Center() - UIVector(txtSize.Horizontal * 0.5f, Font->LineHeight() * 0.5f);
    Canvas->Text(Font.get(), textToDraw, textPos, UIColor::White());
}

void UIWidgets::Separator() {
    UIRectangle rect = Layout->Stretch(Style->Spacing);
    float y = rect.Top + rect.Height * 0.5f;
    Canvas->Line(UIVector(rect.Left, y), UIVector(rect.Right(), y), Style->Outline, Style->Thickness);
}

void UIWidgets::Tooltip(const char* message) {
    if (!message) return;
    UIVector size = Font->Measure(message);
    UIRectangle rect(Input->MousePosition.Horizontal + 12.0f, Input->MousePosition.Vertical + 12.0f,
                     size.Horizontal + Style->PaddingWide, size.Vertical + Style->PaddingTall);

    Canvas->Shadow(rect, Style->Shade, Style->ControlRounding, Style->Softness);
    Canvas->Rectangle(rect, Style->Popup, Style->ControlRounding);
    Canvas->Border(rect, Style->Outline, Style->ControlRounding, Style->Thickness);
    Canvas->Text(Font.get(), message, rect.Origin() + UIVector(Style->PaddingWide * 0.5f, Style->PaddingTall * 0.5f), Style->Text);
}

void UIWidgets::ColorBox(const char* title, const UIColor& color) {
    const char* caption = Context->Caption(title);
    UIRectangle rect = Layout->Stretch(Style->ControlHeight);

    Canvas->Text(Font.get(), caption, rect.Origin() + UIVector(0.0f, (rect.Height - Font->LineHeight()) * 0.5f), Style->Text);
    UIRectangle box(rect.Right() - 50.0f, rect.Top + 4.0f, 40.0f, rect.Height - 8.0f);
    Canvas->Rectangle(box, color, Style->ControlRounding * 0.5f);
    Canvas->Border(box, Style->Outline, Style->ControlRounding * 0.5f, Style->Thickness);
}

} // namespace EunoiaUI
