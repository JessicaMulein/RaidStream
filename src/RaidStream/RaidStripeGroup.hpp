#ifndef RAIDSTREAM_RAIDSTRIPEGROUP_HPP
#define RAIDSTREAM_RAIDSTRIPEGROUP_HPP

#include "RaidStream/RaidStream.hpp"
#include "RaidStream/RaidFileStripe.hpp"

namespace RaidStream {
    class RaidStripeGroup {
    public:
    protected:
        RaidFile::raid_stripegroup_map _stripeBlocks;
    };
}
#endif //RAIDSTREAM_RAIDSTRIPEGROUP_HPP
