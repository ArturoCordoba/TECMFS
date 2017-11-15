//
// Created by arturocv on 14/11/17.
//

#include "Controller.h"

/// Metodo para dividir el video y enviar las partes a los discos
/// \param data Vector con el video
/// \param filename Nombre del archivo del video
void Controller::sendParts(std::vector<byte> data, std::string filename) {
    //Se obtienen las partes del video y la paridad
    LinkedList<std::vector<byte>> parts = VideoHandler::splitVideo(data);
    std::vector<byte> parity = FaultTolerance::calculateParity(parts.getElement(0)->getData(), parts.getElement(1)->getData(), parts.getElement(2)->getData());

    //Se obtiene el nombre y la extension del fichero
    LinkedList<std::string> list = DataBase::splitString(filename, ".");
    std::string name = list.getElement(0)->getData();
    std::string extension = list.getElement(1)->getData();

    //Se forma el string que almacena el nuevo elemento de la base de datos
    std::string newElement = name + ";" + extension + ";" + std::to_string(data.size()) + ";";
    std::string diskParity;

    int parityDisk = getParityDisk();
    int currentPart = 0;

    //Se envian las partes a cada uno de los discos
    for (int i = 0; i < 4; ++i) {
        sf::Packet packet;
        packet << "savePart";

        //El disco actual es al que le corresponde la paridad
        if(i == parityDisk){
            packet << name + "_parity";
            packet << std::string(parity.begin(), parity.end());
            Singleton::getDisks()->getElement(i)->getData()->send(packet);
            diskParity = std::to_string(i);

        //Al disco actual no le corresponde la paridad
        } else {
            packet << name + std::to_string(currentPart);
            std::vector<byte> part = parts.getElement(currentPart)->getData();
            packet << std::string(part.begin(), part.end());
            newElement.append(std::to_string(i) + ";");
            currentPart++;

        }

        //Se envia el packet
        Singleton::getDisks()->getElement(i)->getData()->send(packet);
    }

    //Se termina de formar el nuevo elemento y se envia a la base de datos
    newElement.append(diskParity);
    DataBase::addToTable(newElement);

}

/// Metodo para obtener a cual disco le corresponde la paridad
/// \return Posicion del disco que le corresponde la paridad
int Controller::getParityDisk() {
    int disc = parityDisc;
    parityDisc++;

    if(parityDisc > 3){
        parityDisc = 0;
    }

    return disc;
}

/// Metodo para obtener las partes del video almacenadas en los discos
/// \param name Nombre del video
/// \return Un string del video
std::string Controller::getVideo(std::string name) {
    LinkedList<std::string> data = DataBase::getVideoData(name);

    //Se verifica que se obtuvo el video de manera correcta
    if(data.getSize() > 0){
        std::string part1 = "";
        std::string part2 = "";
        std::string part3 = "";

        int currentPart = 0;

        //Se solicitan las partes del video a cada uno de los discos
        for (int i = 1; i < 4; ++i) {
            //Se obtiene el disco en el que se encuentra la parte actual
            int diskNum = std::stoi(data.getElement(2+i)->getData());

            //Se obtiene la parte actual del video
            if(i == 1){
                part1 = getPart(name + std::to_string(currentPart), diskNum);
            } else if(i == 2){
                part2 = getPart(name + std::to_string(currentPart), diskNum);
            } else if(i == 3){
                part3 = getPart(name + std::to_string(currentPart), diskNum);
            }
            currentPart++;
        }

        //Se reconstruye el video
        std::string video = VideoHandler::joinVideo(part1, part2, part3);

        return video;
    }

}

/// Metodo para obtener una parte del video de un disco en especifico
/// \param name Nombre del video
/// \param diskNum Numero del disco
/// \return Un string con la parte obtenida del disco
std::string Controller::getPart(std::string name, int diskNum) {
    //Se obtiene el disco solicitado
    sf::TcpSocket* disk = Singleton::getDisks()->getElement(diskNum)->getData();

    //Se le indica al disco que se esta solicitando una parte de un video
    sf::Packet packet;
    packet << "getPart";
    packet << name;
    disk->send(packet);

    //Se espera la respuesta del disco
    sf::Packet receivePacket;
    std::string receiveMessage = "";

    while(true){
        if (disk->receive(receivePacket) == sf::Socket::Done) {
            receivePacket >> receiveMessage;
            break;
        }
    }

    return receiveMessage;
}
