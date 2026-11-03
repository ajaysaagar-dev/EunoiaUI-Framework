#pragma once
#include <memory>
#include <vector>
#include "EunoiaUI/Core/UIGeometry.h"

namespace EunoiaUI {

// Cursor pointer shapes
inline constexpr int UIPointerArrow   = 0;
inline constexpr int UIPointerAcross  = 1;
inline constexpr int UIPointerDown    = 2;
inline constexpr int UIPointerSlant   = 3;
inline constexpr int UIPointerMove    = 4;
inline constexpr int UIPointerText    = 5;
inline constexpr int UIPointerHand    = 6;

// Key codes
inline constexpr int UIKeyBackspace = 8;
inline constexpr int UIKeyTab       = 9;
inline constexpr int UIKeyEnter     = 13;
inline constexpr int UIKeyShift     = 16;
inline constexpr int UIKeyControl   = 17;
inline constexpr int UIKeyAlt       = 18;
inline constexpr int UIKeyEscape    = 27;
inline constexpr int UIKeySpace     = 32;
inline constexpr int UIKeyPageUp    = 33;
inline constexpr int UIKeyPageDown  = 34;
inline constexpr int UIKeyEnd       = 35;
inline constexpr int UIKeyHome      = 36;
inline constexpr int UIKeyLeft      = 37;
inline constexpr int UIKeyUp        = 38;
inline constexpr int UIKeyRight     = 39;
inline constexpr int UIKeyDown      = 40;
inline constexpr int UIKeyDelete    = 46;
inline constexpr int UIKeyLetterA   = 65;
inline constexpr int UIKeyLetterC   = 67;
inline constexpr int UIKeyLetterV   = 86;
inline constexpr int UIKeyLetterX   = 88;
inline constexpr int UIKeyLetterY   = 89;
inline constexpr int UIKeyLetterZ   = 90;

class UIInput {
public:
    void Settle(); // Called at end of frame to advance state

    bool MouseDown(int button = 0) const;
    bool MousePressed(int button = 0) const;
    bool MouseReleased(int button = 0) const;
    bool MouseDouble(int button = 0) const;

    bool KeyDownState(int key) const;
    bool KeyPressed(int key) const;
    bool KeyReleased(int key) const;

    bool Shift() const;
    bool Control() const;
    bool Alt() const;

    void ApplyButton(int button, bool state);
    void ApplyDouble(int button);
    void ApplyPosition(float across, float down);
    void ApplyWheel(float amount);
    void ApplyKey(int key, bool state);
    void ApplyText(unsigned int codepoint);
    void ApplyCancel();

    UIVector MousePosition{0.0f, 0.0f};
    UIVector PressPosition{0.0f, 0.0f};
    UIVector MouseDelta{0.0f, 0.0f};
    float WheelDelta = 0.0f;
    int Pointer = UIPointerArrow;

    std::vector<unsigned int> Typed;

private:
    bool m_buttons[3] = { false, false, false };
    bool m_pressing[3] = { false, false, false };
    bool m_releasing[3] = { false, false, false };
    bool m_doubling[3] = { false, false, false };

    bool m_keys[256] = {};
    bool m_starting[256] = {};
    bool m_ending[256] = {};
};

inline auto Input = std::make_unique<UIInput>();

} // namespace EunoiaUI
