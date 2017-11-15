//
// Created by arturocv on 13/11/17.
//

#ifndef TECMFS_SINGLETON_H
#define TECMFS_SINGLETON_H


#include <SFML/Network/TcpSocket.hpp>
#include "DataStructures/LinkedList.h"
#include "Controller.h"

class Singleton {
public:
    static Singleton* getInstance();

    static LinkedList<sf::TcpSocket*>* getClients();

    static LinkedList<sf::TcpSocket*>* getDisks();

private:
    Singleton();

    static Singleton* instance;

    static LinkedList<sf::TcpSocket*>* clients;

    static LinkedList<sf::TcpSocket*>* disks;

};


#endif //TECMFS_SINGLETON_H
