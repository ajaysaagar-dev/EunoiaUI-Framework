#include "EunoiaUI/Draw/UICanvas.h"
#include "EunoiaUI/Text/UIFont.h"
#include "EunoiaUI/Text/UISheet.h"
#include <cstring>
#include <algorithm>

namespace EunoiaUI {

void UICanvas::Begin(const UIVector& display) {
    m_display = display;
    m_activeChannel = 0;
    m_channels.clear();
    m_channels.resize(256);
    m_clipStack.clear();
    m_clipStack.push_back(UIRectangle(0.0f, 0.0f, display.Horizontal, display.Vertical));
}

void UICanvas::End() {
    m_flatInstances.clear();
    m_flatBatches.clear();

    for (const auto& ch : m_channels) {
        if (ch.Items.empty()) continue;
        int startOffset = (int)m_flatInstances.size();

        for (const auto& b : ch.Marks) {
            UIBatch flatB = b;
            flatB.Offset += startOffset;
            m_flatBatches.push_back(flatB);
        }

        m_flatInstances.insert(m_flatInstances.end(), ch.Items.begin(), ch.Items.end());
    }

    m_drawData.Items = m_flatInstances.empty() ? nullptr : m_flatInstances.data();
    m_drawData.Count = (int)m_flatInstances.size();
    m_drawData.Batches = m_flatBatches.empty() ? nullptr : m_flatBatches.data();
    m_drawData.BatchCount = (int)m_flatBatches.size();
    m_drawData.Extent = m_display;
}

const UIDrawData& UICanvas::Data() {
    return m_drawData;
}

int UICanvas::Route(int channel) {
    int prev = m_activeChannel;
    m_activeChannel = std::clamp(channel, 0, (int)m_channels.size() - 1);
    return prev;
}

void UICanvas::PushClip(const UIRectangle& bounds) {
    UIRectangle current = m_clipStack.empty() ? UIRectangle(0.0f, 0.0f, m_display.Horizontal, m_display.Vertical) : m_clipStack.back();
    m_clipStack.push_back(current.Clip(bounds));
}

void UICanvas::PopClip() {
    if (m_clipStack.size() > 1) {
        m_clipStack.pop_back();
    }
}

bool UICanvas::Visible(const UIVector& point) const {
    if (m_clipStack.empty()) return true;
    return m_clipStack.back().Contains(point);
}

void UICanvas::AddInstance(const UIInstance& item, unsigned long long image) {
    if (m_activeChannel >= (int)m_channels.size()) {
        m_channels.resize(m_activeChannel + 1);
    }
    auto& ch = m_channels[m_activeChannel];

    if (!ch.Marks.empty() && ch.Marks.back().Image == image) {
        ch.Marks.back().Count++;
    } else {
        UIBatch b;
        b.Offset = (int)ch.Items.size();
        b.Count = 1;
        b.Image = image;
        ch.Marks.push_back(b);
    }

    ch.Items.push_back(item);
}

void UICanvas::Rectangle(const UIRectangle& bounds, const UIColor& fill, float rounding) {
    if (bounds.Width <= 0.0f || bounds.Height <= 0.0f) return;
    const auto& clip = m_clipStack.back();

    UIInstance inst;
    inst.Bounds = bounds;
    inst.ClipLeft = clip.Left;
    inst.ClipTop = clip.Top;
    inst.ClipRight = clip.Right();
    inst.ClipBottom = clip.Bottom();
    inst.ColorStart = fill.Pack();
    inst.ColorFinish = fill.Pack();
    inst.Rounding = rounding;
    inst.Flags = (rounding > 0.5f) ? UIFlagRounded : 0;

    AddInstance(inst, 0);
}

void UICanvas::Gradient(const UIRectangle& bounds, const UIColor& first, const UIColor& second, float rounding, bool horizontal) {
    if (bounds.Width <= 0.0f || bounds.Height <= 0.0f) return;
    const auto& clip = m_clipStack.back();

    UIInstance inst;
    inst.Bounds = bounds;
    inst.ClipLeft = clip.Left;
    inst.ClipTop = clip.Top;
    inst.ClipRight = clip.Right();
    inst.ClipBottom = clip.Bottom();
    inst.ColorStart = first.Pack();
    inst.ColorFinish = second.Pack();
    inst.Rounding = rounding;
    inst.Flags = UIFlagGradient | ((rounding > 0.5f) ? UIFlagRounded : 0);
    inst.Spare = horizontal ? 1.0f : 0.0f;

    AddInstance(inst, 0);
}

void UICanvas::Border(const UIRectangle& bounds, const UIColor& stroke, float rounding, float thickness) {
    if (bounds.Width <= 0.0f || bounds.Height <= 0.0f || thickness <= 0.0f) return;
    const auto& clip = m_clipStack.back();

    UIInstance inst;
    inst.Bounds = bounds;
    inst.ClipLeft = clip.Left;
    inst.ClipTop = clip.Top;
    inst.ClipRight = clip.Right();
    inst.ClipBottom = clip.Bottom();
    inst.ColorStart = stroke.Pack();
    inst.ColorFinish = stroke.Pack();
    inst.Rounding = rounding;
    inst.Thickness = thickness;
    inst.Flags = UIFlagBorder | ((rounding > 0.5f) ? UIFlagRounded : 0);

    AddInstance(inst, 0);
}

void UICanvas::Shadow(const UIRectangle& bounds, const UIColor& tint, float rounding, float softness) {
    if (bounds.Width <= 0.0f || bounds.Height <= 0.0f || softness <= 0.0f) return;
    const auto& clip = m_clipStack.back();

    UIInstance inst;
    inst.Bounds = bounds.Inflate(softness * 0.5f);
    inst.ClipLeft = clip.Left;
    inst.ClipTop = clip.Top;
    inst.ClipRight = clip.Right();
    inst.ClipBottom = clip.Bottom();
    inst.ColorStart = tint.Pack();
    inst.ColorFinish = tint.Pack();
    inst.Rounding = rounding;
    inst.Softness = softness;
    inst.Flags = UIFlagShadow;

    AddInstance(inst, 0);
}

void UICanvas::Circle(const UIVector& middle, float radius, const UIColor& fill) {
    if (radius <= 0.0f) return;
    const auto& clip = m_clipStack.back();

    UIInstance inst;
    inst.Bounds = UIRectangle(middle.Horizontal - radius, middle.Vertical - radius, radius * 2.0f, radius * 2.0f);
    inst.ClipLeft = clip.Left;
    inst.ClipTop = clip.Top;
    inst.ClipRight = clip.Right();
    inst.ClipBottom = clip.Bottom();
    inst.ColorStart = fill.Pack();
    inst.ColorFinish = fill.Pack();
    inst.Rounding = radius;
    inst.Flags = UIFlagCircle;

    AddInstance(inst, 0);
}

void UICanvas::Line(const UIVector& p0, const UIVector& p1, const UIColor& stroke, float thickness) {
    const auto& clip = m_clipStack.back();

    float dx = p1.Horizontal - p0.Horizontal;
    float dy = p1.Vertical - p0.Vertical;
    float len = std::sqrt(dx * dx + dy * dy);
    if (len <= 1e-4f) return;

    UIInstance inst;
    inst.Bounds = UIRectangle(std::min(p0.Horizontal, p1.Horizontal), std::min(p0.Vertical, p1.Vertical),
                              std::max(std::abs(dx), thickness), std::max(std::abs(dy), thickness));
    inst.ClipLeft = clip.Left;
    inst.ClipTop = clip.Top;
    inst.ClipRight = clip.Right();
    inst.ClipBottom = clip.Bottom();
    inst.ColorStart = stroke.Pack();
    inst.ColorFinish = stroke.Pack();
    inst.Thickness = thickness;
    inst.Flags = UIFlagLine;

    AddInstance(inst, 0);
}

void UICanvas::Text(const UIFont* font, const char* text, const UIVector& origin, const UIColor& color) {
    if (!font || !text) return;
    const auto& clip = m_clipStack.back();

    float penX = origin.Horizontal;
    float penY = origin.Vertical;
    unsigned int col = color.Pack();

    size_t len = std::strlen(text);
    for (size_t i = 0; i < len; ++i) {
        char c = text[i];
        if (c == '\n') {
            penX = origin.Horizontal;
            penY += font->LineHeight();
            continue;
        }

        const UIGlyph& g = font->GetGlyph(c);
        if (c != ' ') {
            UIInstance inst;
            inst.Bounds = UIRectangle(penX + g.Offset.Horizontal, penY + g.Offset.Vertical, g.Size.Horizontal, g.Size.Vertical);
            inst.ClipLeft = clip.Left;
            inst.ClipTop = clip.Top;
            inst.ClipRight = clip.Right();
            inst.ClipBottom = clip.Bottom();
            inst.Source = g.SourceUV;
            inst.ColorStart = col;
            inst.ColorFinish = col;
            inst.Flags = UIFlagTextured;

            AddInstance(inst, Sheet->ImageHandle);
        }
        penX += g.Advance;
    }
}

void UICanvas::Picture(const UIRectangle& bounds, unsigned long long image, const UIColor& tint, float rounding) {
    if (bounds.Width <= 0.0f || bounds.Height <= 0.0f) return;
    const auto& clip = m_clipStack.back();

    UIInstance inst;
    inst.Bounds = bounds;
    inst.ClipLeft = clip.Left;
    inst.ClipTop = clip.Top;
    inst.ClipRight = clip.Right();
    inst.ClipBottom = clip.Bottom();
    inst.Source = UIRectangle(0.0f, 0.0f, 1.0f, 1.0f);
    inst.ColorStart = tint.Pack();
    inst.ColorFinish = tint.Pack();
    inst.Rounding = rounding;
    inst.Flags = UIFlagTextured | ((rounding > 0.5f) ? UIFlagRounded : 0);

    AddInstance(inst, image);
}

} // namespace EunoiaUI
