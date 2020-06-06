#include "RaidStream.hpp"
#include "RaidFileStripe.hpp"

namespace RaidStream {
    RaidFileStripe::RaidFileStripe(const RaidStripeType type, std::vector<RaidFileBlock> blocks) : _type{type} {
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
