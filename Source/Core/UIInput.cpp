#include "EunoiaUI/Core/UIInput.h"
#include <cstring>

namespace EunoiaUI {

void UIInput::Settle() {
    for (int i = 0; i < 3; ++i) {
        m_pressing[i] = false;
        m_releasing[i] = false;
        m_doubling[i] = false;
    }
    for (int i = 0; i < 256; ++i) {
        m_starting[i] = false;
        m_ending[i] = false;
    }
    WheelDelta = 0.0f;
    MouseDelta = UIVector(0.0f, 0.0f);
    Typed.clear();
}

bool UIInput::MouseDown(int button) const {
    if (button < 0 || button >= 3) return false;
    return m_buttons[button];
}

bool UIInput::MousePressed(int button) const {
    if (button < 0 || button >= 3) return false;
    return m_pressing[button];
}

bool UIInput::MouseReleased(int button) const {
    if (button < 0 || button >= 3) return false;
    return m_releasing[button];
}

bool UIInput::MouseDouble(int button) const {
    if (button < 0 || button >= 3) return false;
    return m_doubling[button];
}

bool UIInput::KeyDownState(int key) const {
    if (key < 0 || key >= 256) return false;
    return m_keys[key];
}

bool UIInput::KeyPressed(int key) const {
    if (key < 0 || key >= 256) return false;
    return m_starting[key];
}

bool UIInput::KeyReleased(int key) const {
    if (key < 0 || key >= 256) return false;
    return m_ending[key];
}

bool UIInput::Shift() const {
    return KeyDownState(UIKeyShift);
}

bool UIInput::Control() const {
    return KeyDownState(UIKeyControl);
}

bool UIInput::Alt() const {
    return KeyDownState(UIKeyAlt);
}

void UIInput::ApplyButton(int button, bool state) {
    if (button < 0 || button >= 3) return;
    if (state && !m_buttons[button]) {
        m_pressing[button] = true;
        PressPosition = MousePosition;
    } else if (!state && m_buttons[button]) {
        m_releasing[button] = true;
    }
    m_buttons[button] = state;
}

void UIInput::ApplyDouble(int button) {
    if (button < 0 || button >= 3) return;
    m_doubling[button] = true;
}

void UIInput::ApplyPosition(float across, float down) {
    UIVector newPos(across, down);
    MouseDelta = newPos - MousePosition;
    MousePosition = newPos;
}

void UIInput::ApplyWheel(float amount) {
    WheelDelta += amount;
}

void UIInput::ApplyKey(int key, bool state) {
    if (key < 0 || key >= 256) return;
    if (state && !m_keys[key]) {
        m_starting[key] = true;
    } else if (!state && m_keys[key]) {
        m_ending[key] = true;
    }
    m_keys[key] = state;
}

void UIInput::ApplyText(unsigned int codepoint) {
    Typed.push_back(codepoint);
}

void UIInput::ApplyCancel() {
    for (int i = 0; i < 3; ++i) {
        m_buttons[i] = false;
        m_pressing[i] = false;
        m_releasing[i] = false;
        m_doubling[i] = false;
    }
    for (int i = 0; i < 256; ++i) {
        m_keys[i] = false;
        m_starting[i] = false;
        m_ending[i] = false;
    }
    WheelDelta = 0.0f;
    Typed.clear();
}

} // namespace EunoiaUI
