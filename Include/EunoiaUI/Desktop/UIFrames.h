#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include "EunoiaUI/Core/UIGeometry.h"

namespace EunoiaUI {

inline constexpr unsigned int UIFrameMove     = 1 << 0;
inline constexpr unsigned int UIFrameResize   = 1 << 1;
inline constexpr unsigned int UIFrameCollapse = 1 << 2;
inline constexpr unsigned int UIFrameClose    = 1 << 3;
inline constexpr unsigned int UIFrameDefault  = 15;

class UIFrameState {
public:
    UIVector Position{60.0f, 60.0f};
    UIVector Size{380.0f, 320.0f};
    UIVector GrabOffset{0.0f, 0.0f};

    UIRectangle Bounds;
    UIRectangle Inner;

    char Title[64] = {};
    unsigned int Options = UIFrameDefault;

    bool Collapsed = false;
    bool Dragging = false;
    bool Resizing = false;
    bool Initialized = false;
};

class UIFrames {
public:
    void Update();

    bool Begin(const char* title, bool* open = nullptr, unsigned int options = UIFrameDefault,
               const UIVector& anchor = UIVector(60.0f, 60.0f), const UIVector& extent = UIVector(380.0f, 320.0f));
    void End();

    void Destroy();
    UIFrameState* Find(unsigned int id);

private:
    std::unordered_map<unsigned int, UIFrameState> m_states;
    std::vector<unsigned int> m_stack;
    std::vector<unsigned int> m_order;
    unsigned int m_activeDragWindow = 0;
    unsigned int m_activeResizeWindow = 0;
};

inline auto Frames = std::make_unique<UIFrames>();

} // namespace EunoiaUI
