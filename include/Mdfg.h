//
// Created by luigi on 02/09/22.
//

#ifndef PARALLELDAG_MDFG_H
#define PARALLELDAG_MDFG_H
#include "node.h"
//#include "graph.h"
#include "Mdfi.h"
#include "../ThreadPool.cpp"
#include <iostream>
#include <queue>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <unordered_map>
class Graph;

class Mdfg {
    private:
        std::vector<Mdfi *> repository;
        std::queue<Mdfi *> firable;
        std::unordered_map<Node *, Mdfi *> mapNodeMdfi;
        std::mutex m_firable;
        std::condition_variable cv;
        bool computation_done = false;
        thread_pool tp;
        void fireInstr(Mdfi *);

    public:

        Mdfg(Graph *dag);

        void sendToken(Mdfi *executeInstr);

        unsigned countMissingInstructions();

        Mdfi *getFirable();

        void start(int);

};

#endif //PARALLELDAG_MDFG_H
