#ifndef TCP_HPP
# define TCP_HPP

#include <vector>
#include <set>

class TCPSequencer
{
    public:
        // Coplien Form
        TCPSequencer();
        TCPSequencer(const TCPSequencer&);
        ~TCPSequencer(){};
        TCPSequencer &operator=(const TCPSequencer&);

        std::vector<int> receivePacket(int packetId);

    private:
        int _expectedPacket;
        std::set<int> _receivedPackets;
};

#endif