#ifndef RAIDSTREAM_RAIDSTREAM_HPP
#define RAIDSTREAM_RAIDSTREAM_HPP

#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <filesystem>
#include <cassert>
#include "RaidStream/RaidConfiguration.hpp"

namespace RaidStream {
    class RaidStream {
    public:
        enum RaidStreamStatus {
            CLOSED,
            OPENING_UNVERIFIED,
            OPENING_UNVERIFIED_VERIFYING,
            ERROR,
            DEGRADED,
            DEGRADED_REBUILDING,
            CONSISTENT, // ONLINE, Good health
            VERIFYING,  // ONLINE, but checking consistency (RO)
            SCRUBBING,  // ONLINE, fixing detected consistency errors - (minor errors, not full disk)
            CLOSING,
            OFFLINE,
            OFFLINE_DEGRADED,
            OFFLINE_REBUILDING
        };

        RaidStream(std::shared_ptr<RaidConfiguration> configuration, bool existingArray, bool allowInitializeArray = false);
        ~RaidStream();
//        void open(std::ios_base::openmode __mode = std::ios_base::in | std::ios_base::out | std::ios_base::binary);


        inline bool Online();

        inline bool Opening();

        inline bool Closing();

        inline bool Closed();

        inline bool Error();

//        void Close();

        static std::shared_ptr<RaidConfiguration> MakeConfiguration(RaidConfiguration::RaidType type, std::vector<RaidFile> files,
                                                                    std::ostream *os = nullptr,
                                                                    std::ostream *oe = nullptr);

        inline bool Degraded();

        inline bool Rebuilding();

        inline bool Consistent();

        inline bool Scrubbing();

        inline void Rebuild(bool forceRebuild = false);

        friend std::ostream &operator<<(std::ostream &os, const RaidStream &raidstream);

        inline bool InStatus(std::vector<RaidStreamStatus> statusList);

        inline void Sync();

        inline void Seek(uintmax_t offset, bool seekSpares = false);

        inline void ForceWrite(uintmax_t offset, char data);

        inline const unsigned char CalculateKnownData(RaidFile* file, uintmax_t offset, const unsigned char data, bool *spare = nullptr);

        inline std::vector<unsigned char> readAll(uintmax_t offset, bool readSpares = false);

        const std::string StatusString();

        const RaidStreamStatus Status();

        bool Open(std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out | std::ios_base::binary | std::ios_base::ate);

    protected:
        inline bool needFlush();
        inline void flush();

        RaidStreamStatus _status = RaidStreamStatus::OPENING_UNVERIFIED;
        bool _openExistingArray = false;
        bool _openAllowInitialize = false;
        uintmax_t _currentOffset = 0;
        uintmax_t _normalizedDeviceSize = 0;
        uintmax_t _largestDeviceSize = 0;
        std::shared_ptr<RaidConfiguration> _configuration;
//        std::streambuf _uncommitted;
    };
}

#endif //RAIDSTREAM_RAIDSTREAM_HPP
