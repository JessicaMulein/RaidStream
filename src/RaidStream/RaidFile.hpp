#ifndef RAIDSTREAM_RAIDFILE_HPP
#define RAIDSTREAM_RAIDFILE_HPP

#include <string>
#include <fstream>
#include <map>
#include <memory>
#include <sole/sole.hpp>
#include "RaidStream/RaidFileBlock.hpp"
#include "RaidStream/RaidSuperBlock.hpp"
//#include "RaidStream/RaidStripeGroup.hpp"

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

        enum FileType {
            DATA,
            PARITY_MIRROR, // 1:1, multiples of 2
            PARITY_XOR,    // theoretically can have a 2 disk system with one disk being a xor reciprocal
            PARITY_RS,
            PARITY_EXPERIMENTAL,
            SPARE
        };

        RaidFile(const char* filename,
                        FileType type,
                        uintmax_t sizeOnDisk = 0,
                        std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out | std::ios_base::ate | std::ios_base::binary
        );

        const FileType Type() const;

        const std::string TypeString();

        const std::string FileName() const;

        uintmax_t SparseSize();

        uintmax_t Size();

        void Size(uintmax_t size);

        uint64_t VirtualSize();

        void VirtualSize(uint64_t size);

        FileStatus Status();

        bool Consistent();

        bool Rebuilding();

        unsigned int Mode();

        void Mode(unsigned int mode);

        void FlushBlock(RaidFileBlock::block_pos_t id, bool force = false, bool keepMemory = true);

        const sole::uuid UUID();
        
        //std::shared_ptr<RaidConfiguration> Configuration();

    protected:
        friend class RaidConfiguration;
        void _setConfiguration(RaidConfiguration* configuration);
        RaidConfiguration* _configuration = nullptr;
        const sole::uuid _uuid = sole::uuid4();
        const std::string _fileName;
        const FileType _type;
        const std::ofstream* _fileStream;
        uintmax_t _virtualSize = 0; // may/will be < actual size
        uintmax_t _actualSize = 0;  // size on disk
        FileStatus _fileStatus = FileStatus::NEW;
        RaidFileBlock::block_pos_t _numBlocks;
        RaidFileBlock::block_pos_t _numBlocksVirtual;
        raid_block_map _loadedBlocks;
        unsigned int _fileMode;
    };
}

#endif //RAIDSTREAM_RAIDFILE_HPP
