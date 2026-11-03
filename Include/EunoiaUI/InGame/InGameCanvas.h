#pragma once
#include <memory>
#include <vector>
#include <string>
#include "EunoiaUI/InGame/InGameWidget.h"

namespace EunoiaUI {

class InGameCanvas : public InGameWidget {
public:
    InGameCanvas(const std::string& name = "HUDCanvas");

    void SetReferenceResolution(const UIVector& res) { m_referenceResolution = res; }
    UIVector GetReferenceResolution() const { return m_referenceResolution; }

    void UpdateCanvas(float dt, const UIVector& viewportSize);
    void RenderCanvas();

    bool ProcessInput(const UIVector& mousePos, bool leftClicked);

private:
    UIVector m_referenceResolution{1920.0f, 1080.0f};
    UIVector m_viewportSize{1920.0f, 1080.0f};
    float m_scaleFactor = 1.0f;
};

} // namespace EunoiaUI
