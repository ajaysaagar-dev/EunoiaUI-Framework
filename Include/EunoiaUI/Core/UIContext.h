#pragma once
#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include "EunoiaUI/Core/UIGeometry.h"

namespace EunoiaUI {

class UIContext {
public:
    bool Create();
    void Destroy();

    void Begin();

    unsigned int Hash(const char* name) const;
    const char* Caption(const char* name);

    void PushOwner(unsigned int id);
    void PopOwner();

    void PushIdentifier(const char* name);
    void PushIdentifier(int number);
    void PopIdentifier();

    bool Covered(const UIVector& point) const;

    unsigned int HoveredItem = 0;
    unsigned int ActiveItem = 0;
    unsigned int FocusedItem = 0;
    unsigned int FrameIndex = 0;

    bool WantKeyboard = false;
    bool FocusClaimed = false;

    float DeltaTime = 0.016f;
    double Elapsed = 0.0;
    float Framerate = 60.0f;

    bool Shielding = false;
    UIRectangle Shield;
    UIVector Display{1920.0f, 1080.0f};

private:
    std::chrono::steady_clock::time_point m_previousTime;
    bool m_started = false;

    double m_sampleTime = 0.0;
    int m_sampleCount = 0;

    std::vector<unsigned int> m_owners;
    std::vector<unsigned int> m_idStack;
};

inline auto Context = std::make_unique<UIContext>();

} // namespace EunoiaUI
