#include <vector>
#include <thread>
#include "node.h"
#include "Mdfi.h"
#include "Mdfg.h"

#ifndef SEQ
    #define NWORKER int nw
#else
    #define NWORKER
#endif

template<typename T>
class Graph{
/**
 * Implementation of a task graph.
 */
	private:
	    Mdfg<T> *mdfg;
        unsigned threadCount;
        std::vector<Node<T> *> nodes;
        std::unordered_map<int, std::vector<T>> mapNodeIdsOutput;
        std::vector<int> sinkNodes;

    void perThreadWork()
        {
        /**
         * Defines the work for the threads.
         *
         */
            while (true) {

                Mdfi<T> *f = mdfg->getFirable();
                if(f != nullptr) {

                    std::vector<T> flattenedInput;
                    for(auto && v : f->inputs){
                        flattenedInput.insert(flattenedInput.end(), v.begin(), v.end());
                    }

                    auto output = f->run(flattenedInput);
                    auto res = mdfg->sendToken(f, output);
                    if(res.size() > 0)
                        mapNodeIdsOutput[f->dagNode->id]=res;
                }
                else
                {
                    return;
                }
            }
        }
        void initializeSources(std::vector<T> const &sourceInput){
            /**
             * Initializes the graph's source/root nodes.
             */
            auto sourceInstr = mdfg->getSources();
            int i = 0;
            for(auto instr : sourceInstr) {
                std::vector<T> forInstrIn;
                for (; i < instr->inputToken; i++) {
                    forInstrIn.push_back(sourceInput[i]);
                }
                instr->inputs[0]=forInstrIn;
            }
        }
        void setSinkNodes()
        {
            for (unsigned i = 0; i < nodes.size(); ++i) {
                if(nodes[i]->outputArity == 0)
                    sinkNodes.push_back(nodes[i]->id);
            }
        }
        std::vector<T> collectResults()
        {
            std::vector<T> results;
            for(unsigned i= 0; i < sinkNodes.size(); ++i)
            {
                auto curr_node_out = mapNodeIdsOutput[sinkNodes[i]];
                results.insert(end(results), begin(curr_node_out), end(curr_node_out));

            }
            return results;
        }

public:
        Graph(){};

		void  addNode(Node<T> * node) {

            nodes.emplace_back(node);

        }

		std::vector<Node<T> *> getNodes(){
            return nodes;
        }

        void setUpComp(NWORKER)
        {
            /**
             * Sets up the computation.
             * If a n(umber) (of) w(orker) <= 0 is specified, takes the available hw concurrency.
             */
    #ifdef SEQ
            std::cout<<"Sequential mode \n";
    #endif
            mdfg = new Mdfg<T>(this);
    #ifndef SEQ
            std::cout<<"Parallel mode \n";
            if(nw > 0)
                threadCount = nw;
            else
                threadCount = std::thread::hardware_concurrency();
    #endif
        }

        std::vector<T> compute(std::vector<T> sourceInput) {
            /**
             * Start the graph computation in a parallel way and return the computed values.
             */
            std::cout<<"Starting computations \n";
            initializeSources(sourceInput);
            std::vector<std::thread> threads;
            for(unsigned i=0; i < threadCount; i++)
            {
                threads.push_back(std::thread(&Graph::perThreadWork, this));
            }

            for(unsigned i=0;i<threads.size();++i)
            {
                    threads[i].join();
            }

            setSinkNodes();
            auto results = collectResults();
            return results;
        }
        std::vector<T> compute_seq(std::vector<T> sourceInput) {
            /**
             * Start the graph computation in sequential way and return the computed values.
             */
            initializeSources(sourceInput);
            Mdfi<T> *f = mdfg->getFirable();
            while (f!=nullptr) {

                std::vector<T> flattenedInput;
                for(auto && v : f->inputs){
                    flattenedInput.insert(flattenedInput.end(), v.begin(), v.end());
                }
                auto output = f->run(flattenedInput);
                auto res = mdfg->sendToken(f, output);
                if(res.size() > 0)
                    mapNodeIdsOutput[f->dagNode->id]=res;
                f = mdfg->getFirable();
            }
            setSinkNodes();
            auto results = collectResults();
            return results;
        }

};
