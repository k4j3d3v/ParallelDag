//
// Created by luigi on 02/09/22.
//

#ifndef PARALLELDAG_MDFI_H
#define PARALLELDAG_MDFI_H
#include "node.h"
#include <vector>

class Mdfi {
    private:
        int inputToken;
        int outputToken;
    public:

        Node * dagNode;
        bool firable;
        int missingToken;

        std::vector<Mdfi *> outputDestination;

        Mdfi(Node * node);
        bool addOuputDest(Mdfi * dest);
        void setFirable();
        bool checkFirable();
        void run();




};
#endif //PARALLELDAG_MDFI_H
