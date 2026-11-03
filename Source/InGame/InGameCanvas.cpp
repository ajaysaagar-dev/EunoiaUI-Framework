#include "EunoiaUI/InGame/InGameCanvas.h"
#include <algorithm>

namespace EunoiaUI {

InGameCanvas::InGameCanvas(const std::string& name) : InGameWidget(name) {
    m_anchor = UIAnchor::StretchAll;
    m_pivot = UIVector(0.0f, 0.0f);
}

void InGameCanvas::UpdateCanvas(float dt, const UIVector& viewportSize) {
    m_viewportSize = viewportSize;
    float scaleX = (m_referenceResolution.Horizontal > 0.0f) ? (viewportSize.Horizontal / m_referenceResolution.Horizontal) : 1.0f;
    float scaleY = (m_referenceResolution.Vertical > 0.0f) ? (viewportSize.Vertical / m_referenceResolution.Vertical) : 1.0f;
    m_scaleFactor = std::min(scaleX, scaleY);

    UIRectangle screenRect(0.0f, 0.0f, viewportSize.Horizontal, viewportSize.Vertical);
    ComputeBounds(screenRect);
    Update(dt);
}

void InGameCanvas::RenderCanvas() {
    Render();
}

bool InGameCanvas::ProcessInput(const UIVector& mousePos, bool leftClicked) {
    if (!m_visible || !leftClicked) return false;

    // Hit-test children in reverse order (topmost first)
    for (auto it = m_children.rbegin(); it != m_children.rend(); ++it) {
        auto& ch = *it;
        if (ch && ch->IsVisible()) {
            if (ch->ComputeBounds(m_cachedBounds).Contains(mousePos)) {
                if (ch->OnClick()) return true;
            }
        }
    }
    return false;
}

} // namespace EunoiaUI
