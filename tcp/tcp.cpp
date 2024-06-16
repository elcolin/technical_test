#include <vector>
#include "tcp.hpp"

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest_proxy.h"

class TCPSequencer
{
    public:
        // Coplien Form
        TCPSequencer();
        // TCPSequencer(const TCPSequencer&);
        // ~TCPSequencer(){};
        // TCPSequencer &operator=(const TCPSequencer&);

        std::vector<int> receivePacket(int packetId);

    private:
        int             _expectedPacket;
        // On choisit set car la valeur est la clé, ce conteneur permet d'obtenir les valeurs dans l'ordre
        std::set<int>   _receivedPackets;
};

TCPSequencer::TCPSequencer(): _expectedPacket(0) 
{
}

std::vector<int> TCPSequencer::receivePacket(int packetId)
{
    // Insertion du nouveau packet
    this->_receivedPackets.insert(packetId);

    std::vector<int>            packetsRes;
    auto                        packetsIter = this->_receivedPackets.begin();

    if (_expectedPacket != packetId)
    // Si ce n'est pas le packet attendu, retour d'un vector vide
        return packetsRes;

    while (packetsIter != this->_receivedPackets.end() && packetId == (*packetsIter))
    {// Incrementation sur les packets reçu jusqu'à la présence d'un gap ou fin du set
        packetsRes.push_back(packetId++);
        // Suppression du packet du set pour éviter les doublons
        packetsIter = this->_receivedPackets.erase(packetsIter);
    }
    // Changement de la valeur du prochain packet attendu
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