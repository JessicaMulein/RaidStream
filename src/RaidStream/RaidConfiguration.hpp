#ifndef RAIDSTREAM_RAIDCONFIGURATION_HPP
#define RAIDSTREAM_RAIDCONFIGURATION_HPP

#include <vector>
#include <sole/sole.hpp>
#include "RaidStream/RaidStream.hpp"
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

        std::vector<RaidFile> Files();

        std::pair<unsigned int, unsigned int> logStats();

        void log(std::string data);

        void warn(std::string warning);

        void setStdOut(std::fstream *fs);

        void setStdErr(std::fstream *fs);

        sole::uuid UUID();

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
