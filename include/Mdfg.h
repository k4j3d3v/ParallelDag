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
        std::mutex m_firable, repository_m;
        std::condition_variable cv;
#endif
        bool computation_done = false;


    public:
    std::vector<Mdfi<T>*> repository;
    std::queue<Mdfi<T> *> firable;
        Mdfg(Graph<T> *dag);

        void sendToken(Mdfi<T> *executeInstr, std::vector<T> inputs);

        unsigned countMissingInstructions();
        std::vector<Mdfi<T> *> getSources();

        Mdfi<T> *getFirable();
};

#endif //PARALLELDAG_MDFG_H
