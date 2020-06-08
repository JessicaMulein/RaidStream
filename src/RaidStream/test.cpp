#include "RaidStream/RaidStream.hpp"

typedef std::vector<std::shared_ptr<RaidStream::RaidConfiguration>> raid_config_vector;
int main() {
    std::cout << "Making configurations" << std::endl;

    std::vector<RaidStream::RaidFile> v0;
    v0.emplace_back("/tmp/raidstream.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v0.emplace_back("/tmp/raidstream.1", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v0.emplace_back("/tmp/raidstream.2", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v0.emplace_back("/tmp/raidstream.3", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v0.emplace_back("/tmp/raidstream.4", RaidStream::RaidFile::FileType::PARITY_XOR,static_cast<uintmax_t>(16*UNITS_MB));
    v0.emplace_back("/tmp/raidstream.5", RaidStream::RaidFile::FileType::PARITY_RS,static_cast<uintmax_t>(16*UNITS_MB));
    v0.emplace_back("/tmp/raidstream.6", RaidStream::RaidFile::FileType::PARITY_EXPERIMENTAL,static_cast<uintmax_t>(16*UNITS_MB));

    std::vector<RaidStream::RaidFile> v1;
    v1.emplace_back("/tmp/raidstream.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v1.emplace_back("/tmp/raidstream.1", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v1.emplace_back("/tmp/raidstream.2", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v1.emplace_back("/tmp/raidstream.3", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v1.emplace_back("/tmp/raidstream.4", RaidStream::RaidFile::FileType::PARITY_XOR,static_cast<uintmax_t>(16*UNITS_MB));
    v1.emplace_back("/tmp/raidstream.5", RaidStream::RaidFile::FileType::PARITY_RS,static_cast<uintmax_t>(16*UNITS_MB));
    v1.emplace_back("/tmp/raidstream.6", RaidStream::RaidFile::FileType::PARITY_EXPERIMENTAL,static_cast<uintmax_t>(16*UNITS_MB));

    std::vector<RaidStream::RaidFile> v2;
    v2.emplace_back("/tmp/raidstream2.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v2.emplace_back("/tmp/raidstream2.1", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v2.emplace_back("/tmp/raidstream2.2", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v2.emplace_back("/tmp/raidstream2.3", RaidStream::RaidFile::FileType::PARITY_XOR,static_cast<uintmax_t>(16*UNITS_MB));
    v2.emplace_back("/tmp/raidstream2.4", RaidStream::RaidFile::FileType::PARITY_RS,static_cast<uintmax_t>(16*UNITS_MB));

    std::vector<RaidStream::RaidFile> v3;
    v3.emplace_back("/tmp/raidstream3.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v3.emplace_back("/tmp/raidstream3.1", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v3.emplace_back("/tmp/raidstream3.2", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB));
    v3.emplace_back("/tmp/raidstream3.3", RaidStream::RaidFile::FileType::PARITY_XOR,static_cast<uintmax_t>(16*UNITS_MB));

    std::vector<RaidStream::RaidFile> v4;
    v4.emplace_back("/tmp/raidstream4.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(40*UNITS_MB));
    v4.emplace_back("/tmp/raidstream4.1", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(40*UNITS_MB));
    v4.emplace_back("/tmp/raidstream4.2", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(40*UNITS_MB));

    std::vector<RaidStream::RaidFile> v5;
    v5.emplace_back("/tmp/raidstream5.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(40*UNITS_MB));
    v5.emplace_back("/tmp/raidstream5.1", RaidStream::RaidFile::FileType::PARITY_MIRROR,static_cast<uintmax_t>(40*UNITS_MB));

    raid_config_vector configurations = {
        RaidStream::RaidStream::MakeConfiguration(
            RaidStream::RaidConfiguration::RaidType::EXPERIMENTAL, std::move(v0), &std::cout, &std::cerr),
            RaidStream::RaidStream::MakeConfiguration(
                    RaidStream::RaidConfiguration::RaidType::EXPERIMENTAL, std::move(v1), &std::cout, &std::cerr),
            RaidStream::RaidStream::MakeConfiguration(
                    RaidStream::RaidConfiguration::RaidType::RAID6, std::move(v2), &std::cout, &std::cerr),
        RaidStream::RaidStream::MakeConfiguration(
                RaidStream::RaidConfiguration::RaidType::RAID5, std::move(v3), &std::cout, &std::cerr),
        RaidStream::RaidStream::MakeConfiguration(
                RaidStream::RaidConfiguration::RaidType::JBOD, std::move(v4), &std::cout, &std::cerr),
        RaidStream::RaidStream::MakeConfiguration(
                RaidStream::RaidConfiguration::RaidType::MIRROR, std::move(v5), &std::cout, &std::cerr)
    };

    std::cout << "Creating raid(s) for testing" << std::endl;
    // do tests
    for (raid_config_vector::iterator it = configurations.begin(); it != configurations.end(); ++it) {
        std::cout << "-- Loading Configuration: " << std::endl;
        RaidStream::RaidConfiguration *c = it->get();
        std::cout << "  -- " << c->ErrorCount() << " Errors" << std::endl;
        std::cout << "  -- " << c->WarningCount() << " Warnings" << std::endl;
        std::cout << "  -- " << c->LogCount() << " Messages" << std::endl;
        RaidStream::RaidStream rs{*it, true, false};
        std::cout << "  -- Loaded with status: " << rs.StatusString() << std::endl;
        rs.Open();
    }

}
