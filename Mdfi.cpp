#include "include/Mdfi.h"

Mdfi::Mdfi(Node * node):dagNode(node),inputToken(node->input_arity),\
        outputToken(node->out_arity),missingToken(inputToken)
{
    //outputDestination = node->getDependant();
    firable = inputToken == 0;
    if(node->id==1)
    {
        std::cout<<"Node 1, in arity: "<<node->input_arity<<" inputToken: "\
                <<inputToken<<" firable? "<<firable<<" \n";
    }
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
void Mdfi::run()
{
    this->dagNode->getTask()();
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
