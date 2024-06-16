#include <vector>
#include "tcp.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest_proxy.h"

TCPSequencer::TCPSequencer(): _expectedPacket(0) 
{
}

std::vector<int> TCPSequencer::receivePacket(int packetId)
{
    // Inserting directly packetId into the received packet set
    this->_receivedPackets.insert(packetId);

    std::vector<int>            packetsRes;
    auto                        packetsIter = this->_receivedPackets.begin();

    if (_expectedPacket != packetId)
    // Checking if packetId is different from the expected id, if true returning empty vector
        return packetsRes;

    while (packetsIter != this->_receivedPackets.end() && packetId == (*packetsIter))
    {// Incrementing packetId and pushing into the vector until a gap is found or encountering the end of the receivedPackets
        packetsRes.push_back(packetId++);
        // Erasing packet to prevent returning already sent IDs
        packetsIter = this->_receivedPackets.erase(packetsIter);
    }
    // Changing the value of the expected packet
    this->_expectedPacket = packetId;
    return packetsRes;
}

TEST_CASE("Return packets ids in order") {
    TCPSequencer seq;
    CHECK(seq.receivePacket(4) == std::vector<int>{});
    CHECK(seq.receivePacket(0) == std::vector<int>{0});
    CHECK(seq.receivePacket(2) == std::vector<int>{});
    CHECK(seq.receivePacket(3) == std::vector<int>{});
    CHECK(seq.receivePacket(1) == std::vector<int>{1, 2, 3, 4});
    CHECK(seq.receivePacket(5) == std::vector<int>{5});
}


/**
     * Receive a packet ID and return the IDs of the received packet in order, starting from 0, without gaps.
     * Does not return already returned IDs.
     * @example
     * receivePacket(1) -> []
     * receivePacket(2) -> []
     * receivePacket(0) -> [0,1,2]
     * receivePacket(3) -> [3]
     * @param packetId The id of the received packet.
     * @return The ids of the received packet, in order
     */