#ifndef RAIDSTREAM_ALGORITHMEXPERIMENTALTRIPLEPARITY_HPP
#define RAIDSTREAM_ALGORITHMEXPERIMENTALTRIPLEPARITY_HPP

#include "RaidStream/algorithms/Algorithm.hpp"
namespace RaidStream {
    namespace Algorithms {

        /**
         * This is the experimental algorithm. Combo of using RS and/or different RSFEC algo and/or XOR
         * This class will map reads/writes to the appropriate volume/block and handle triggering corrections
         */
        class AlgorithmExperimentalTripleParity : Algorithm {

        };
    }
}

#endif //RAIDSTREAM_ALGORITHMEXPERIMENTALTRIPLEPARITY_HPP
