#ifndef RAIDSTREAM_RAIDSUPERBLOCK_HPP
#define RAIDSTREAM_RAIDSUPERBLOCK_HPP

#include "RaidStream/RaidFileBlock.hpp"
#include "RaidStream/RaidStream.hpp"

namespace RaidStream {
    class RaidSuperBlock : RaidFileBlock {
        RaidSuperBlock(uintmax_t nativeOffset);

        bool AllocateMemory();
    };
}

#endif //RAIDSTREAM_RAIDSUPERBLOCK_HPP
