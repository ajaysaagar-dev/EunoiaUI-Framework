#pragma once
#include <memory>
#include <string>
#include "EunoiaUI/Core/UIGeometry.h"

namespace EunoiaUI {

struct UIGlyph {
    UIRectangle SourceUV;
    UIVector Size{0.0f, 0.0f};
    UIVector Offset{0.0f, 0.0f};
    float Advance = 0.0f;
};

class UIFont {
public:
    bool Create(float size = 16.0f, bool isBold = false);
    void Destroy();
    void Rescale(float factor);

    UIVector Measure(const char* text, int length = -1) const;
    float LineHeight() const { return m_size * 1.25f; }
    float FontSize() const { return m_size; }

    const UIGlyph& GetGlyph(char c) const {
        unsigned char uc = (unsigned char)c;
        if (uc >= 128) uc = '?';
        return m_glyphs[uc];
    }

private:
    void BuildEmbeddedFont(float size, bool isBold);

    float m_size = 16.0f;
    float m_scale = 1.0f;
    bool m_bold = false;
    UIGlyph m_glyphs[128] = {};
};

inline auto Font = std::make_unique<UIFont>();
inline auto Heading = std::make_unique<UIFont>();

} // namespace EunoiaUI
