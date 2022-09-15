//
// Created by luigi on 02/09/22.
//

#ifndef PARALLELDAG_MDFI_H
#define PARALLELDAG_MDFI_H
#include "node.h"
#include <vector>

template<typename T>
using vector = std::vector<T>;
template<typename T>
class Mdfi {
    private:
         // TO BE USED ONLY FOR SOURCE INSTR
        int outputToken;
    public:

        Node<T> * dagNode;
        bool firable;
        int missingToken;
        int inputToken;

        vector<Mdfi *> outputDestination;
        vector<vector<int>> inputs;

        Mdfi(Node<T> * node);
        bool addOuputDest(Mdfi * dest);
        void setFirable();
        bool checkFirable();
        vector<T> run(vector<T>);




};
#endif //PARALLELDAG_MDFI_H
