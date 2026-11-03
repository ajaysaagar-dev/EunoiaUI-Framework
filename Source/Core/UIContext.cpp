#include "EunoiaUI/Core/UIContext.h"
#include <cstring>

namespace EunoiaUI {

static unsigned int FNV1a(const void* data, size_t length, unsigned int seed = 2166136261u) {
    const unsigned char* bytes = static_cast<const unsigned char*>(data);
    unsigned int hash = seed;
    for (size_t i = 0; i < length; ++i) {
        hash ^= bytes[i];
        hash *= 16777619u;
    }
    return hash;
}

bool UIContext::Create() {
    m_previousTime = std::chrono::steady_clock::now();
    m_started = true;
    m_owners.clear();
    m_idStack.clear();
    FrameIndex = 0;
    return true;
}

void UIContext::Destroy() {
    m_owners.clear();
    m_idStack.clear();
}

void UIContext::Begin() {
    auto now = std::chrono::steady_clock::now();
    if (!m_started) {
        m_previousTime = now;
        m_started = true;
    }
    float dt = std::chrono::duration<float>(now - m_previousTime).count();
    m_previousTime = now;
    if (dt > 0.2f) dt = 0.016f;
    DeltaTime = dt;
    Elapsed += dt;

    m_sampleTime += dt;
    m_sampleCount++;
    if (m_sampleTime >= 0.5) {
        Framerate = (float)(m_sampleCount / m_sampleTime);
        m_sampleTime = 0.0;
        m_sampleCount = 0;
    }

    FrameIndex++;
    FocusClaimed = false;
    Shielding = false;
    m_owners.clear();
    m_idStack.clear();
}

unsigned int UIContext::Hash(const char* name) const {
    if (!name) return 0;
    unsigned int seed = m_idStack.empty() ? 2166136261u : m_idStack.back();
    if (!m_owners.empty()) {
        seed = FNV1a(&m_owners.back(), sizeof(unsigned int), seed);
    }
    return FNV1a(name, std::strlen(name), seed);
}

const char* UIContext::Caption(const char* name) {
    if (!name) return "";
    const char* hashPos = std::strstr(name, "##");
    if (!hashPos) return name;
    // Return text before ##
    static char s_captionBuf[256];
    size_t len = hashPos - name;
    if (len >= 255) len = 255;
    std::memcpy(s_captionBuf, name, len);
    s_captionBuf[len] = '\0';
    return s_captionBuf;
}

void UIContext::PushOwner(unsigned int id) {
    m_owners.push_back(id);
}

void UIContext::PopOwner() {
    if (!m_owners.empty()) m_owners.pop_back();
}

void UIContext::PushIdentifier(const char* name) {
    unsigned int h = Hash(name);
    m_idStack.push_back(h);
}

void UIContext::PushIdentifier(int number) {
    unsigned int seed = m_idStack.empty() ? 2166136261u : m_idStack.back();
    unsigned int h = FNV1a(&number, sizeof(int), seed);
    m_idStack.push_back(h);
}

void UIContext::PopIdentifier() {
    if (!m_idStack.empty()) m_idStack.pop_back();
}

bool UIContext::Covered(const UIVector& point) const {
    if (Shielding && Shield.Contains(point)) return true;
    return false;
}

} // namespace EunoiaUI
