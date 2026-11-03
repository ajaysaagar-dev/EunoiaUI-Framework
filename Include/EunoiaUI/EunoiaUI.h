#pragma once

// ============================================================================
// Eunoia Engine — Unified Custom UI Framework System
// Powered by modern standalone architecture for both Engine & In-Game UI.
// ============================================================================

#include "EunoiaUI/Core/UIGeometry.h"
#include "EunoiaUI/Core/UIArena.h"
#include "EunoiaUI/Core/UIContext.h"
#include "EunoiaUI/Core/UIInput.h"
#include "EunoiaUI/Core/UIStyle.h"
#include "EunoiaUI/Draw/UIDrawData.h"
#include "EunoiaUI/Draw/UIGraphics.h"
#include "EunoiaUI/Draw/UICanvas.h"
#include "EunoiaUI/Text/UIFont.h"
#include "EunoiaUI/Text/UISheet.h"
#include "EunoiaUI/Layout/UILayout.h"
#include "EunoiaUI/Widgets/UIWidgets.h"
#include "EunoiaUI/Desktop/UIFrames.h"
#include "EunoiaUI/InGame/InGameCanvas.h"
#include "EunoiaUI/InGame/InGameWidget.h"
#include "EunoiaUI/InGame/InGameElements.h"
#include "EunoiaUI/Backend/UID3D12Backend.h"
#include "EunoiaUI/Backend/UIEngineBridge.h"

#include <memory>
#include <string>
#include <vector>

namespace EunoiaUI {

class EunoiaUIManager {
public:
    static EunoiaUIManager& Get() {
        static EunoiaUIManager instance;
        return instance;
    }

    bool Initialize(float displayWidth = 1920.0f, float displayHeight = 1080.0f);
    void Shutdown();
    bool IsInitialized() const { return m_initialized; }

    // Frame Lifecycle
    void BeginFrame(float screenW, float screenH, float dt = 0.016f);
    void EndFrame();
    void Render(void* stream = nullptr);

    // Input Dispatching
    void ProcessMouseMove(float x, float y);
    void ProcessMouseButton(int button, bool down);
    void ProcessMouseWheel(float delta);
    void ProcessKey(int key, bool down);
    void ProcessChar(unsigned int codepoint);

    // In-Game UI Shortcuts
    std::shared_ptr<InGameCanvas> GetInGameCanvas() { return m_inGameCanvas; }

    std::shared_ptr<InGameHealthBar> CreateHealthBar(float current = 100.0f, float maxVal = 100.0f,
                                                     const UIVector& pos = UIVector(30.0f, -30.0f),
                                                     const UIVector& size = UIVector(240.0f, 22.0f));

    std::shared_ptr<InGameCrosshair> CreateCrosshair();

    std::shared_ptr<InGameButton> CreateInGameButton(const std::string& text,
                                                     const UIVector& pos,
                                                     const UIVector& size,
                                                     std::function<void()> onClick = nullptr);

    std::shared_ptr<InGameLabel> CreateInGameLabel(const std::string& text,
                                                   const UIVector& pos,
                                                   const UIColor& color = UIColor::White());

    std::shared_ptr<InGamePanel> CreateInGamePanel(const UIVector& pos,
                                                   const UIVector& size);

    void SetBackend(std::unique_ptr<UIGraphics> backend) { m_customBackend = std::move(backend); }
    UIGraphics* GetBackend() const { return m_customBackend.get(); }

private:
    EunoiaUIManager() = default;
    ~EunoiaUIManager() = default;

    bool m_initialized = false;
    std::shared_ptr<InGameCanvas> m_inGameCanvas;
    std::unique_ptr<UIGraphics> m_customBackend;
    UIVector m_displaySize{1920.0f, 1080.0f};
};

} // namespace EunoiaUI
