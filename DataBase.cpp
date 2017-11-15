//
// Created by arturocv on 13/11/17.
//

#include <SFML/Network/SocketSelector.hpp>
#include "DataBase.h"

///Metodo para crear la tabla de la base de datos
void DataBase::createTable() {
    std::string content = "filename;extension;size;part1;part2;part3;parity\n";
    send(content, "save");
}

/// Metodo para agregar un nuevo elemento a la tabla
/// \param content Elemento a agregar a la tabla
void DataBase::addToTable(std::string content) {
    std::string dataBase = getTable();
    dataBase.append(content);
    dataBase.append("\n");
    send(dataBase, "save");
}

/// Metodo para obtener las partes de la tabla almacenadas en los discos y reconstruir la tabla
/// \return String con la tabla
std::string DataBase::getTable() {
    std::string dataBase = "";

    //Se solicita a cada uno de los discos la parte de la tabla que almacenan
    for (int i = 0; i < 3; ++i) {
        sf::Packet packet;
        packet << "database";
        packet << "get";

        sf::TcpSocket *disk = Singleton::getDisks()->getElement(i)->getData();
        disk->send(packet);

        //Se espera la respuesta del disco
        sf::Packet receivePacket;
        std::string receiveMessage;
        while(true){
        if (disk->receive(receivePacket) == sf::Socket::Done) {
            receivePacket >> receiveMessage;
            dataBase.append(receiveMessage);
            break;
            }
        }
    }

    return dataBase;
}

/// Metodo para almacenar la tabla en los discos
/// \param content Tabla
/// \param action Accion a realizar
void DataBase::send(std::string content, std::string action) {
    int size = content.size() / 3; //Se calcula el tamaño de cada parte

    int start = 0;
    int end = size;

    LinkedList<std::vector<byte>> parts = LinkedList<std::vector<byte>>(); //Lista en la que se guardan las partes para el calculo de la paridad

    for (int i = 0; i < 3; ++i) {
        if(i == 2)
            end = content.size();

        //Se divide el string en partes iguales
        std::string string = std::string(&content[start], &content[end]);
        std::vector<byte> vector(string.begin(), string.end());
        parts.insertAtEnd(vector);

        //Se calcula el nuevo inicio y el nuevo final de la siguiente parte
        start = end;
        end += size;

        //Se envia al disco
        sf::Packet packet;
        packet << "database";
        packet << action;
        packet << string;
        Singleton::getDisks()->getElement(i)->getData()->send(packet);
    }

    //Se realiza el calculo de la paridad
    std::vector<byte> parityVector = FaultTolerance::calculateParity(parts.getElement(0)->getData(),
                                                                     parts.getElement(1)->getData(),
                                                                     parts.getElement(2)->getData());

    //Se envia la patidad al cuarto disco
    sf::Packet parity;
    parity << "database";
    parity << action;
    parity << std::string(parityVector.begin(), parityVector.end());
    Singleton::getDisks()->getElement(3)->getData()->send(parity);
}

/// Metodo para dividir un string en elementos
/// \param string String a separar
/// \param splitCharacter Caracter utilizado para separar los elementos
/// \return LinkedList con los elementos obtenidos de la separacion
LinkedList<std::string> DataBase::splitString(std::string string, char *splitCharacter) {
    LinkedList<std::string> list = LinkedList<std::string>(); //Lista en la que se guardan los elementos
    char* messageChar = strdup(string.c_str()); //Se transforma el mensaje a char*
    char* element = strtok(messageChar, splitCharacter); //Separa el char cuando lea el splitCharacter
    while (element != NULL) {
        std::string str(element);
        list.insertAtEnd(str); // Se guarda el dato en la lista
        element = strtok (NULL, splitCharacter);  // Separa el resto de la cadena cuando lea la coma
    }

    return list;
}

/// Metodo para obtener la informacion de un video que se encuentra en la tabla
/// \param name Nombre del video
/// \return LinkedList con la informacion del video, si esta no se encuentra se retorna una lista vacia
LinkedList<std::string> DataBase::getVideoData(std::string name) {
    //Se obtiene una lista con todos los videos almacenados en la tabla
    LinkedList<std::string> rows = splitString(getTable(), "\n");

    //Se busca la informacion del video solicitado utilizando su nombre
    for (int i = 0; i < rows.getSize(); ++i) {
        std::string row = rows.getElement(i)->getData();
        LinkedList<std::string> data = splitString(row, ";");

        //Se compara el nombre del video actual con el nombre del video que se esta buscando
        if(strcmp(data.getElement(0)->getData().c_str(), name.c_str()) == 0){
            return data;
        }
    }

    return LinkedList<std::string>();
}
