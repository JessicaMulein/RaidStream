#include "RaidStream/RaidConfiguration.hpp"
#include <filesystem>

namespace RaidStream {
    RaidConfiguration::RaidConfiguration(RaidType type, std::vector<RaidFile> files, std::ostream *os,
                                         std::ostream *oe) : _type{type}, _os{os}, _oe{oe} {

        this->log("Loading configuration of type " + this->TypeString());

        for (std::vector<RaidFile>::iterator it = files.begin(); it != files.end(); ++it) {
            // set the configuration on this RaidFile
            it->_setConfiguration(this);
            this->log("-- Opening " + it->FileName() + " of type " + it->TypeString());

            std::error_code ec;
            uintmax_t actualSizeOnDisk = std::filesystem::file_size(it->FileName(), ec);
            if (ec) {
                this->error(it->FileName() + ": " + ec.message());
                actualSizeOnDisk = 0;
            } else if (actualSizeOnDisk > 0) {
                // if it's empty, its ready to initialize
                // if it's not, this file has data!
                if (it->Status() == RaidFile::NEW) {
                    this->warn(it->FileName() + ": File has existing data");
                }
                this->_bytesActualTotal += actualSizeOnDisk;
            }
            if (it->Size() != actualSizeOnDisk) {
                this->error("Size on disk does not match configuration: " + std::to_string(actualSizeOnDisk) + " actual vs " + std::to_string(it->Size()) + " expected");
            }

            switch (it->Type()) {
                case RaidFile::FileType::DATA:
                    this->_filesData++;
                    this->_bytesData += it->Size();
                    this->_bytesTotal += it->Size();
                    break;
                case RaidFile::FileType::PARITY_MIRROR:
                    this->_filesMirror++;
                    this->_bytesMirror += it->Size();
                    this->_bytesTotal += it->Size();
                    break;
                case RaidFile::FileType::PARITY_XOR:
                    this->_filesXor++;
                    this->_bytesXor += it->Size();
                    this->_bytesTotal += it->Size();
                    break;
                case RaidFile::FileType::PARITY_RS:
                    this->_filesReedSolomon++;
                    this->_bytesReedSolomon += it->Size();
                    this->_bytesTotal += it->Size();
                    break;
                case RaidFile::FileType::PARITY_EXPERIMENTAL:
                    this->_filesExperimental++;
                    this->_bytesExperimental += it->Size();
                    this->_bytesTotal += it->Size();
                    break;
                default:
                    throw std::invalid_argument("Unexpected RaidFile::FileType");
            }
        }
        _files.swap(files);
        this->log("  -- Detected data space: " + std::to_string(_bytesData));
        this->log("  -- Detected mirror space: " + std::to_string(_bytesTotal));
        this->log("  -- Detected XOR space: " + std::to_string(_bytesXor));
        this->log("  -- Detected Reed Solomon space: " + std::to_string(_bytesReedSolomon));
        this->log("  -- Detected Experimental space: " + std::to_string(_bytesExperimental));
        this->log("  -- Total raw space as stored: " + std::to_string(_bytesTotal));
        this->log("Configuration Loaded");
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
        *_oe << "WARNING: " << warning << std::endl;
    }

    void RaidConfiguration::error(std::string error) {
        _errorCount++;
        if ((_oe == nullptr) || (_oe == NULL)) return;
        *_oe << "ERROR: " << error << std::endl;
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

    std::string RaidConfiguration::TypeString() {
        switch (this->_type) {
            case JBOD:        // 0 raid parity algorithms: 0% overhead, 0% redundancy
                return "JBOD";
            case MIRROR:      // 0 raid parity algorithms: 50% overhead, 50% redundancy, requires N % 2 = 0
                return "MIRROR";
            case RAID5:       // 1 raid parity algorithm, XOR:
                return "RAID-5";
            case RAID6:       // 2 raid parity algorithms, XOR, RS
                return "RAID-6";
            case EXPERIMENTAL: // 3 raid parity algorithms XOR, RS, XOR+RS(?)
                return "Experimental";
            default:
                return std::string();
        }
    }

    const unsigned long RaidConfiguration::LogCount() {
        return _logCount;
    }

    const unsigned long RaidConfiguration::WarningCount() {
        return _warningCount;
    }

    const unsigned long RaidConfiguration::ErrorCount() {
        return _errorCount;
    }
}
