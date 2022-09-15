#include "include/Mdfg.h"
#include "include/graph.h"
#include "utimer.cpp"
#include <iostream>

template<typename T>
Mdfg<T>::Mdfg(Graph<T> * dag){
            for (auto node : dag->getNodes()) {
                Mdfi<T> * instr = new Mdfi(node);
                mapNodeMdfi[node]=instr;
                if(instr->checkFirable())
                    firable.push(instr);
                else
                    repository.push_back(instr);
            }
            std::cout<<"Firable initial elements: \n";
            for(int i = 0; i < firable.size(); i++)
            {
               Mdfi<T> * instr = firable.front();
                std::cout<<"Instruction dag n. "<<instr->dagNode->id<<std::endl;

                std::vector<Node<T> *> depNodes = instr->dagNode->getDependant();
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
template<typename T>
void Mdfg<T>::sendToken(Mdfi<T> * executeInstr, std::vector<T> inputs){
    auto dst =  executeInstr->outputDestination;
    if(dst.empty())
    {

//        for(auto i : inputs)
//            std::cout<<(i)<<std::endl;

    }
    else

        for(Mdfi<T> * outDest : dst)
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
#ifndef SEQ
                    m_firable.lock();
#endif
                    std::cout<<"Pushing instruction n. "<<outDest->dagNode->id<<std::endl;
                    firable.push(outDest);
#ifndef SEQ
                    m_firable.unlock();
                    cv.notify_all();
#endif
                }
        }

}
        /*void compile()
        {
            for(auto instr : repository)
                instr.compile();
        }*/

template<typename T>
unsigned Mdfg<T>::countMissingInstructions()
{
    return repository.size();
}

template<typename T>
std::vector<Mdfi<T> *> Mdfg<T>::getSources()
{
    std::vector<Mdfi<T> *> sourceInstr;
    for(int i = 0; i < firable.size(); i++)
    {
        sourceInstr.emplace_back(firable.front());
    }
    return  sourceInstr;
}
template<typename T>
Mdfi<T> * Mdfg<T>::getFirable()
{
    //repository_m.lock();
    // m_firable.lock();
    bool empty_r = repository.empty();
    //repository_m.unlock();
    bool empty_f = firable.empty();
   // m_firable.unlock();
    if(empty_r && empty_f) {
        computation_done = true;
#ifndef SEQ
        cv.notify_all();
#endif
        return nullptr;
    }
    Mdfi<T> *instr = nullptr;
    {
#ifndef SEQ
        std::unique_lock lc(m_firable);

        cv.wait(lc, [&] { return !firable.empty() || computation_done; });
#endif
        if(!firable.empty()) {
            instr = firable.front();
            firable.pop();
        }
    }
    return instr;

}

        
