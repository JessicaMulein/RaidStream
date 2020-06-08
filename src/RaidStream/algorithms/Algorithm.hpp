#ifndef RAIDSTREAM_ALGORITHM_HPP
#define RAIDSTREAM_ALGORITHM_HPP

#include <cstdint>
#include "RaidStream/RaidConfiguration.hpp"

namespace RaidStream {
    namespace Algorithms {
        class Algorithm {
        public:
            Algorithm(std::shared_ptr<RaidStream::RaidConfiguration> configuration);

            virtual bool Write(uintmax_t offset, const char *data, unsigned int n, bool correctIfWrong = true) = 0;

            virtual const char* Read(uintmax_t offset, const char *data, unsigned int n, bool correctIfWrong = true) = 0;
        protected:
            std::shared_ptr<RaidStream::RaidConfiguration> _configuration;
        };
    }
}



#endif //RAIDSTREAM_ALGORITHM_HPP
