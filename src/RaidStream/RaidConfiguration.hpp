#ifndef RAIDSTREAM_RAIDCONFIGURATION_HPP
#define RAIDSTREAM_RAIDCONFIGURATION_HPP

#include <vector>
#include <sole/sole.hpp>
#include "RaidStream/RaidFile.hpp"

namespace RaidStream {
    class RaidConfiguration {
    public:
        enum RaidType {
            JBOD,
            MIRROR,
            RAID5,
            RAID6,
            EXPERIMENTAL
        };

        RaidConfiguration(RaidType type, std::vector<RaidFile> files, std::fstream *stdout = nullptr,
                          std::fstream *stderr = nullptr);

        inline std::vector<RaidFile> Files();

        inline std::pair<unsigned int, unsigned int> logStats();

        inline void log(std::string data);

        inline void warn(std::string warning);

        inline void setStdOut(std::fstream *fs);

        inline void setStdErr(std::fstream *fs);

        inline sole::uuid UUID();

    protected:
        const sole::uuid _uuid = sole::uuid4();
        RaidType _type;
        std::vector<RaidFile> _files;
        std::fstream *_stdout = nullptr;
        std::fstream *_stderr = nullptr;
        unsigned long _logCount = 0;
        unsigned long _warningCount = 0;
        unsigned short _filesData = 0;
        unsigned short _filesXor = 0;
        unsigned short _filesReedSolomon = 0;
        unsigned short _filesExperimental = 0;
    };
}

#endif //RAIDSTREAM_RAIDCONFIGURATION_HPP
