#include "EunoiaUI/EunoiaUI.h"
#include <iostream>

namespace EunoiaUI {

bool EunoiaUIManager::Initialize(float displayWidth, float displayHeight) {
    if (m_initialized) return true;

    Style->Dark();

    if (!Sheet->Create(512, 512)) {
        return false;
    }

    if (!Font->Create(15.0f, false)) {
        return false;
    }

    if (!Heading->Create(18.0f, true)) {
        return false;
    }

    if (!Arena->Create(1048576)) {
        return false;
    }

    if (!Context->Create()) {
        return false;
    }

    m_displaySize = UIVector(displayWidth, displayHeight);
    m_inGameCanvas = std::make_shared<InGameCanvas>("MainHUD");
    m_inGameCanvas->SetReferenceResolution(m_displaySize);

    m_initialized = true;
    std::cout << "[EunoiaUI] Custom UI Framework initialized successfully (" 
              << displayWidth << "x" << displayHeight << ")\n";
    return true;
}

void EunoiaUIManager::Shutdown() {
    if (!m_initialized) return;

    if (m_inGameCanvas) {
        m_inGameCanvas->ClearChildren();
        m_inGameCanvas.reset();
    }

    Widgets->Destroy();
    Frames->Destroy();
    Heading->Destroy();
    Font->Destroy();
    Sheet->Destroy();
    Arena->Destroy();
    Context->Destroy();
    if (m_customBackend) {
        m_customBackend->Destroy();
        m_customBackend.reset();
    }

    m_initialized = false;
    std::cout << "[EunoiaUI] Framework shutdown.\n";
}

void EunoiaUIManager::BeginFrame(float screenW, float screenH, float dt) {
    if (!m_initialized) {
        Initialize(screenW, screenH);
    }

    m_displaySize = UIVector(screenW, screenH);
    Context->Begin();
    Context->Display = m_displaySize;
    Context->DeltaTime = dt;

    Input->Pointer = UIPointerArrow;
    Arena->Reset();

    Widgets->Sweep();
    Frames->Update();
    Canvas->Begin(m_displaySize);

    // Update in-game HUD tree
    if (m_inGameCanvas && m_inGameCanvas->IsVisible()) {
        m_inGameCanvas->UpdateCanvas(dt, m_displaySize);
    }
}

void EunoiaUIManager::EndFrame() {
    if (!m_initialized) return;

    // Render in-game HUD tree into Canvas
    if (m_inGameCanvas && m_inGameCanvas->IsVisible()) {
        m_inGameCanvas->RenderCanvas();
    }

    Canvas->End();
    Input->Settle();
}

void EunoiaUIManager::Render(void* stream) {
    if (!m_initialized) return;

    const UIDrawData& drawData = Canvas->Data();
    if (drawData.Count <= 0) return;

    if (m_customBackend) {
        m_customBackend->Render(drawData, stream);
    } else {
        // Fallback to Viewport Bridge
        UIEngineBridge::RenderToEngineViewport(drawData, stream);
    }
}

void EunoiaUIManager::ProcessMouseMove(float x, float y) {
    Input->ApplyPosition(x, y);
}

void EunoiaUIManager::ProcessMouseButton(int button, bool down) {
    Input->ApplyButton(button, down);
    if (down && m_inGameCanvas) {
        m_inGameCanvas->ProcessInput(Input->MousePosition, true);
    }
}

void EunoiaUIManager::ProcessMouseWheel(float delta) {
    Input->ApplyWheel(delta);
}

void EunoiaUIManager::ProcessKey(int key, bool down) {
    Input->ApplyKey(key, down);
}

void EunoiaUIManager::ProcessChar(unsigned int codepoint) {
    Input->ApplyText(codepoint);
}

std::shared_ptr<InGameHealthBar> EunoiaUIManager::CreateHealthBar(float current, float maxVal, const UIVector& pos, const UIVector& size) {
    auto bar = std::make_shared<InGameHealthBar>();
    bar->SetValue(current, maxVal);
    bar->SetPosition(pos);
    bar->SetSize(size);
    if (m_inGameCanvas) m_inGameCanvas->AddChild(bar);
    return bar;
}

std::shared_ptr<InGameCrosshair> EunoiaUIManager::CreateCrosshair() {
    auto ch = std::make_shared<InGameCrosshair>();
    if (m_inGameCanvas) m_inGameCanvas->AddChild(ch);
    return ch;
}

std::shared_ptr<InGameButton> EunoiaUIManager::CreateInGameButton(const std::string& text, const UIVector& pos, const UIVector& size, std::function<void()> onClick) {
    auto btn = std::make_shared<InGameButton>(text);
    btn->SetPosition(pos);
    btn->SetSize(size);
    btn->SetOnClick(onClick);
    if (m_inGameCanvas) m_inGameCanvas->AddChild(btn);
    return btn;
}

std::shared_ptr<InGameLabel> EunoiaUIManager::CreateInGameLabel(const std::string& text, const UIVector& pos, const UIColor& color) {
    auto lbl = std::make_shared<InGameLabel>(text);
    lbl->SetPosition(pos);
    lbl->Color = color;
    if (m_inGameCanvas) m_inGameCanvas->AddChild(lbl);
    return lbl;
}

std::shared_ptr<InGamePanel> EunoiaUIManager::CreateInGamePanel(const UIVector& pos, const UIVector& size) {
    auto p = std::make_shared<InGamePanel>();
    p->SetPosition(pos);
    p->SetSize(size);
    if (m_inGameCanvas) m_inGameCanvas->AddChild(p);
    return p;
}

} // namespace EunoiaUI
