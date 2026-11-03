#pragma once
#include <memory>
#include <string>
#include <vector>
#include <functional>
#include "EunoiaUI/Core/UIGeometry.h"

namespace EunoiaUI {

class InGameWidget : public std::enable_shared_from_this<InGameWidget> {
public:
    InGameWidget(const std::string& name = "Widget");
    virtual ~InGameWidget() = default;

    // Hierarchy
    void AddChild(std::shared_ptr<InGameWidget> child);
    void RemoveChild(std::shared_ptr<InGameWidget> child);
    void ClearChildren();
    const std::vector<std::shared_ptr<InGameWidget>>& GetChildren() const { return m_children; }
    InGameWidget* GetParent() const { return m_parent; }

    // Transform & Anchors
    void SetAnchor(UIAnchor anchor) { m_anchor = anchor; }
    UIAnchor GetAnchor() const { return m_anchor; }

    void SetPosition(const UIVector& pos) { m_position = pos; }
    UIVector GetPosition() const { return m_position; }

    void SetSize(const UIVector& size) { m_size = size; }
    UIVector GetSize() const { return m_size; }

    void SetPivot(const UIVector& pivot) { m_pivot = pivot; }
    UIVector GetPivot() const { return m_pivot; }

    // Visibility & Lifecycle
    void SetVisible(bool v) { m_visible = v; }
    bool IsVisible() const { return m_visible; }

    void SetAlpha(float a) { m_alpha = std::clamp(a, 0.0f, 1.0f); }
    float GetAlpha() const { return m_alpha; }

    // Compute absolute screen rectangle from parent bounds
    virtual UIRectangle ComputeBounds(const UIRectangle& parentBounds);

    virtual void Update(float dt);
    virtual void Render();

    // Event handling
    virtual bool OnClick() { if (m_onClick) { m_onClick(); return true; } return false; }
    void SetOnClick(std::function<void()> cb) { m_onClick = cb; }

protected:
    std::string m_name;
    InGameWidget* m_parent = nullptr;
    std::vector<std::shared_ptr<InGameWidget>> m_children;

    UIAnchor m_anchor = UIAnchor::Center;
    UIVector m_position{0.0f, 0.0f};
    UIVector m_size{100.0f, 40.0f};
    UIVector m_pivot{0.5f, 0.5f}; // Center pivot default

    UIRectangle m_cachedBounds;
    bool m_visible = true;
    float m_alpha = 1.0f;

    std::function<void()> m_onClick;
};

} // namespace EunoiaUI
