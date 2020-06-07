#include "RaidStream/RaidStream.hpp"
#include "RaidStream/RaidConfiguration.hpp"

namespace RaidStream {
    RaidConfiguration::RaidConfiguration(RaidType type, std::vector<RaidFile> files, std::fstream *stdout,
                      std::fstream *stderr) : _type{type} {
        RaidConfiguration *This = this;
        for (std::vector<RaidFile>::iterator it = files.begin(); it != files.end(); ++it) {
            std::error_code ec;
            uintmax_t fileSize = std::filesystem::file_size(it->FileName(), ec);
            if (ec) {
                This->warn("ERROR: " + it->FileName() + ": " + ec.message());
            }
            if (fileSize > 0) {
                // if it's empty, its ready to initialize
                // if it's not, this file has data!
                This->warn("WARNING: " + it->FileName() + ": File has existing data");
            }
            switch (it->Type()) {
                case RaidFile::FileType::DATA:
                    this->_filesData++;
                    this->_expectedAvailableDataBytes += fileSize;
                    this->_expectedTotalBytes += fileSize;
                    break;
                case RaidFile::FileType::PARITY_XOR:
                    this->_filesXor++;
                    this->_expectedXorBytes += fileSize;
                    this->_expectedTotalBytes += fileSize;
                    break;
                case RaidFile::FileType::PARITY_RS:
                    this->_filesReedSolomon++;
                    this->_expectedReedSolomonBytes += fileSize;
                    this->_expectedTotalBytes += fileSize;
                    break;
                case RaidFile::FileType::PARITY_EXPERIMENTAL:
                    this->_filesExperimental++;
                    this->_expectedExperimentalBytes += fileSize;
                    this->_expectedTotalBytes += fileSize;
                    break;
                default:
                    throw std::invalid_argument("Unexpected RaidFile::FileType");
            }
        }
        _files.swap(files);
    }

    std::vector<RaidFile> RaidConfiguration::Files() {
        return _files;
    }

    std::pair<unsigned int, unsigned int> RaidConfiguration::logStats() {
        return std::make_pair(_logCount, _warningCount);
    }

    void RaidConfiguration::log(std::string data) {
        _logCount++;
        if ((_stdout == nullptr) || (_stdout == NULL)) return;
        *_stdout << data << std::endl;
    }

    void RaidConfiguration::warn(std::string warning) {
        _warningCount++;
        if ((_stderr == nullptr) || (_stderr == NULL)) return;
        *_stderr << warning << std::endl;
    }

    void RaidConfiguration::setStdOut(std::fstream *fs) {
        _stdout = fs;
    }

    void RaidConfiguration::setStdErr(std::fstream *fs) {
        _stderr = fs;
    }

    sole::uuid RaidConfiguration::UUID() {
        return _uuid;
    }
}
