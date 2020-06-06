#ifndef RAIDSTREAM_RAIDSUPERSTRIPE_HPP
#define RAIDSTREAM_RAIDSUPERSTRIPE_HPP

#include "RaidStream/RaidFileStripe.hpp"
#include "RaidStream/RaidStream.hpp"

namespace RaidStream {
    class RaidSuperStripe : RaidFileStripe {
    public:
    protected:
        const RaidStripeType _type = RaidStripeType::SUPER;
        RaidFile::raid_superblock_map _stripeBlocks;
    };
}

#endif //RAIDSTREAM_RAIDSUPERSTRIPE_HPP
