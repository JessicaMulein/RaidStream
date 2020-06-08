#ifndef RAIDSTREAM_ALGORITHMXOR_HPP
#define RAIDSTREAM_ALGORITHMXOR_HPP

#include "RaidStream/algorithms/Algorithm.hpp"

namespace RaidStream {
    namespace Algorithms {

    /**
     * Make use of block->XorBlock() function
     * This class will map reads/writes to the appropriate volume/block and handle triggering corrections
     */
        class AlgorithmXor : Algorithm {

        };
    }
}

#endif //RAIDSTREAM_ALGORITHMXOR_HPP
