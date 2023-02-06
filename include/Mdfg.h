#ifndef PARALLELDAG_MDFG_H
#define PARALLELDAG_MDFG_H
#include "node.h"
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
    /**
     * Implements a Macro Data Flow Graph.
     */
    private:

        std::unordered_map<Node<T> *, Mdfi<T> *> mapNodeMdfi;
#ifndef SEQ
        std::mutex firableMutex;
        std::mutex repositoryMutex;
        std::condition_variable cv;
#endif
        bool computationDone = false;
        std::vector<Mdfi<T>*> repository;
        std::queue<Mdfi<T> *> firable;
    public:

        Mdfg(Graph<T> *dag)
        {
            /**
             * Creates a MDFG starting from a dag.
             */
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
        std::vector<T> sendToken(Mdfi<T> *executedInstr, std::vector<T> inputs){
            /**
             * Sends the token to the dependants' instruction.
             * It contains the crucial part for thread-safe behaviours.
             */
            auto dst =  executedInstr->outputDestination;

            if(dst.empty())
            {
                return inputs;
            }
            else {
                for (Mdfi<T> *outDest: dst) {

                    outDest->missingToken--;
                    int pos = executedInstr->dagNode->offsetInput[outDest->dagNode];
                    outDest->inputs[pos] = inputs;
                    if (outDest->missingToken == 0) {
#ifndef SEQ
                        repositoryMutex.lock();
#endif
                        std::erase(repository, outDest);
#ifndef SEQ

                        repositoryMutex.unlock();
#endif
#ifndef SEQ
                        firableMutex.lock();
#endif
                        firable.push(outDest);
#ifndef SEQ
                        firableMutex.unlock();
                        cv.notify_all();
#endif
                    }
                }
                return std::vector<T>{};
            }
        }

        std::vector<Mdfi<T> *> getSources(){
            /**
             * Returns the dag's source/root nodes.
             */
            std::vector<Mdfi<T> *> sourceInstr;
            for(int i = 0; i < firable.size(); i++)
            {
                sourceInstr.emplace_back(firable.front());
            }
            return  sourceInstr;
        }
        Mdfi<T> *getFirable()
        {
            /**
             * Returns the next firable (mdf)Instruction.
             */

            bool emptyRep = repository.empty();
            bool emptyFire = firable.empty();
            if(emptyRep && emptyFire) {
                computationDone = true;
#ifndef SEQ
                cv.notify_all();
#endif
                return nullptr;
            }
            Mdfi<T> *instr = nullptr;
            {
#ifndef SEQ
                std::unique_lock lc(firableMutex);
                // while (firable.empty && !computation_done))
                // stop waiting if firable is not empty or the computation is finished
                cv.wait(lc, [&] { return !firable.empty() || computationDone; });
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
