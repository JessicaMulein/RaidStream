#include "RaidSuperBlock.hpp"

namespace RaidStream {
    RaidSuperBlock::RaidSuperBlock(uintmax_t nativeOffset) : RaidFileBlock(nativeOffset) {

    }

    bool RaidSuperBlock::AllocateMemory() {
        if (_bytes != nullptr) return false;
        _bytes = new block_data_type[BLOCK_SIZE];
        // fill new memory with guards
        std::memset(_bytes, 0xCC, BLOCK_SIZE);
        // set last byte to 00
        _bytes[BLOCK_SIZE-1] = '\0';
        return true;
    }
}