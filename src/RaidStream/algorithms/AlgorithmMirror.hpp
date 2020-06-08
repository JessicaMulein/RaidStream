#ifndef RAIDSTREAM_ALGORITHMMIRROR_HPP
#define RAIDSTREAM_ALGORITHMMIRROR_HPP

#include "RaidStream/algorithms/Algorithm.hpp"

namespace RaidStream {
    namespace Algorithms {

        /**
         * Algorithm ensures reads/writes from a mirror disk in the array can be read and corrected
         * This class will map reads/writes to the appropriate volume/block and handle triggering corrections
         */
        class AlgorithmMirror : Algorithm {

        };
    }
}


#endif //RAIDSTREAM_ALGORITHMMIRROR_HPP
