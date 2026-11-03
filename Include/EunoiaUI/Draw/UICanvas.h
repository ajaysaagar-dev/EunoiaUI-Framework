#pragma once
#include <memory>
#include <vector>
#include "EunoiaUI/Core/UIGeometry.h"
#include "EunoiaUI/Draw/UIDrawData.h"

namespace EunoiaUI {

class UIFont;

class UICanvas {
public:
    void Begin(const UIVector& display);
    void End();

    int Route(int channel);
    void PushClip(const UIRectangle& bounds);
    void PopClip();

    bool Visible(const UIVector& point) const;

    void Rectangle(const UIRectangle& bounds, const UIColor& fill, float rounding = 0.0f);
    void Gradient(const UIRectangle& bounds, const UIColor& first, const UIColor& second, float rounding = 0.0f, bool horizontal = false);
    void Border(const UIRectangle& bounds, const UIColor& stroke, float rounding = 0.0f, float thickness = 1.0f);
    void Shadow(const UIRectangle& bounds, const UIColor& tint, float rounding = 0.0f, float softness = 16.0f);
    void Circle(const UIVector& middle, float radius, const UIColor& fill);
    void Line(const UIVector& p0, const UIVector& p1, const UIColor& stroke, float thickness = 1.0f);
    void Text(const UIFont* font, const char* text, const UIVector& origin, const UIColor& color);
    void Picture(const UIRectangle& bounds, unsigned long long image, const UIColor& tint = UIColor::White(), float rounding = 0.0f);

    const UIDrawData& Data();

private:
    void AddInstance(const UIInstance& item, unsigned long long image = 0);

    UIVector m_display{1920.0f, 1080.0f};
    int m_activeChannel = 0;

    std::vector<UIChannel> m_channels;
    std::vector<UIRectangle> m_clipStack;

    std::vector<UIInstance> m_flatInstances;
    std::vector<UIBatch> m_flatBatches;
    UIDrawData m_drawData;
};

inline auto Canvas = std::make_unique<UICanvas>();

} // namespace EunoiaUI
