//
// Created by luigi on 02/09/22.
//

#ifndef PARALLELDAG_MDFG_H
#define PARALLELDAG_MDFG_H
#include "node.h"
//#include "graph.h"
#include "Mdfi.h"
#include <iostream>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
template<typename T>
class Graph;

template<typename T>
class Mdfg{
    private:

        std::unordered_map<Node<T> *, Mdfi<T> *> mapNodeMdfi;
#ifndef SEQ
        std::mutex m_firable;
        std::mutex repository_m;
        std::condition_variable cv;
#endif
        bool computation_done = false;
        std::vector<Mdfi<T>*> repository;
        std::queue<Mdfi<T> *> firable;
    public:

        Mdfg(Graph<T> *dag)
        {
            for (auto node : dag->getNodes()) {
                Mdfi<T> * instr = new Mdfi(node);
                mapNodeMdfi[node]=instr;
                if(instr->checkFirable())
                    firable.push(instr);
                else
                    repository.push_back(instr);
            }
            for(int i = 0; i < firable.size(); i++)
            {
                Mdfi<T> * instr = firable.front();

                std::vector<Node<T> *> depNodes = instr->dagNode->getDependant();
                for(auto &node : depNodes)
                    instr->addOutputDest(mapNodeMdfi[node]);
            }
            for(auto & instr : repository)
            {
                auto depNodes = instr->dagNode->getDependant();
                for(auto &node : depNodes)
                    instr->addOutputDest(mapNodeMdfi[node]);
            }

        }
        std::vector<T> sendToken(Mdfi<T> *executeInstr, std::vector<T> inputs){
            auto dst =  executeInstr->outputDestination;

            if(dst.empty())
            {

//                for(auto i : inputs)
//                    std::cout<<(i)<<std::endl;
                return inputs;
            }
            else {
                for (Mdfi<T> *outDest: dst) {

                    outDest->missingToken--;
                    int pos = executeInstr->dagNode->offset_input[outDest->dagNode];
                    //here?
                    outDest->inputs[pos] = inputs;
                    if (outDest->missingToken == 0) {
#ifndef SEQ
                        repository_m.lock();
#endif
                        std::erase(repository, outDest);
#ifndef SEQ

                        repository_m.unlock();
#endif
                        // outDest->setFirable();
#ifndef SEQ
                        m_firable.lock();
#endif
                        //std::cout<<"Pushing instruction n. "<<outDest->dagNode->id<<std::endl;
                        firable.push(outDest);
#ifndef SEQ
                        m_firable.unlock();
                        cv.notify_all();
#endif
                    }
                }
                return std::vector<T>{};
            }
        }
//        unsigned countMissingInstructions(){
//            return repository.size();
//        }
        std::vector<Mdfi<T> *> getSources(){
            std::vector<Mdfi<T> *> sourceInstr;
            for(int i = 0; i < firable.size(); i++)
            {
                sourceInstr.emplace_back(firable.front());
            }
            return  sourceInstr;
        }
        Mdfi<T> *getFirable()
        {

            bool empty_r = repository.empty();
            bool empty_f = firable.empty();
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
                // wait until the firable is no more empty or the computation is finished
                // while (firable.empty && !computation_done))
                // esci se firable non è vuota o la computazione è finita
                cv.wait(lc, [&] { return !firable.empty() || computation_done; });
#endif
                if(!firable.empty()) {
                    instr = firable.front();
                    firable.pop();
                }
            }
            return instr;

        }
};

#endif //PARALLELDAG_MDFG_H
