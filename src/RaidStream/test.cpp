#include "RaidStream/RaidStream.hpp"

int main() {
    // open up the raidstream
    RaidStream::RaidStream rs = RaidStream::RaidStream(RaidStream::RaidStream::MakeConfiguration(
            RaidStream::RaidConfiguration::RaidType::EXPERIMENTAL, {
                    {"/tmp/raidstream.0", RaidStream::RaidFile::FileType::DATA},
                    {"/tmp/raidstream.1", RaidStream::RaidFile::FileType::DATA},
                    {"/tmp/raidstream.2", RaidStream::RaidFile::FileType::DATA},
                    {"/tmp/raidstream.3", RaidStream::RaidFile::FileType::DATA},
                    {"/tmp/raidstream.4", RaidStream::RaidFile::FileType::PARITY_XOR},
                    {"/tmp/raidstream.5", RaidStream::RaidFile::FileType::PARITY_RS},
                    {"/tmp/raidstream.6", RaidStream::RaidFile::FileType::PARITY_EXPERIMENTAL}
            }), true, false);
}
