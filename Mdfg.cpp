#include "include/Mdfg.h"
#include "include/graph.h"
#include "utimer.cpp"

Mdfg::Mdfg(Graph * dag){

            for (auto node : dag->getNodes()) {
                Mdfi * instr = new Mdfi(node);
                mapNodeMdfi[node]=instr;
                if(instr->firable)
                    firable.push(instr);
                else
                    repository.push_back(instr);
            }
            for(int i = 0; i < firable.size(); i++)
            {
               Mdfi * instr = firable.front();
               std::vector<Node *> depNodes = instr->dagNode->getDependant();
               for(auto node : depNodes)
                   instr->addOuputDest(mapNodeMdfi[node]);
               auto fi = std::bind(&Mdfg::fireInstr, this, instr);
               tp.submit(fi);
            }
            for(auto instr : repository)
            {
               auto depNodes = instr->dagNode->getDependant();
               for(auto node : depNodes)
                   instr->addOuputDest(mapNodeMdfi[node]);
            }
        }

void Mdfg::fireInstr(Mdfi *instr)
{
    instr->run();
    sendToken(instr);

}
 void Mdfg::sendToken(Mdfi * executeInstr){
    auto dst =  executeInstr->outputDestination;

    for(Mdfi * outDest : dst)
    {

            outDest->missingToken--;
            if (outDest->missingToken == 0) {
                std::erase(repository, outDest);
                outDest->setFirable();
                //firable.push(outDest);
                auto fi = std::bind(&Mdfg::fireInstr, this, outDest);
                tp.submit(fi);
            }
    }

}

unsigned Mdfg::countMissingInstructions()
{
    return repository.size();
}
void Mdfg::start(int nw)
{
    tp.run(nw);
    tp.join();
}
Mdfi * Mdfg::getFirable()
{
    if(repository.empty() && firable.empty()) {
        computation_done = true;
        return nullptr;
    }

    Mdfi *instr;
    instr = firable.front();
    firable.pop();

    return instr;
}

        
