#include "include/Mdfg.h"
#include "include/graph.h"
#include "utimer.cpp"
#include <iostream>
Mdfg::Mdfg(Graph * dag){
#ifdef SEQ
    std::cout<<"HEY MAIN!\n";
#endif
            for (auto node : dag->getNodes()) {
                Mdfi * instr = new Mdfi(node);
                mapNodeMdfi[node]=instr;
                if(instr->checkFirable())
                    firable.push(instr);
                else
                    repository.push_back(instr);
            }

            for(int i = 0; i < firable.size(); i++)
            {
               Mdfi * instr = firable.front();
               std::vector<Node *> depNodes = instr->dagNode->getDependant();
               for(auto &node : depNodes)
                   instr->addOuputDest(mapNodeMdfi[node]);
            }
            for(auto & instr : repository)
            {
               auto depNodes = instr->dagNode->getDependant();
               for(auto &node : depNodes)
                   instr->addOuputDest(mapNodeMdfi[node]);
            }

        }
 void Mdfg::sendToken(Mdfi * executeInstr, std::vector<int> inputs){
    auto dst =  executeInstr->outputDestination;
    if(dst.empty())
    {

//        for(auto i : inputs)
//            std::cout<<(i)<<std::endl;

    }
    else

        for(Mdfi * outDest : dst)
        {

                outDest->missingToken--;
                int pos = executeInstr->dagNode->offset_input[outDest->dagNode];
                //here?
                outDest->inputs[pos]=inputs;
                if (outDest->missingToken == 0) {
                    std::erase(repository, outDest);

                    outDest->setFirable();
                    m_firable.lock();
                    firable.push(outDest);
                    m_firable.unlock();
                    cv.notify_all();
                }
        }

}
        /*void compile()
        {
            for(auto instr : repository)
                instr.compile();
        }*/

unsigned Mdfg::countMissingInstructions()
{
    return repository.size();
}

std::vector<Mdfi *> Mdfg::getSources()
{
    std::vector<Mdfi *> sourceInstr;
    for(int i = 0; i < firable.size(); i++)
    {
        sourceInstr.emplace_back(firable.front());
    }
    return  sourceInstr;
}

Mdfi * Mdfg::getFirable()
{
    if(repository.empty() && firable.empty()) {
        computation_done = true;
        cv.notify_all();
        return nullptr;
    }

    Mdfi *instr;
    {
        std::unique_lock lc(m_firable);

         cv.wait(lc, [&] { return !firable.empty() || computation_done; });

        instr = firable.front();
        firable.pop();

    }
    return instr;

}

        
