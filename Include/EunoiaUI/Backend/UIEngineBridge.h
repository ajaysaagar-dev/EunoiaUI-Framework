#pragma once
#include "EunoiaUI/Draw/UIDrawData.h"

namespace EunoiaUI {

class UIEngineBridge {
public:
    static void RenderToEngineViewport(const UIDrawData& data, void* imDrawListPtr = nullptr);
};

} // namespace EunoiaUI
