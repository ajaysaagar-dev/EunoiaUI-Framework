#include "EunoiaUI/InGame/InGameWidget.h"
#include <algorithm>

namespace EunoiaUI {

InGameWidget::InGameWidget(const std::string& name) : m_name(name) {}

void InGameWidget::AddChild(std::shared_ptr<InGameWidget> child) {
    if (!child) return;
    child->m_parent = this;
    m_children.push_back(child);
}

void InGameWidget::RemoveChild(std::shared_ptr<InGameWidget> child) {
    if (!child) return;
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if (it != m_children.end()) {
        (*it)->m_parent = nullptr;
        m_children.erase(it);
    }
}

void InGameWidget::ClearChildren() {
    for (auto& ch : m_children) {
        if (ch) ch->m_parent = nullptr;
    }
    m_children.clear();
}

UIRectangle InGameWidget::ComputeBounds(const UIRectangle& parentBounds) {
    float anchorX = parentBounds.Left;
    float anchorY = parentBounds.Top;

    switch (m_anchor) {
        case UIAnchor::TopLeft:
            anchorX = parentBounds.Left;
            anchorY = parentBounds.Top;
            break;
        case UIAnchor::TopCenter:
            anchorX = parentBounds.Left + parentBounds.Width * 0.5f;
            anchorY = parentBounds.Top;
            break;
        case UIAnchor::TopRight:
            anchorX = parentBounds.Right();
            anchorY = parentBounds.Top;
            break;
        case UIAnchor::CenterLeft:
            anchorX = parentBounds.Left;
            anchorY = parentBounds.Top + parentBounds.Height * 0.5f;
            break;
        case UIAnchor::Center:
            anchorX = parentBounds.Left + parentBounds.Width * 0.5f;
            anchorY = parentBounds.Top + parentBounds.Height * 0.5f;
            break;
        case UIAnchor::CenterRight:
            anchorX = parentBounds.Right();
            anchorY = parentBounds.Top + parentBounds.Height * 0.5f;
            break;
        case UIAnchor::BottomLeft:
            anchorX = parentBounds.Left;
            anchorY = parentBounds.Bottom();
            break;
        case UIAnchor::BottomCenter:
            anchorX = parentBounds.Left + parentBounds.Width * 0.5f;
            anchorY = parentBounds.Bottom();
            break;
        case UIAnchor::BottomRight:
            anchorX = parentBounds.Right();
            anchorY = parentBounds.Bottom();
            break;
        case UIAnchor::StretchHorizontal:
            anchorX = parentBounds.Left;
            anchorY = parentBounds.Top + parentBounds.Height * 0.5f;
            m_size.Horizontal = parentBounds.Width;
            break;
        case UIAnchor::StretchVertical:
            anchorX = parentBounds.Left + parentBounds.Width * 0.5f;
            anchorY = parentBounds.Top;
            m_size.Vertical = parentBounds.Height;
            break;
        case UIAnchor::StretchAll:
            anchorX = parentBounds.Left;
            anchorY = parentBounds.Top;
            m_size.Horizontal = parentBounds.Width;
            m_size.Vertical = parentBounds.Height;
            m_pivot = UIVector(0.0f, 0.0f);
            break;
    }

    float finalX = anchorX + m_position.Horizontal - m_size.Horizontal * m_pivot.Horizontal;
    float finalY = anchorY + m_position.Vertical - m_size.Vertical * m_pivot.Vertical;

    m_cachedBounds = UIRectangle(finalX, finalY, m_size.Horizontal, m_size.Vertical);

    // Recursively compute bounds for children
    for (auto& ch : m_children) {
        if (ch) ch->ComputeBounds(m_cachedBounds);
    }

    return m_cachedBounds;
}

void InGameWidget::Update(float dt) {
    for (auto& ch : m_children) {
        if (ch && ch->IsVisible()) ch->Update(dt);
    }
}

void InGameWidget::Render() {
    if (!m_visible || m_alpha <= 0.0f) return;
    for (auto& ch : m_children) {
        if (ch && ch->IsVisible()) ch->Render();
    }
}

} // namespace EunoiaUI
