#include "EunoiaUI/Core/UIArena.h"
#include <algorithm>

namespace EunoiaUI {

bool UIArena::Create(size_t initialCapacity) {
    Destroy();
    UIArenaBlock block;
    block.Bytes = std::make_unique<unsigned char[]>(initialCapacity);
    block.Capacity = initialCapacity;
    m_blocks.push_back(std::move(block));
    m_currentBlock = 0;
    m_offset = 0;
    return true;
}

void UIArena::Destroy() {
    m_blocks.clear();
    m_currentBlock = 0;
    m_offset = 0;
}

void* UIArena::Allocate(size_t size) {
    if (m_blocks.empty()) {
        if (!Create(1048576)) return nullptr;
    }

    // 8-byte alignment
    size = (size + 7) & ~7;

    if (m_currentBlock < m_blocks.size()) {
        auto& current = m_blocks[m_currentBlock];
        if (m_offset + size <= current.Capacity) {
            void* ptr = current.Bytes.get() + m_offset;
            m_offset += size;
            return ptr;
        }
    }

    // Allocate next block
    size_t nextCapacity = std::max(size, (size_t)1048576);
    UIArenaBlock block;
    block.Bytes = std::make_unique<unsigned char[]>(nextCapacity);
    block.Capacity = nextCapacity;
    m_blocks.push_back(std::move(block));
    m_currentBlock = m_blocks.size() - 1;
    m_offset = size;
    return m_blocks[m_currentBlock].Bytes.get();
}

void UIArena::Reset() {
    m_currentBlock = 0;
    m_offset = 0;
}

} // namespace EunoiaUI
