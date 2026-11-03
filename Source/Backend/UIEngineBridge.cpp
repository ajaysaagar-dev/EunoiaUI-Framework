#include "EunoiaUI/Backend/UIEngineBridge.h"
#include <algorithm>

#if __has_include(<imgui.h>)
#include <imgui.h>
#define EUNOIA_HAS_IMGUI 1
#endif

namespace EunoiaUI {

void UIEngineBridge::RenderToEngineViewport(const UIDrawData& data, void* imDrawListPtr) {
    if (data.Count <= 0 || !data.Items) return;

#if defined(EUNOIA_HAS_IMGUI)
    ImDrawList* drawList = imDrawListPtr ? static_cast<ImDrawList*>(imDrawListPtr) : ImGui::GetForegroundDrawList();
    if (!drawList) return;

    for (int i = 0; i < data.Count; ++i) {
        const auto& item = data.Items[i];
        ImVec2 pMin(item.Bounds.Left, item.Bounds.Top);
        ImVec2 pMax(item.Bounds.Right(), item.Bounds.Bottom());
        ImVec4 clipRect(item.ClipLeft, item.ClipTop, item.ClipRight, item.ClipBottom);

        drawList->PushClipRect(ImVec2(clipRect.x, clipRect.y), ImVec2(clipRect.z, clipRect.w), true);

        if (item.Flags & UIFlagCircle) {
            float radius = item.Rounding;
            ImVec2 center((pMin.x + pMax.x) * 0.5f, (pMin.y + pMax.y) * 0.5f);
            drawList->AddCircleFilled(center, radius, item.ColorStart);
        } else if (item.Flags & UIFlagLine) {
            drawList->AddLine(pMin, pMax, item.ColorStart, item.Thickness);
        } else if (item.Flags & UIFlagShadow) {
            // Draw soft shadow
            drawList->AddRectFilled(pMin, pMax, item.ColorStart, item.Rounding);
        } else if (item.Flags & UIFlagBorder) {
            drawList->AddRect(pMin, pMax, item.ColorStart, item.Rounding, 0, item.Thickness);
        } else if (item.Flags & UIFlagTextured) {
            ImVec2 uv0(item.Source.Left, item.Source.Top);
            ImVec2 uv1(item.Source.Right(), item.Source.Bottom());
            drawList->AddImage((ImTextureID)(uintptr_t)item.ImageHandle, pMin, pMax, uv0, uv1, item.ColorStart);
        } else if (item.Flags & UIFlagGradient) {
            drawList->AddRectFilledMultiColor(pMin, pMax, item.ColorStart, item.ColorFinish, item.ColorFinish, item.ColorStart);
        } else {
            drawList->AddRectFilled(pMin, pMax, item.ColorStart, item.Rounding);
        }

        drawList->PopClipRect();
    }
#endif
}

} // namespace EunoiaUI
