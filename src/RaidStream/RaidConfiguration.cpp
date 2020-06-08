#include "RaidStream/RaidConfiguration.hpp"
#include "RaidStream/RaidStream.hpp"
#include <filesystem>

namespace RaidStream {
    RaidConfiguration::RaidConfiguration(RaidType type, std::vector<RaidFile> files, std::ostream *os,
                                         std::ostream *oe) : _type{type}, _os{os}, _oe{oe} {

        this->log("Loading configuration of type " + this->TypeString());
        this->log("Configuration UUID: " + UUID().str());

        for (std::vector<RaidFile>::iterator it = files.begin(); it != files.end(); ++it) {
            // set the configuration on this RaidFile
            it->setConfiguration(this);
            this->log(" -- Found described volume: " + it->FileName() + " of type " + it->TypeString());
            this->log(" -- Volume UUID: " + it->UUID().str());

            std::error_code ec;
            uintmax_t actualSizeOnDisk = it->SizeOnDisk(ec);
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
                if (it->Status() == RaidFile::NEW) {
                    this->log("-- File on disk is empty, but expected as this is marked NEW");
                    this->warn("Array file will be initialized");
                } else {
                    this->error("Size on disk does not match configuration: " + BytesToSize(actualSizeOnDisk) + " actual vs " + BytesToSize(it->Size()) + " expected");
                }
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
        this->log("-- Done reading configuration. Calculating totals.");
        this->log("  -- Configured data space used: " + BytesToSize(_bytesData));
        this->log("  -- Configured mirror space used: " + BytesToSize(_bytesMirror));
        this->log("  -- Configured XOR space used: " + BytesToSize(_bytesXor));
        this->log("  -- Configured Reed Solomon space used: " + BytesToSize(_bytesReedSolomon));
        this->log("  -- Configured Experimental space used: " + BytesToSize(_bytesExperimental));
        this->log("  -- Configured total array size on disk: " + BytesToSize(_bytesTotal));
        this->log("  -- Total space use as detected on disk: " + BytesToSize(_bytesActualTotal));
        this->log("Configuration Loaded");
    }

    std::vector<RaidFile>& RaidConfiguration::Files() {
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

    std::string RaidConfiguration::BytesToSize(uintmax_t bytes) {
        std::ostringstream s;
        s << std::fixed << std::setprecision(2) << std::showpoint;
        if(bytes >= UNITS_TB )
            s << ((float)bytes / UNITS_TB) << " TB";
        else if(bytes >= UNITS_GB && bytes < UNITS_TB )
            s << ((float)bytes / UNITS_GB) << " GB";
        else if(bytes >= UNITS_MB && bytes < UNITS_GB )
            s << ((float)bytes / UNITS_MB) << " MB";
        else if(bytes >= UNITS_KB && bytes < UNITS_MB )
            s << ((float)bytes / UNITS_KB) << " KB";
        else /* if (bytes < UNITS_KB) */
            s << bytes << " Bytes";
        return s.str();
    }
}
