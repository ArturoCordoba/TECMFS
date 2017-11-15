//
// Created by arturocv on 13/11/17.
//

#ifndef TECMFS_DATABASE_H
#define TECMFS_DATABASE_H


#include <iostream>
#include <fstream>
#include <SFML/Network/Packet.hpp>
#include "Singleton.h"
#include "FaultTolerance.h"

class DataBase {
public:
    static void createTable();

    static void addToTable(std::string content);

    static std::string getTable();

    static LinkedList<std::string> splitString(std::string string, char *splitCharacter);

    static LinkedList<std::string> getVideoData(std::string name);

private:
    static void send(std::string content, std::string action);
};


#endif //TECMFS_DATABASE_H
