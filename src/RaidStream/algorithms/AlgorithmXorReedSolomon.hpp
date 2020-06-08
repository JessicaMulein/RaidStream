#ifndef RAIDSTREAM_ALGORITHMXORREEDSOLOMON_HPP
#define RAIDSTREAM_ALGORITHMXORREEDSOLOMON_HPP

#include "RaidStream/algorithms/Algorithm.hpp"

namespace RaidStream {
    namespace Algorithms {

        /**
        * Algorithm ensures reads/writes from a mirror disk in the array can be read and corrected
        * This class will map reads/writes to the appropriate volume/block and handle triggering corrections
        */
        class AlgorithmXorReedSolomon : Algorithm {

        };
    }
}


#endif //RAIDSTREAM_ALGORITHMXORREEDSOLOMON_HPP
