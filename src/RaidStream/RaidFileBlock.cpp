#include "RaidStream/RaidStream.hpp"
#include "RaidStream/RaidFileBlock.hpp"


namespace RaidStream {
    RaidFileBlock::RaidFileBlock(uintmax_t nativeOffset) :
            _blockID{static_cast<RaidFileBlock::block_pos_t>(nativeOffset / BLOCK_SIZE)} {
        if ((nativeOffset % BLOCK_SIZE) != 0) {
            throw std::invalid_argument("nativeOffset must be at multiple of block size");
        }
    }

    bool RaidFileBlock::InMemory() {
        return (_bytes != nullptr);
    }

    bool RaidFileBlock::NeedsSync() {
        return _uncommittedWrites;
    }

    bool RaidFileBlock::ReleaseMemory(bool force) {
        if (!force && NeedsSync()) return false;
        if (_bytes == nullptr) return true;
        delete _bytes;
        _bytes = nullptr;
        return true;
    }

    bool RaidFileBlock::ReadBlockFromDisk(bool force) {
        if (!force && (InMemory() || NeedsSync())) return false;
        AllocateMemory(); // will return false if it was already allocated
        // TODO: read from disk
        return false;

        _uncommittedWrites = false;
        return true;
    }

    bool RaidFileBlock::WriteBlockToDisk(bool force, bool keepMemory) {
        if (!force && !NeedsSync()) return false;
        // TODO: write to disk
        _uncommittedWrites = false;
        if (!keepMemory) {
            ReleaseMemory();
        }
    }

    const RaidFileBlock::block_size_t RaidFileBlock::BlockSize() {
        return BLOCK_SIZE;
    }

    const RaidFileBlock::block_size_t RaidFileBlock::BlockID() {
        return _blockID;
    }

    RaidFileBlock::block_data_type RaidFileBlock::at(block_size_t offset) {
        if ((offset < 0) || (offset >= BLOCK_SIZE)) {
            throw std::invalid_argument("Invalid offset out of range");
        }
        return Bytes()[offset];
    }

    /** Writes data to memory, forces full block sync if immediateSync, bool to keep data in memory after write **/
    bool
    RaidFileBlock::Set(block_size_t blockOffset, const block_data_type *data, size_t len, bool immediateSync,
                       bool keepInMemory) {
        size_t data_it = 0;
        for (size_t it = blockOffset; (it < BLOCK_SIZE) && (data_it < len); it++) {
            _bytes[it] = data[data_it++];
        }
        if (immediateSync) {
            return WriteBlockToDisk(true, keepInMemory);
        }
        return false;
    }

    RaidFileBlock::block_data_type *RaidFileBlock::Bytes() {
        if (!InMemory()) {
            if (!ReadBlockFromDisk()) {
                throw std::invalid_argument("Unable to read disk"); // TODO: error types
            }
        }
        return _bytes;
    }

    RaidFileBlock::block_data_type *RaidFileBlock::BytesCopy() {
        block_data_type *ourBytes = Bytes();
        block_data_type *newCopy = new block_data_type[BLOCK_SIZE];
        memcpy(newCopy, ourBytes, BLOCK_SIZE);
        return newCopy;
    }

    CRC32::crc RaidFileBlock::crc32() {
        CRC32::crcInit();
        return CRC32::crcFast(Bytes(), BLOCK_SIZE);
    }

    CRC64::crc RaidFileBlock::crc64() {
        return CRC64::crc64(0, Bytes(), BLOCK_SIZE);
    }

    bool RaidFileBlock::UpdateCRC32() {
        CRC32::crc oldCRC = _lastCRC32;
        _lastCRC32 = crc32();
        return (oldCRC == _lastCRC32);
    }

    bool RaidFileBlock::UpdateCRC64() {
        CRC64::crc oldCRC = _lastCRC64;
        _lastCRC64 = crc64();
        return (oldCRC == _lastCRC64);
    }

    RaidFileBlock::block_data_type *RaidFileBlock::XorBlock(RaidFileBlock block) {
        block_data_type *ourBytes = Bytes();
        block_data_type *theirBytes = block.Bytes();
        block_data_type *xorBuf = new block_data_type[BLOCK_SIZE];
        for (block_size_t it = 0; it < BLOCK_SIZE; ++it) {
            xorBuf[it] = ourBytes[it] ^ theirBytes[it];
        }
        return xorBuf;
    }

    RaidFileBlock::block_data_type *RaidFileBlock::XorBlock(std::vector<RaidFileBlock> blocks) {
        block_data_type *xorBuf = BytesCopy();
        for (std::vector<RaidFileBlock>::iterator it = blocks.begin(); it != blocks.end(); ++it) {
            block_data_type *theirBytes = it->Bytes();
            for (block_size_t it = 0; it < BLOCK_SIZE; ++it) {
                xorBuf[it] = xorBuf[it] ^ theirBytes[it];
            }
        }
        return xorBuf;
    }

    bool RaidFileBlock::SwapBlockBytes(block_data_type *newBytes, bool forceRelease) {
        if (InMemory()) {
            if (!ReleaseMemory(forceRelease)) {
                return false;
            }
        }
        _bytes = newBytes;
        _uncommittedWrites = true;
        return true;
    }

    const sole::uuid RaidFileBlock::UUID() {
        return _uuid;
    }

    bool RaidFileBlock::AllocateMemory() {
        if (_bytes != nullptr) return false;
        _bytes = new block_data_type[BLOCK_SIZE];
        return true;
    }
}
