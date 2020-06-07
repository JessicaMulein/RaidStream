#include "RaidStream/RaidStream.hpp"

int main() {
    RaidStream::RaidStream rs = RaidStream::RaidStream(RaidStream::RaidStream::MakeConfiguration(
            RaidStream::RaidConfiguration::RaidType::EXPERIMENTAL, {
                    {"/tmp/raidstream.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream.1", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream.2", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream.3", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream.4", RaidStream::RaidFile::FileType::PARITY_XOR,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream.5", RaidStream::RaidFile::FileType::PARITY_RS,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream.6", RaidStream::RaidFile::FileType::PARITY_EXPERIMENTAL,static_cast<uintmax_t>(16*UNITS_MB)}
            }), true, false);

    RaidStream::RaidStream rs2 = RaidStream::RaidStream(RaidStream::RaidStream::MakeConfiguration(
            RaidStream::RaidConfiguration::RaidType::RAID6, {
                    {"/tmp/raidstream2.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream2.1", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream2.2", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream2.3", RaidStream::RaidFile::FileType::PARITY_XOR,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream2.4", RaidStream::RaidFile::FileType::PARITY_RS,static_cast<uintmax_t>(16*UNITS_MB)},
            }), true, false);

    RaidStream::RaidStream rs3 = RaidStream::RaidStream(RaidStream::RaidStream::MakeConfiguration(
            RaidStream::RaidConfiguration::RaidType::RAID5, {
                    {"/tmp/raidstream3.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream3.1", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream3.2", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(16*UNITS_MB)},
                    {"/tmp/raidstream3.3", RaidStream::RaidFile::FileType::PARITY_XOR,static_cast<uintmax_t>(16*UNITS_MB)},
            }), true, false);

    RaidStream::RaidStream rs4 = RaidStream::RaidStream(RaidStream::RaidStream::MakeConfiguration(
            RaidStream::RaidConfiguration::RaidType::JBOD, {
                    {"/tmp/raidstream4.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(40*UNITS_MB)},
                    {"/tmp/raidstream4.1", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(40*UNITS_MB)},
                    {"/tmp/raidstream4.2", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(40*UNITS_MB)},
            }), true, false);

    RaidStream::RaidStream rs5 = RaidStream::RaidStream(RaidStream::RaidStream::MakeConfiguration(
            RaidStream::RaidConfiguration::RaidType::MIRROR, {
                    {"/tmp/raidstream5.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(40*UNITS_MB)},
                    {"/tmp/raidstream5.1", RaidStream::RaidFile::FileType::PARITY_MIRROR,static_cast<uintmax_t>(40*UNITS_MB)},
            }), true, false);

    // this should hypothetically work
    RaidStream::RaidStream rs6 = RaidStream::RaidStream(RaidStream::RaidStream::MakeConfiguration(
            RaidStream::RaidConfiguration::RaidType::RAID5, {
                    {"/tmp/raidstream6.0", RaidStream::RaidFile::FileType::DATA,static_cast<uintmax_t>(40*UNITS_MB)},
                    {"/tmp/raidstream6.1", RaidStream::RaidFile::FileType::PARITY_XOR,static_cast<uintmax_t>(40*UNITS_MB)},
            }), true, false);

}
