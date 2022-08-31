#include "include/node.h"
#include <iostream>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
#include "include/graph.h"

class Mdfi
{
    private:
        int inputToken;
        int outputToken;
    public:

        Node * dagNode;
        bool firable;
        int missingToken;

    std::vector<Mdfi *> outputDestination;

        Mdfi(Node * node)
        :dagNode(node),inputToken(node->input_arity),\
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
        bool addOuputDest(Mdfi * dest)
        {
            if (outputDestination.size() < outputToken) {
                outputDestination.emplace_back(dest);
                return true;
            }
            return false;
        }
        void setFirable()
        {
            firable=true;
        }
        bool checkFirable()
        {
            return missingToken == 0;
        }
        void run()
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
};
class Mdfg
{
    private:
        std::vector<Mdfi *> repository;
        std::queue<Mdfi *> firable;
        std::unordered_map<Node *, Mdfi *> mapNodeMdfi;
        std::mutex m_firable;
        std::condition_variable cv;

    public:

        Mdfg(Graph * dag){

            for (auto node : dag->getNodes()) {
                Mdfi * instr = new Mdfi(node);
                mapNodeMdfi[node]=instr;
                if(instr->firable)
                    firable.push(instr);
                else
                    repository.push_back(instr);
            }
           // std::cout<<"nope";
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
    void sendToken(Mdfi * executeInstr){
        auto dst =  executeInstr->outputDestination;
        for(Mdfi * outDest : dst)
        {
            {
//                std::lock_guard lc(m_firable);
                outDest->missingToken--;
                if (outDest->missingToken == 0) {
                    std::erase(repository, outDest);
                    outDest->setFirable();
                    m_firable.lock();
                    firable.push(outDest);
                    m_firable.unlock();
                    cv.notify_one();
                }
            }
        }
    }
        /*void compile()
        {
            for(auto instr : repository)
                instr.compile();
        }*/

        unsigned countMissingInstructions()
        {
            return repository.size();
        }
        Mdfi * getFirable()
        {
            if(repository.empty())
                return nullptr;

            Mdfi *instr;
                    //std::cout<<"Firable size: "<<firable.size()<<std::endl;
            {
                std::unique_lock lc(m_firable);
                //if (!firable.empty()) {
                //
                // cv.wait(lc, [&] { return !(firable.empty()); });
                while(firable.empty())
                {
                    cv.wait(lc);
                    std::cout<<"[ " << std::this_thread::get_id()<<" ]"<<"Sticked here \n";
                }
                instr = firable.front();
                firable.pop();
            }
            return instr;

                //}

           //std::cout<<"Repository before end: "<<repository.size()<< std::endl;
        }
        
        
};