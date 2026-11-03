#pragma once
#include "EunoiaUI/Draw/UIGraphics.h"
#include <d3d12.h>
#include <wrl/client.h>
#include <vector>

namespace EunoiaUI {

using Microsoft::WRL::ComPtr;

class UID3D12Backend : public UIGraphics {
public:
    UID3D12Backend() = default;
    ~UID3D12Backend() override { Destroy(); }

    bool Initialize(ID3D12Device* device, DXGI_FORMAT rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM);
    void Destroy() override;

    void Render(const UIDrawData& data, void* stream = nullptr) override;

    unsigned long long CreateImage(const unsigned char* pixels, int width, int height) override;
    void UpdateImage(unsigned long long image, const unsigned char* pixels) override;
    unsigned long long AdoptImage(void* nativeResource) override;
    void DestroyImage(unsigned long long image) override;

private:
    bool m_initialized = false;
    ID3D12Device* m_device = nullptr;
    DXGI_FORMAT m_rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

    ComPtr<ID3D12RootSignature>  m_rootSignature;
    ComPtr<ID3D12PipelineState>  m_pipelineState;
    ComPtr<ID3D12Resource>       m_constantBuffer;
    ComPtr<ID3D12Resource>       m_fontTexture;
    ComPtr<ID3D12DescriptorHeap> m_srvHeap;

    ComPtr<ID3D12Resource>       m_instanceBuffer;
    void*                        m_mappedInstances = nullptr;
    size_t                       m_instanceCapacity = 16384;
};

} // namespace EunoiaUI
