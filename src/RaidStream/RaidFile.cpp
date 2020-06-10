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

    const std::string RaidFile::TypeString() { // TODO: DRY
        std::map<FileType, const std::string>::const_iterator it = FileTypeDescriptions.find(_type);
        if (it != FileTypeDescriptions.end()) {
            return it->second;
        }
        return std::string();
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

    const RaidFile::FileStatus RaidFile::Status() {
        return _fileStatus;
    }

    const std::string RaidFile::StatuString() { // TODO: DRY
        std::map<FileStatus, const std::string>::const_iterator it = FileStatusDescriptions.find(_fileStatus);
        if (it != FileStatusDescriptions.end()) {
            return it->second;
        }
        return std::string();
    }

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
        _fileStream = new std::fstream(this->FileName(), mode);
        if (!_fileStream->is_open()) {
            this->_configuration->warn("Could not open " + this->FileName());
            delete _fileStream;
            _fileStream = nullptr;
            return false;
        }
        return true;
    }

    bool RaidFile::Create(std::ios::openmode mode) {
        if (_fileStream != nullptr) {
            return false;
        }
        std::error_code ec;
        if (!SufficientSpaceForCreate(ec)) {
            this->_configuration->warn(ec.message());
            this->_configuration->error("Insufficient disk space for volume");
            return false;
        }
        std::ofstream tmp = std::ofstream(this->FileName(), std::ios::trunc);
        if (!tmp.is_open()) {
            return false;
        }
        tmp.close();

        return OpenOnly(mode);
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
        return ((_actualSize + withClearance) <= DiskSpaceAvailable(ec));
    }

    void RaidFile::to_json(json &j, const RaidFile &file) {

    }

    RaidFile::RaidFile(RaidConfiguration *config, json configJson) {
        this->setConfiguration(config);
        if (configJson.contains("uuid")) {
            const std::string uuid = configJson.at("uuid").get<std::string>();
            _uuid = sole::rebuild(uuid);
        } else {
            this->_configuration->warn("Config did not contain UUID");
            _uuid = sole::uuid4();
        }
        if (configJson.contains("type")) {
            _type = configJson.at("type").get<FileType>();
        } else {
            this->_configuration->error("Config did not contain TYPE");
        }
    }
}

namespace nlohmann {
    template <>
    struct adl_serializer<RaidStream::RaidFile> {
        static RaidStream::RaidFile from_json(const json& j) {
            return RaidStream::RaidFile(j);
        }
        static void to_json(json& j, RaidStream::RaidFile t) {
            RaidStream::RaidFile::to_json(j, t);
        }
    };
}