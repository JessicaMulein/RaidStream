#include "RaidStream/RaidFileStripe.hpp"

namespace RaidStream {
    RaidFileStripe::RaidFileStripe(std::shared_ptr<RaidFile> raidFile, const RaidStripeType type, std::vector<RaidFileBlock> blocks) :
        _raidFile{raidFile},
        _type{type}
    {
        bool setBlockSize = true;
        for (std::vector<RaidFileBlock>::iterator it = blocks.begin(); it != blocks.end(); ++it) {
            if (setBlockSize) {
                _blockSize = it->BlockSize();
                setBlockSize = false;
            } else if (it->BlockSize() != _blockSize) {
                throw new std::invalid_argument("Block sizes must match throughout stripe");
            }
            _stripeBlocks.insert(std::make_pair(it->BlockID(), *it));
//                if (it->)
        }
    }

    const sole::uuid RaidFileStripe::UUID() {
        return _uuid;
    }
}
