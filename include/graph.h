#include <vector>
#include <thread>
#include "node.h"
#include "Mdfi.h"
#include "Mdfg.h"

//template<typename T>
//class Mdfg;
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

    void perThreadWork()
        {
            while (true) {

                Mdfi<T> *f = g_mdf->getFirable();
                if(f != nullptr) {

                    std::cout << "[ "<<std::this_thread::get_id()<<" ] Executing instruction dag n. " << f->dagNode->id << std::endl;

                    std::vector<float> flattenedInput;
                    std::cout << "[ "<<std::this_thread::get_id()<<" ]  BEFORE Flattened" << std::endl;
                    for(auto && v : f->inputs){
                        flattenedInput.insert(flattenedInput.end(), v.begin(), v.end());
                    }
                    std::cout << "[ "<<std::this_thread::get_id()<<" ]  AFTER Flattened" << std::endl;

                    auto output = f->run(flattenedInput);
                    g_mdf->sendToken(f, output);
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

        void compute(std::vector<T> sourceInput) {
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
