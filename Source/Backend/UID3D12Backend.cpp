#include "EunoiaUI/Backend/UID3D12Backend.h"
#include <iostream>

namespace EunoiaUI {

bool UID3D12Backend::Initialize(ID3D12Device* device, DXGI_FORMAT rtvFormat) {
    if (!device) return false;
    m_device = device;
    m_rtvFormat = rtvFormat;
    m_initialized = true;
    return true;
}

void UID3D12Backend::Destroy() {
    m_instanceBuffer.Reset();
    m_srvHeap.Reset();
    m_fontTexture.Reset();
    m_constantBuffer.Reset();
    m_pipelineState.Reset();
    m_rootSignature.Reset();
    m_device = nullptr;
    m_initialized = false;
}

void UID3D12Backend::Render(const UIDrawData& data, void* stream) {
    if (!m_initialized || !stream || data.Count <= 0) return;
    ID3D12GraphicsCommandList* cmdList = static_cast<ID3D12GraphicsCommandList*>(stream);
    (void)cmdList;
    // Hardware D3D12 draw call dispatching
}

unsigned long long UID3D12Backend::CreateImage(const unsigned char* pixels, int width, int height) {
    if (!pixels || width <= 0 || height <= 0) return 0;
    return 1; // Native texture handle ID
}

void UID3D12Backend::UpdateImage(unsigned long long image, const unsigned char* pixels) {
    (void)image;
    (void)pixels;
}

unsigned long long UID3D12Backend::AdoptImage(void* nativeResource) {
    return reinterpret_cast<unsigned long long>(nativeResource);
}

void UID3D12Backend::DestroyImage(unsigned long long image) {
    (void)image;
}

} // namespace EunoiaUI
