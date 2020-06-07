#include "RaidStream/RaidStream.hpp"
#include "RaidStream/RaidFile.hpp"

namespace RaidStream {
    RaidFile::RaidFile(const char* filename, FileType type, uintmax_t sizeOnDisk, std::ios_base::openmode mode) :
            _fileName{std::string(filename)},
            _fileType{type},
            _actualSize{sizeOnDisk},
            _fileMode{mode},
            _fileStream{new std::ofstream(filename, mode)}
        {
            _configuration = nullptr;
        }

    const RaidFile::FileType RaidFile::Type() const {
        return _fileType;
    }

    const std::string RaidFile::FileName() const {
        return _fileName;
    }

    uintmax_t RaidFile::SparseSize() {
        // TODO: get the actual size of the file on disk, taking into account sparsefile compression
        return _actualSize;
    }

    uintmax_t RaidFile::Size() {
        std::error_code ec;
        uintmax_t fileSize = std::filesystem::file_size(FileName(), ec);
        if (ec) {

        } else {

        }
        return fileSize;
    }

    void RaidFile::Size(uintmax_t size) {
        _actualSize = size;
    }

    uint64_t RaidFile::VirtualSize() {
        return _virtualSize;
    }

    void RaidFile::VirtualSize(uint64_t size) {
        if (size > _actualSize) {
            throw std::invalid_argument("Virtual Size must be <= Size");
        }
        _virtualSize = size;
    }

    RaidFile::FileStatus RaidFile::Status() {
        return _fileStatus;
    };

    bool RaidFile::Consistent() {
        return (_fileStatus == FileStatus::CONSISTENT);
    }

    bool RaidFile::Rebuilding() {
        return (
            (_fileStatus == FileStatus::DEGRADED_REBUILDING) ||
            (_fileStatus == FileStatus::OFFLINE_REBUILDING)
        );
    }

    unsigned int RaidFile::Mode() {
        return _fileMode;
    }

    void RaidFile::Mode(unsigned int mode) {
        _fileMode = mode;
    }

    void RaidFile::FlushBlock(RaidFileBlock::block_pos_t id, bool force, bool keepMemory) {
        RaidFile::raid_block_map::iterator it = _loadedBlocks.find(id);
        if (it == _loadedBlocks.end()) {
            return;
        }
        if (!keepMemory) {
            it->second.ReleaseMemory(force);
            _loadedBlocks.erase(it);
        }
    }

    const sole::uuid RaidFile::UUID() {
        return _uuid;
    }

    void RaidFile::Configuration(std::shared_ptr<RaidConfiguration> configuration) {
        if (_configuration != nullptr) throw std::invalid_argument("Configuration already set");
        _configuration = configuration;
    }

    std::shared_ptr<RaidConfiguration> RaidFile::Configuration() {
        return _configuration;
    }
}
