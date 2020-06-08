#ifndef RAIDSTREAM_ALGORITHMJBOD_HPP
#define RAIDSTREAM_ALGORITHMJBOD_HPP

#include "RaidStream/algorithms/Algorithm.hpp"

namespace RaidStream {
    namespace Algorithms {

        /**
         * This algorithm just concatenates all the available disks into a long stream and maps reads
         * This class will map reads/writes to the appropriate volume/block and handle triggering corrections
        */
        class AlgorithmJBOD : Algorithm {

        };
    }
}


#endif //RAIDSTREAM_ALGORITHMJBOD_HPP
