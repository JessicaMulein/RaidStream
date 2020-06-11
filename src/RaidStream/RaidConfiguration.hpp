#ifndef RAIDSTREAM_RAIDCONFIGURATION_HPP
#define RAIDSTREAM_RAIDCONFIGURATION_HPP

#include <nlohmann/json.hpp>
#include <vector>
#include <sole/sole.hpp>
#include "RaidStream/RaidFile.hpp"

// for convenience
using json = nlohmann::json;

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
#define RaidTypeEnumMap { \
        {JBOD, "JBOD"}, \
        {MIRROR, "Mirror"}, \
        {RAID5, "RAID-5"}, \
        {RAID6, "RAID-6"}, \
        {EXPERIMENTAL, "Experimental"}, \
    }

        const std::map<RaidType, const std::string> RaidTypeDescriptions = RaidTypeEnumMap;
        NLOHMANN_JSON_SERIALIZE_ENUM( RaidType, RaidTypeEnumMap);

        RaidConfiguration(json configJson, std::ostream *os = nullptr, std::ostream *oe = nullptr);

        RaidConfiguration(RaidType type, std::vector<RaidFile> files, std::ostream *os = nullptr,
                          std::ostream *oe = nullptr);

        std::vector<RaidFile>& Files();

        std::pair<unsigned int, unsigned int> logStats();

        void log(std::string data);

        void warn(std::string warning);

        void error(std::string error);

        void setStdOut(std::ostream *fs);

        void setStdErr(std::ostream *fs);

        sole::uuid UUID();

        RaidType Type();

        const std::string TypeString();

        const unsigned long LogCount();

        const unsigned long WarningCount();

        const unsigned long ErrorCount();

        static std::string BytesToSize(uintmax_t bytes);

        void to_json(json &j, const RaidConfiguration &config);

    protected:
        sole::uuid _uuid = sole::uuid4();
        RaidType _type;
        std::vector<RaidFile> _files;
        std::ostream *_os = nullptr;
        std::ostream *_oe = nullptr;
        unsigned long _logCount = 0;
        unsigned long _warningCount = 0;
        unsigned long _errorCount = 0;
        unsigned short _filesData = 0;
        unsigned short _filesMirror = 0;
        unsigned short _filesXor = 0;
        unsigned short _filesReedSolomon = 0;
        unsigned short _filesExperimental = 0;
        uintmax_t _bytesData = 0;
        uintmax_t _bytesMirror = 0;
        uintmax_t _bytesXor = 0;
        uintmax_t _bytesReedSolomon = 0;
        uintmax_t _bytesExperimental = 0;
        uintmax_t _bytesTotal = 0;
        uintmax_t _bytesActualTotal = 0;
    };
}

#endif //RAIDSTREAM_RAIDCONFIGURATION_HPP
