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

	private:
	    Mdfg<T> *g_mdf;
        unsigned thread_count;
        std::vector<Node<T> *> nodes;
        std::unordered_map<int, std::vector<T>> nodes_output;
    void perThreadWork()
        {
            while (true) {

                Mdfi<T> *f = g_mdf->getFirable();
                if(f != nullptr) {

                   // std::cout << "[ "<<std::this_thread::get_id()<<" ] Executing instruction dag n. " << f->dagNode->id << std::endl;

                    std::vector<T> flattenedInput;
                    //std::cout << "[ "<<std::this_thread::get_id()<<" ]  BEFORE Flattened" << std::endl;
                    for(auto && v : f->inputs){
                        flattenedInput.insert(flattenedInput.end(), v.begin(), v.end());
                    }
                   // std::cout << "[ "<<std::this_thread::get_id()<<" ]  AFTER Flattened" << std::endl;

                    auto output = f->run(flattenedInput);
                    auto res = g_mdf->sendToken(f, output);
                    if(res.size() > 0)
                        nodes_output[f->dagNode->id]=res;
                }
                else
                {
                    std::cout<<"[ " << std::this_thread::get_id()<<" ]"<< " Computation finished\n";
                    return;
                }
            }
        }
        void initializeSources(std::vector<T> const &sourceInput){
            auto sourceInstr = g_mdf->getSources();
            int i = 0;
            for(auto instr : sourceInstr) {
                std::vector<T> forInstrIn;
                for (; i < instr->inputToken; i++) {
                    forInstrIn.push_back(sourceInput[i]);
                }
                instr->inputs[0]=forInstrIn;
            }
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
    #ifdef SEQ
            std::cout<<"Sequential mode \n";
    #endif
            g_mdf = new Mdfg<T>(this);
    #ifndef SEQ
            std::cout<<"Parallel mode \n";
            if(nw > 0)
                thread_count = nw;
            else
                thread_count = std::thread::hardware_concurrency();
    #endif
        }

        std::vector<T> compute(std::vector<T> sourceInput) {
            std::cout<<"Starting computations \n";
            initializeSources(sourceInput);
            std::vector<std::thread> threads;
            for(unsigned i=0; i < thread_count; i++)
            {
                threads.push_back(std::thread(&Graph::perThreadWork, this));
            }

            for(unsigned i=0;i<threads.size();++i)
            {
                    threads[i].join();
            }
            std::vector<int> sink_nodes;
            for (unsigned i = 0; i < nodes.size(); ++i) {
                if(nodes[i]->out_arity==0)
                    sink_nodes.push_back(nodes[i]->id);
            }
            std::vector<T> results;
            for(unsigned i= 0; i < sink_nodes.size(); ++i)
            {
                auto curr_node_out = nodes_output[sink_nodes[i]];
                results.insert(end(results), begin(curr_node_out), end(curr_node_out));

            }
            return results;
        }
        void compute_seq(std::vector<T> sourceInput) {
            std::cout<<"DIO ESEGUE SEQ \n ";
            initializeSources(sourceInput);
            Mdfi<T> *f = g_mdf->getFirable();
            while (f!=nullptr) {

                std::cout << "[ "<<std::this_thread::get_id()<<" ] Executing instruction dag n. " << f->dagNode->id << std::endl;

                std::vector<T> flattenedInput;
                std::cout << "[ "<<std::this_thread::get_id()<<" ]  BEFORE Flattened" << std::endl;
                for(auto && v : f->inputs){
                    flattenedInput.insert(flattenedInput.end(), v.begin(), v.end());
                }
                std::cout << "[ "<<std::this_thread::get_id()<<" ]  AFTER Flattened" << std::endl;

                auto output = f->run(flattenedInput);
                g_mdf->sendToken(f, output);

                f = g_mdf->getFirable();
            }

        }

};
