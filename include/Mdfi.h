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

        Node<T> * dagNode;
        int missingToken;
        int inputToken;

        vector<Mdfi *> outputDestination;
        vector<vector<int>> inputs;

public:

        Mdfi(Node<T> * node):dagNode(node),inputToken(node->input_size),\
        outputToken(node->out_arity),missingToken(node->input_arity)
        {
            if(inputToken==0)
                inputs.resize(this->missingToken);
            else
                inputs.resize(inputToken);

        }
        bool addOutputDest(Mdfi * dest)
        {
            if (outputDestination.size() < outputToken) {
                outputDestination.emplace_back(dest);
                return true;
            }
            return false;
        }
        bool checkFirable()
        {
            return missingToken == 0;
        }
        vector<T> run(vector<T> flattenedInputs)
        {
            return this->dagNode->getTask()(flattenedInputs);
        }


};
#endif //PARALLELDAG_MDFI_H
