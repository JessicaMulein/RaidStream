#ifndef RAIDSTREAM_RAIDFILEBLOCK_HPP
#define RAIDSTREAM_RAIDFILEBLOCK_HPP

#define USE_1024 false // Make sure this aligns with your disk!
#define UNITS_KB (USE_1024 ? 1024 : 1000)
#define UNITS_MB (UNITS_KB * UNITS_KB)
#define UNITS_GB (UNITS_MB * UNITS_KB)
#define UNITS_TB (UNITS_GB * UNITS_KB)
#define UNITS_PB (UNITS_TB * UNITS_KB)


#include <exception>
#include <sole/sole.hpp>
#include "RaidStream/RaidStream.hpp"
#include "RaidStream/RaidFile.hpp"
#include "RaidStream/crc64/crc64.hpp"
#include "libschifra/include/schifra_crc.hpp"

namespace RaidStream {
    class RaidFileBlock {
    public:
        typedef uint32_t block_size_t;
        typedef uint16_t block_pos_t;
        typedef unsigned char block_data_type;

        static const block_size_t BLOCK_SIZE = (4 * UNITS_MB); // Make sure this aligns with your disk!
        RaidFileBlock(uintmax_t nativeOffset);

        bool InMemory();

        bool NeedsSync();

        bool ReleaseMemory(bool force = false);

        bool ReadBlockFromDisk(bool force = false);

        bool WriteBlockToDisk(bool force = false, bool keepMemory = true);

        const block_size_t BlockSize();

        const block_size_t BlockID();

        block_data_type at(block_size_t offset);

        /** Writes data to memory, forces full block sync if immediateSync, bool to keep data in memory after write **/
        bool
        Set(block_size_t blockOffset, const block_data_type *data, size_t len, bool immediateSync = false,
            bool keepInMemory = true);

        RaidFileBlock::block_data_type *Bytes();

        RaidFileBlock::block_data_type *BytesCopy();

        schifra::crc32::crc32_t crc32();

        CRC64::crc crc64();

        bool UpdateCRC32();

        bool UpdateCRC64();

        block_data_type* XorBlock(RaidFileBlock block);

        block_data_type* XorBlock(std::vector<RaidFileBlock> blocks);

        bool SwapBlockBytes(block_data_type* newBytes, bool forceRelease = false);

        const sole::uuid UUID();

    protected:
        bool AllocateMemory();

        const sole::uuid _uuid = sole::uuid4();
        bool _uncommittedWrites = false;
        const RaidFileBlock::block_pos_t _blockID;
        schifra::crc32::crc32_t _lastCRC32 = 0;
        CRC64::crc _lastCRC64 = 0;
        /** @var _inMem bool Whether the block is loaded into memory or is only on disk **/
        RaidFileBlock::block_data_type *_bytes = nullptr;
    };
}


#endif //RAIDSTREAM_RAIDFILEBLOCK_HPP
