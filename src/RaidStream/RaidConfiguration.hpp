#ifndef RAIDSTREAM_RAIDCONFIGURATION_HPP
#define RAIDSTREAM_RAIDCONFIGURATION_HPP

#include <vector>
#include <sole/sole.hpp>
#include "RaidStream/RaidFile.hpp"

namespace RaidStream {
    class RaidConfiguration {
    public:
        enum RaidType {
            JBOD,        // 0 raid parity algorithms: 0% overhead, 0% redundancy
            MIRROR,      // 0 raid parity algorithms: 50% overhead, 50% redundancy, requires N % 2 = 0
            RAID5,       // 1 raid parity algorithm, XOR:
            RAID6,       // 2 raid parity algorithms, XOR, RS
            EXPERIMENTAL // 3 raid parity algorithms XOR, RS, XOR+RS(?)
        };

        RaidConfiguration(RaidType type, std::vector<RaidFile> files, std::ostream *os = nullptr,
                          std::ostream *oe = nullptr);

        std::vector<RaidFile> Files();

        std::pair<unsigned int, unsigned int> logStats();

        void log(std::string data);

        void warn(std::string warning);

        void setStdOut(std::ostream *fs);

        void setStdErr(std::ostream *fs);

        sole::uuid UUID();

        RaidType Type();

    protected:
        const sole::uuid _uuid = sole::uuid4();
        RaidType _type;
        std::vector<RaidFile> _files;
        std::ostream *_os = nullptr;
        std::ostream *_oe = nullptr;
        unsigned long _logCount = 0;
        unsigned long _warningCount = 0;
        unsigned short _filesData = 0;
        unsigned short _filesMirror = 0;
        unsigned short _filesXor = 0;
        unsigned short _filesReedSolomon = 0;
        unsigned short _filesExperimental = 0;
        uintmax_t _expectedAvailableDataBytes = 0;
        uintmax_t _expectedMirrorBytes = 0;
        uintmax_t _expectedXorBytes = 0;
        uintmax_t _expectedReedSolomonBytes = 0;
        uintmax_t _expectedExperimentalBytes = 0;
        uintmax_t _expectedTotalBytes = 0;
    };
}

#endif //RAIDSTREAM_RAIDCONFIGURATION_HPP
