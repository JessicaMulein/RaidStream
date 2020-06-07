#include "RaidStream/RaidStream.hpp"

int main() {
    // open up the raidstream
    RaidStream::RaidStream rs = RaidStream::RaidStream(RaidStream::RaidStream::MakeConfiguration(
            RaidStream::RaidConfiguration::RaidType::EXPERIMENTAL, {
                    {"/tmp/raidstream.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(4*UNITS_GB)},
                    {"/tmp/raidstream.1", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(4*UNITS_GB)},
                    {"/tmp/raidstream.2", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(4*UNITS_GB)},
                    {"/tmp/raidstream.3", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(4*UNITS_GB)},
                    {"/tmp/raidstream.4", RaidStream::RaidFile::FileType::PARITY_XOR,static_cast<uintmax_t>(4*UNITS_GB)},
                    {"/tmp/raidstream.5", RaidStream::RaidFile::FileType::PARITY_RS,static_cast<uintmax_t>(4*UNITS_GB)},
                    {"/tmp/raidstream.6", RaidStream::RaidFile::FileType::PARITY_EXPERIMENTAL,static_cast<uintmax_t>(4*UNITS_GB)}
            }), true, false);
}
