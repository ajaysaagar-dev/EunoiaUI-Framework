#include "EunoiaUI/Desktop/UIFrames.h"
#include "EunoiaUI/Core/UIContext.h"
#include "EunoiaUI/Core/UIInput.h"
#include "EunoiaUI/Core/UIStyle.h"
#include "EunoiaUI/Draw/UICanvas.h"
#include "EunoiaUI/Layout/UILayout.h"
#include "EunoiaUI/Text/UIFont.h"
#include <cstring>
#include <algorithm>

namespace EunoiaUI {

void UIFrames::Update() {
    if (!Input->MouseDown(0)) {
        m_activeDragWindow = 0;
        m_activeResizeWindow = 0;
    }
}

void UIFrames::Destroy() {
    m_states.clear();
    m_stack.clear();
    m_order.clear();
}

UIFrameState* UIFrames::Find(unsigned int id) {
    auto it = m_states.find(id);
    return (it != m_states.end()) ? &it->second : nullptr;
}

bool UIFrames::Begin(const char* title, bool* open, unsigned int options, const UIVector& anchor, const UIVector& extent) {
    if (open && !(*open)) return false;

    unsigned int id = Context->Hash(title);
    auto& state = m_states[id];

    if (!state.Initialized) {
        state.Position = anchor;
        state.Size = extent;
        state.Options = options;
        std::strncpy(state.Title, title ? title : "Window", sizeof(state.Title) - 1);
        state.Initialized = true;
        m_order.push_back(id);
    }

    state.Bounds = UIRectangle(state.Position, state.Size);
    UIRectangle titleBar(state.Position, UIVector(state.Size.Horizontal, Style->TitleHeight));

    // Handle Window Dragging
    if (options & UIFrameMove) {
        if (m_activeDragWindow == id) {
            state.Position = Input->MousePosition - state.GrabOffset;
            state.Bounds = UIRectangle(state.Position, state.Size);
        } else if (titleBar.Contains(Input->MousePosition) && Input->MousePressed(0) && m_activeDragWindow == 0) {
            m_activeDragWindow = id;
            state.GrabOffset = Input->MousePosition - state.Position;
        }
    }

    // Handle Window Resizing
    if (options & UIFrameResize) {
        UIRectangle resizeGrip(state.Bounds.Right() - Style->GripSize, state.Bounds.Bottom() - Style->GripSize, Style->GripSize, Style->GripSize);
        if (m_activeResizeWindow == id) {
            state.Size.Horizontal = std::max(150.0f, Input->MousePosition.Horizontal - state.Position.Horizontal);
            state.Size.Vertical = std::max(80.0f, Input->MousePosition.Vertical - state.Position.Vertical);
            state.Bounds = UIRectangle(state.Position, state.Size);
        } else if (resizeGrip.Contains(Input->MousePosition) && Input->MousePressed(0) && m_activeResizeWindow == 0) {
            m_activeResizeWindow = id;
        }
    }

    // Draw Window Shadow & Body
    if (Style->Shadows) {
        Canvas->Shadow(state.Bounds, Style->Shade, Style->Rounding, Style->Softness);
    }
    Canvas->Rectangle(state.Bounds, Style->Surface, Style->Rounding);
    if (Style->Borders) {
        Canvas->Border(state.Bounds, Style->Outline, Style->Rounding, Style->Thickness);
    }

    // Title Bar Background
    Canvas->Rectangle(titleBar, Style->Header, Style->Rounding);

    // Title Text
    const char* caption = Context->Caption(title);
    UIVector titlePos = titleBar.Origin() + UIVector(Style->PaddingWide, (titleBar.Height - Font->LineHeight()) * 0.5f);
    Canvas->Text(Font.get(), caption, titlePos, Style->Text);

    // Close button
    if ((options & UIFrameClose) && open) {
        float btnSize = 22.0f;
        UIRectangle closeBtn(titleBar.Right() - btnSize - 10.0f, titleBar.Top + (titleBar.Height - btnSize) * 0.5f, btnSize, btnSize);
        bool closeHover = closeBtn.Contains(Input->MousePosition);
        if (closeHover) {
            Canvas->Rectangle(closeBtn, Style->Danger, Style->ControlRounding * 0.5f);
            if (Input->MouseReleased(0)) {
                *open = false;
            }
        }
        Canvas->Text(Font.get(), "X", closeBtn.Origin() + UIVector(7.0f, 4.0f), closeHover ? UIColor::White() : Style->Faint);
    }

    // Window Content Area
    state.Inner = UIRectangle(
        state.Position.Horizontal + Style->PaddingWide,
        state.Position.Vertical + Style->TitleHeight + Style->PaddingTall,
        state.Size.Horizontal - Style->PaddingWide * 2.0f,
        state.Size.Vertical - Style->TitleHeight - Style->PaddingTall * 2.0f
    );

    Canvas->PushClip(state.Inner);
    Layout->Begin(state.Inner);

    m_stack.push_back(id);
    Context->PushOwner(id);
    return true;
}

void UIFrames::End() {
    if (m_stack.empty()) return;
    Layout->End();
    Canvas->PopClip();
    Context->PopOwner();
    m_stack.pop_back();
}

} // namespace EunoiaUI
