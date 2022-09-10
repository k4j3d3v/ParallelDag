#include "include/Mdfi.h"

Mdfi::Mdfi(Node * node):dagNode(node),inputToken(node->input_size),\
        outputToken(node->out_arity),missingToken(node->input_arity)
{
    //outputDestination = node->getDependant();
    firable = missingToken == 0;
    if(inputToken==0)
        inputs.resize(this->missingToken);
    else
        inputs.resize(inputToken);

}
bool Mdfi::addOuputDest(Mdfi * dest)
{
    if (outputDestination.size() < outputToken) {
        outputDestination.emplace_back(dest);
        return true;
    }
    return false;
}
void Mdfi::setFirable()
{
    firable=true;
}
bool Mdfi::checkFirable()
{
    return missingToken == 0;
}
std::vector<int> Mdfi::run(vector<int> flattenedInputs)
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
