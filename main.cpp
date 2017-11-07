#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>

#define PORT 2000

using namespace std;

int main(int argc, char const *argv[])
{
    sf::IpAddress IP = "127.0.0.1";
    cout << IP << "\n" << endl;

    sf::TcpListener listener;
    sf::SocketSelector selector;
    vector<sf::TcpSocket*> clients;
    vector<sf::TcpSocket*> disks;

    listener.listen(PORT);
    selector.add(listener);

    bool done = false;
    while(!done)
    {

        if(selector.wait())
        {
            if(selector.isReady(listener))
            {
                sf::TcpSocket *socket = new sf::TcpSocket;
                listener.accept(*socket);
                sf::Packet packet;
                string id;
                if (socket->receive(packet) == sf::Socket::Done)
                    packet >> id;
                if (id == "Disk") {
                    cout << "Se ha identificado un disco nuevo\n" << endl;
                    disks.push_back(socket);
                    selector.add(*socket);
                }else{
                    cout << id << " se ha conectado al TECMFS\n" << endl;
                    clients.push_back(socket);
                    selector.add(*socket);
                }
            }
            else
            {
                ///Clients
                for(int i=0; i<clients.size(); i++)
                {
                    if(selector.isReady(*clients[i]))
                    {
                        sf::Packet receivePacket, sendPacket;
                        if(clients[i]->receive(receivePacket) == sf::Socket::Done)
                        {
                            ///Clients Actions
                        }
                    }
                }
                ///Disks
                for(int i=0; i<disks.size(); i++)
                {
                    if(selector.isReady(*disks[i]))
                    {
                        sf::Packet receivePacket, sendPacket;
                        if(disks[i]->receive(receivePacket) == sf::Socket::Done)
                        {
                            ///Disks Actions
                        }
                    }
                }
            }
        }

        //for(vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); it++)
        //    delete *it;

    }//while(!done)
}