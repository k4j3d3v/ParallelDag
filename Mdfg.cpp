#include "include/Mdfg.h"
#include "include/graph.h"
#include "utimer.cpp"
#include <iostream>
Mdfg::Mdfg(Graph * dag){
            for (auto node : dag->getNodes()) {
                Mdfi * instr = new Mdfi(node);
                mapNodeMdfi[node]=instr;
                if(instr->checkFirable())
                    firable.push(instr);
                else
                    repository.push_back(instr);
            }
            std::cout<<"Firable initial elements: \n";
            for(int i = 0; i < firable.size(); i++)
            {
               Mdfi * instr = firable.front();
                std::cout<<"Instruction dag n. "<<instr->dagNode->id<<std::endl;

                std::vector<Node *> depNodes = instr->dagNode->getDependant();
               for(auto &node : depNodes)
                   instr->addOuputDest(mapNodeMdfi[node]);
            }
            std::cout<<"Repository initial elements: \n";

            for(auto & instr : repository)
            {
                std::cout<<"Instruction dag n. "<<instr->dagNode->id<<std::endl;
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
                 //   repository_m.lock();
                    std::erase(repository, outDest);
                  //  repository_m.unlock();
                    outDest->setFirable();
                    m_firable.lock();
                    std::cout<<"Pushing instruction n. "<<outDest->dagNode->id<<std::endl;
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
    //repository_m.lock();
    // m_firable.lock();
    bool empty_r = repository.empty();
    //repository_m.unlock();
    bool empty_f = firable.empty();
   // m_firable.unlock();
    if(empty_r && empty_f) {
        computation_done = true;
        cv.notify_all();
        return nullptr;
    }
    Mdfi *instr = nullptr;
    {
        std::unique_lock lc(m_firable);

         cv.wait(lc, [&] { return !firable.empty() || computation_done; });
        if(!firable.empty()) {
            instr = firable.front();
            firable.pop();
        }
    }
    return instr;

}

        
