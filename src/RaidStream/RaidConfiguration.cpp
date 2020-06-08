#include "RaidStream/RaidConfiguration.hpp"
#include <filesystem>

namespace RaidStream {
    RaidConfiguration::RaidConfiguration(RaidType type, std::vector<RaidFile> files, std::ostream *os,
                                         std::ostream *oe) : _type{type}, _os{os}, _oe{oe} {
        for (std::vector<RaidFile>::iterator it = files.begin(); it != files.end(); ++it) {
            // set the configuration on this RaidFile
            it->_setConfiguration(this);

            std::error_code ec;
            uintmax_t fileSize = std::filesystem::file_size(it->FileName(), ec);
            if (ec) {
                this->warn("ERROR: " + it->FileName() + ": " + ec.message());
            }
            if (fileSize > 0) {
                // if it's empty, its ready to initialize
                // if it's not, this file has data!
                this->warn("WARNING: " + it->FileName() + ": File has existing data");
            }
            switch (it->Type()) {
                case RaidFile::FileType::DATA:
                    this->_filesData++;
                    this->_expectedAvailableDataBytes += fileSize;
                    this->_expectedTotalBytes += fileSize;
                    break;
                case RaidFile::FileType::PARITY_MIRROR:
                    this->_filesMirror++;
                    this->_expectedMirrorBytes += fileSize;
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
        this->log("Detected data space: " + std::to_string(_expectedAvailableDataBytes));
        this->log("Detected mirror space: " + std::to_string(_expectedTotalBytes));
        this->log("Detected XOR space: " + std::to_string(_expectedXorBytes));
        this->log("Detected Reed Solomon space: " + std::to_string(_expectedReedSolomonBytes));
        this->log("Detected Experimental space: " + std::to_string(_expectedExperimentalBytes));
        this->log("Total raw space as stored: " + std::to_string(_expectedTotalBytes));
    }

    std::vector<RaidFile> RaidConfiguration::Files() {
        return _files;
    }

    std::pair<unsigned int, unsigned int> RaidConfiguration::logStats() {
        return std::make_pair(_logCount, _warningCount);
    }

    void RaidConfiguration::log(std::string data) {
        _logCount++;
        if ((_os == nullptr) || (_os == NULL)) return;
        *_os << data << std::endl;
    }

    void RaidConfiguration::warn(std::string warning) {
        _warningCount++;
        if ((_oe == nullptr) || (_oe == NULL)) return;
        *_oe << warning << std::endl;
    }

    void RaidConfiguration::setStdOut(std::ostream *fs) {
        _os = fs;
    }

    void RaidConfiguration::setStdErr(std::ostream *fs) {
        _oe = fs;
    }

    sole::uuid RaidConfiguration::UUID() {
        return _uuid;
    }

    RaidConfiguration::RaidType RaidConfiguration::Type() {
        return _type;
    }
}
