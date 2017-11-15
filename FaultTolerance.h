//
// Created by arturocv on 14/11/17.
//

#ifndef TECMFS_FAULTTOLERANCE_H
#define TECMFS_FAULTTOLERANCE_H


#include <iostream>
#include <fstream>
#include <vector>

typedef unsigned char byte;

class FaultTolerance {
public:
    /// Metodo que utiliza el XOR para calcular la paridad de un archivo
    /// \param part1 Parte uno del archivo
    /// \param part2 Parte dos del archivo
    /// \param part3 Parte tres del archivo
    /// \return Vector con la paridad
    static std::vector<byte> calculateParity(std::vector<byte> part1, std::vector<byte> part2, std::vector<byte> part3) {
        std::vector<byte> vectorParity;

        //Por cada byte de las partes se aplica el XOR y se añade el resultado al vector de paridad
        for(int i = 0; i < part3.size(); i++) {
            vectorParity.push_back(part1[i] ^ part2[i] ^ part3[i]);
        }

        return vectorParity;
    }

    /// Metodo para recuperar una parte del archivo utilizando la paridad del mismo
    /// \param partA Parte del archivo
    /// \param partB Parte del archivo
    /// \param parity Paridad del archivo
    /// \return Vector con la parte del archivo que se perdio
    static std::vector<byte> recoverData(std::vector<byte> partA, std::vector<byte> partB, std::vector<byte> parity) {
        std::vector<byte> vectorFault;

        //Por cada byte del vector se utiliza el XOR para obtener la parte perdida
        for(int i = 0; i < parity.size(); i++) {
            vectorFault.push_back(partA[i] ^ partB[i] ^ parity[i]);
        }

        if(partA.size() != partB.size()) {
            for (int j = 0; j < (parity.size() - partA.size()); ++j) {
                vectorFault.erase(vectorFault.begin() + (vectorFault.size() - 1));
            }
        }

        return vectorFault;
    }
};


#endif //TECMFS_FAULTTOLERANCE_H
