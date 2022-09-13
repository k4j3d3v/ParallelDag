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
class Graph;

class Mdfg {
    private:

        std::unordered_map<Node *, Mdfi *> mapNodeMdfi;
#ifndef SEQ
        std::mutex m_firable, repository_m;
        std::condition_variable cv;
#endif
        bool computation_done = false;


    public:
    std::vector<Mdfi *> repository;
    std::queue<Mdfi *> firable;
        Mdfg(Graph *dag);

        void sendToken(Mdfi *executeInstr, std::vector<int> inputs);

        unsigned countMissingInstructions();
        std::vector<Mdfi *> getSources();

        Mdfi *getFirable();
};

#endif //PARALLELDAG_MDFG_H
