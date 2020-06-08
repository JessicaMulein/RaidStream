#include "RaidStream/RaidFile.hpp"
#include "RaidStream/RaidConfiguration.hpp"
#include <filesystem>

namespace RaidStream {
    RaidFile::RaidFile(const char* filename, FileType type, uintmax_t sizeOnDisk,
            std::ios_base::openmode mode) :
            _fileName{std::string(filename)},
            _type{type},
            _actualSize{sizeOnDisk},
            _fileMode{mode},
            _fileStream{nullptr}
        {}

    const RaidFile::FileType RaidFile::Type() const {
        return _type;
    }

    const std::string RaidFile::TypeString() {
        switch (this->_type) {
            case DATA:
                return "Data";
            case PARITY_MIRROR:      // 0 raid parity algorithms: 50% overhead, 50% redundancy, requires N % 2 = 0
                return "Data Mirror";
            case PARITY_XOR:       // 1 raid parity algorithm, XOR:
                return "Xor Parity";
            case PARITY_RS:       // 2 raid parity algorithms, XOR, RS
                return "Reed Solomon Parity";
            case PARITY_EXPERIMENTAL: // 3 raid parity algorithms XOR, RS, XOR+RS(?)
                return "Experimental Parity";
            case SPARE:
                return "Spare Volume";
            default:
                return std::string();
        }
    }

    const std::string RaidFile::FileName() const {
        return _fileName;
    }

    uintmax_t RaidFile::SparseSize() {
        // TODO: get the actual size of the file on disk, taking into account sparsefile compression
        return _actualSize;
    }

    uintmax_t RaidFile::Size() {
        return _actualSize;
    }

    uintmax_t RaidFile::SizeOnDisk(std::error_code &ec) {
        uintmax_t fileSize = std::filesystem::file_size(FileName(), ec);
        if (ec) {
            return 0;
        }
        return fileSize;
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

    std::ios_base::openmode RaidFile::Mode() {
        return _fileMode;
    }

    void RaidFile::mode(std::ios_base::openmode mode) {
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

    void RaidFile::setConfiguration(RaidConfiguration* configuration) {
        if (_configuration != nullptr) throw std::invalid_argument("Configuration already set");
        _configuration = configuration;
    }

    //std::shared_ptr<RaidConfiguration> RaidFile::Configuration() {
    //    return _configuration;
    //}

    bool RaidFile::OpenOnly(std::ios_base::openmode mode) {
        if (_fileStream != nullptr) {
            throw std::invalid_argument("File already open");
        }
        _fileMode = mode;
        _fileStream = new std::ofstream(this->FileName(), mode);
        if (!_fileStream->is_open()) {
            this->_configuration->warn("Could not open " + this->FileName());
            return false;
        }
        return true;
    }

    bool RaidFile::Create() {
        if (_fileStream != nullptr) {
            return false;
        }
        std::error_code ec;
        if (!SufficientSpaceForCreate(ec)) {
            this->_configuration->warn(ec.message());
            this->_configuration->error("Insufficient disk space for volume");
            return false;
        }
        // TODO: create
        return true;
    }

    bool RaidFile::OpenOrCreate(std::ios_base::openmode mode) {
        if (!OpenOnly(mode)) {
            return Create();
        }
        return true;
    }

    uintmax_t RaidFile::DiskSpaceAvailable(std::error_code &ec) {
        std::filesystem::space_info spaceInfo = std::filesystem::space(this->_fileName, ec);
        return spaceInfo.available;
    }

    inline bool RaidFile::SufficientSpaceForCreate(std::error_code &ec, uintmax_t withClearance) {
        return (_actualSize <= DiskSpaceAvailable(ec));
    }

}
