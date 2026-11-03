#pragma once
#include <memory>
#include <vector>
#include <cstddef>

namespace EunoiaUI {

class UIArenaBlock {
public:
    std::unique_ptr<unsigned char[]> Bytes;
    size_t Capacity = 0;
};

class UIArena {
public:
    bool Create(size_t initialCapacity = 1048576);
    void Destroy();

    void* Allocate(size_t size);
    void Reset();

    // Helper template for typed temporary allocations
    template<typename T, typename... Args>
    T* AllocateObject(Args&&... args) {
        void* mem = Allocate(sizeof(T));
        if (!mem) return nullptr;
        return new (mem) T(std::forward<Args>(args)...);
    }

private:
    std::vector<UIArenaBlock> m_blocks;
    size_t m_currentBlock = 0;
    size_t m_offset = 0;
};

inline auto Arena = std::make_unique<UIArena>();

} // namespace EunoiaUI
