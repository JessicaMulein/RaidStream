#ifndef RAIDSTREAM_RAIDFILE_HPP
#define RAIDSTREAM_RAIDFILE_HPP

#include <string>
#include <fstream>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <sole/sole.hpp>
#include "RaidStream/RaidFileBlock.hpp"
#include "RaidStream/RaidSuperBlock.hpp"
//#include "RaidStream/RaidStripeGroup.hpp"

using json = nlohmann::json;

namespace RaidStream {
    class RaidConfiguration;

    class RaidFile {
    public:
        typedef std::map<RaidFileBlock::block_pos_t, RaidFileBlock> raid_block_map;
//        typedef std::map<RaidFileBlock::block_pos_t, RaidStripeGroup> raid_stripegroup_map;
        typedef std::map<RaidFileBlock::block_pos_t, RaidSuperBlock> raid_superblock_map;

        enum FileStatus {
            NEW,
            UNVERIFIED,
            CONSISTENT,
            DEGRADED,
            DEGRADED_REBUILDING,
            OFFLINE,
            OFFLINE_REBUILDING
        };
#define FileStatusEnumMap { \
        {NEW, "New"}, \
        {UNVERIFIED, "Unverified"}, \
        {CONSISTENT, "Online - Consistent"}, \
        {DEGRADED, "Online - Degraded"}, \
        {DEGRADED_REBUILDING, "Degraded - Rebuilding"}, \
        {OFFLINE, "Offline"}, \
        {OFFLINE_REBUILDING, "Offline - Rebuilding"}, \
    }
        const std::map<FileStatus, const std::string> FileStatusDescriptions = FileStatusEnumMap;
        NLOHMANN_JSON_SERIALIZE_ENUM( FileStatus, FileStatusEnumMap);

        enum FileType {
            DATA,
            PARITY_MIRROR, // 1:1, multiples of 2
            PARITY_XOR,    // theoretically can have a 2 disk system with one disk being a xor reciprocal
            PARITY_RS,
            PARITY_EXPERIMENTAL,
            SPARE
        };
#define FileTypeEnumMap { \
        {DATA, "Data"}, \
        {PARITY_MIRROR, "Mirror"}, \
        {PARITY_XOR, "Parity - XOR"}, \
        {PARITY_RS, "Parity - Reed Solomon"}, \
        {PARITY_EXPERIMENTAL, "Parity - Experimental"}, \
        {SPARE, "Spare"}, \
    }
        const std::map<FileType, const std::string> FileTypeDescriptions = FileTypeEnumMap;
        NLOHMANN_JSON_SERIALIZE_ENUM( FileType, FileTypeEnumMap);

        RaidFile(const char* filename,
                        FileType type,
                        uintmax_t sizeOnDisk = 0,
                        std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out | std::ios_base::ate | std::ios_base::binary
        );
        RaidFile(RaidConfiguration *config, json configJson);
        RaidFile(const RaidFile&) = delete;
        RaidFile(RaidFile&&) = default;

        const FileType Type() const;

        const std::string TypeString();

        const std::string FileName() const;

        uintmax_t SparseSize();

        uintmax_t Size();

        void Size(uintmax_t size);

        uintmax_t SizeOnDisk(std::error_code &ec);

        uint64_t VirtualSize();

        void VirtualSize(uint64_t size);

        const FileStatus Status();

        const std::string StatuString();

        bool Consistent();

        bool Rebuilding();

        std::ios_base::openmode Mode();

        void FlushBlock(RaidFileBlock::block_pos_t id, bool force = false, bool keepMemory = true);

        const sole::uuid UUID();
        
        //std::shared_ptr<RaidConfiguration> Configuration();

        uintmax_t DiskSpaceAvailable(std::error_code &ec);

        inline bool SufficientSpaceForCreate(std::error_code &ec, uintmax_t withClearance = 0);

        static void to_json(json &j, const RaidFile &file);

    protected:
        friend class RaidConfiguration;
        friend class RaidStream;
        void mode(std::ios_base::openmode mode);
        void setConfiguration(RaidConfiguration* configuration);
        bool OpenOnly(std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out | std::ios_base::binary | std::ios_base::ate);
        bool OpenOrCreate(std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out | std::ios_base::binary | std::ios_base::ate);
        bool Create(std::ios::openmode mode = std::ios_base::in | std::ios_base::out | std::ios_base::binary | std::ios_base::ate);
        RaidConfiguration* _configuration = nullptr;
        sole::uuid _uuid = sole::uuid4();
        std::string _fileName;
        FileType _type;
        std::fstream* _fileStream;
        uintmax_t _virtualSize = 0; // may/will be < actual size
        uintmax_t _actualSize = 0;  // size on disk
        FileStatus _fileStatus = FileStatus::NEW;
        RaidFileBlock::block_pos_t _numBlocks;
        RaidFileBlock::block_pos_t _numBlocksVirtual;
        raid_block_map _loadedBlocks;
        std::ios_base::openmode _fileMode;
    };
}

#endif //RAIDSTREAM_RAIDFILE_HPP
