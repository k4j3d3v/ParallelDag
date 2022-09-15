#include "include/graph.h"
#include "include/Mdfg.h"

#include <iostream>
#include <ostream>
#include <vector>

#include <thread>

template<typename T>
Graph<T>::Graph() {

}
template<typename T>
int Graph<T>::addNode(Node<T> *node)
{
	nodes.emplace_back(node);
	std::cout<<"Node added, id: "<<node->id<<std::endl;

	return 0;
}

template<typename T>
std::vector<Node<T> *> Graph<T>::getNodes()
{
	return nodes;
}

template<typename T>
void Graph<T>::setUpParallelComp(int nw)
{
    #ifdef SEQ
        std::cout<<"Seq mode \n";
    #endif
    g_mdf = new Mdfg<T>(this);
#ifndef SEQ
    if(nw > 0)
        thread_count = nw;
    else
        thread_count = std::thread::hardware_concurrency();
#endif
}
template<typename T>
void Graph<T>::perThreadWork() {
    while (true) {

        Mdfi<T> *f = g_mdf->getFirable();
        if(f != nullptr) {

            std::cout << "[ "<<std::this_thread::get_id()<<" ] Executing instruction dag n. " << f->dagNode->id << std::endl;

            std::vector<int> flattenedInput;
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
//template<typename T>
//Node<T>* Graph<T>::getIndependentNode()
//{
//    for (auto &n : nodes) {
//        if(n->dependence.empty())
//            return n;
//            //v.push_back(n);
//    }
//    return nullptr;
//}
template<typename T>
void Graph<T>::initializeSources(vector<T> const &sourceInput)
{
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
template<typename T>
void Graph<T>::compute(std::vector<T> sourceInput) {
    std::cout<<"Starting computations \n";
    initializeSources(sourceInput);
    std::vector<std::thread> threads;
    for(unsigned i=0; i < thread_count; i++)
    {
        threads.push_back(std::thread(&Graph::perThreadWork, this));
    }

    for(unsigned i=0;i<threads.size();++i)
    {
        std::cout<<"Joining thread:  "<<threads[i].get_id()<< std::endl;
        if(threads[i].joinable())
            threads[i].join();
        else
            std::cout<<"Not Joinable thread:  "<<threads[i].get_id()<< std::endl;

    }

}
template<typename T>
void Graph<T>::compute_seq(std::vector<T> sourceInput) {
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

//template<typename T>
//void Graph<T>::printNodes()
//{
//	for (auto i: nodes)
//		std::cout << (*i) << ' ';
//}

