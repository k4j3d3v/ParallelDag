#include "include/Mdfi.h"

template<typename T>
Mdfi<T>::Mdfi(Node<T> * node):dagNode(node),inputToken(node->input_size),\
        outputToken(node->out_arity),missingToken(node->input_arity)
{
    //outputDestination = node->getDependant();
    firable = missingToken == 0;
    if(inputToken==0)
        inputs.resize(this->missingToken);
    else
        inputs.resize(inputToken);

}
template<typename T>
bool Mdfi<T>::addOuputDest(Mdfi<T> * dest)
{
    if (outputDestination.size() < outputToken) {
        outputDestination.emplace_back(dest);
        return true;
    }
    return false;
}
template<typename T>
void Mdfi<T>::setFirable()
{
    firable=true;
}
template<typename T>
bool Mdfi<T>::checkFirable()
{
    return missingToken == 0;
}
template<typename T>
std::vector<T> Mdfi<T>::run(vector<T> flattenedInputs)
{
    return this->dagNode->getTask()(flattenedInputs);
}



// from project specification assuming to work by side effect
/*void sendToken(){
    for(Mdfi * outDest : outputDestination)
    {
        outDest->missingToken--;
        if(outDest->missingToken == 0)
            outDest->setFirable();
    }
}*/
