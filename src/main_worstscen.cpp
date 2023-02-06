#include <iostream>
#include <cmath>
#include "graph.h"
//#include "graph.cpp"
#include "../utimer.cpp"
//#define SEQ

#define CYCLE 100000

auto fun = [](std::vector<float> in) {

    float bd = in[0], be = in[1], bf = in[2], bg = in[3];
    for (int i = 0; i < CYCLE; i++) {
        bg = sin(sin(sin(bg)));
        be = sin(pow(be, i) + sqrt(i * bg));

        bf = sin(pow(be, i) + pow(i * bg, i));
        bd = cos(1 + bf) + sqrt(i + pow(bd, bf));
    }
    return std::vector<float> {bd,be,bf, bg};

};
void appendNodesChain(int &startingId, Node<float> * startingNode, int chainLen, Graph<float> &g)
{
    std::vector<Node<float> * > dependence= {startingNode};
    for (int i=0, out_a=1; i< chainLen; i++)
    {
        if(i == chainLen - 1)
            out_a =0;
        auto dep = new Node<float>(startingId, 1, out_a);
        dependence.push_back(dep);
        dep->addCompute(fun);
        dependence[i]->addDependant(dep);
        g.addNode(dep);
        startingId++;
    }
}
int main(int argc, char *argv[]) {


    int nw = 0;
    if (argc > 1)
        nw = atoi(argv[1]);
    std::cout << "Specified num of worker: " << nw << std::endl;

    Graph<float> g;
	auto *A = new Node<float>(1, 0, 1, 1);
    using vector = std::vector<float>;

    A->addCompute([](vector in) {

        float x = in[0];
        float ab = x + 1;
        float ac = x + 1;

        for (int i = 0; i < CYCLE; i++) {
            ab = sin(sin(sin(ab + i)));
            ac = cos(cos(sin(ac))) + i;

        }
        return vector {ab,ac};

    });
    g.addNode(A);

    int id = 2;
    appendNodesChain(id, A, 10000, g);

    std::vector<float> v;
    {
#ifndef SEQ
        utimer t("PAR");
        g.setUpComp(nw);
        v = g.compute(std::vector<float>{9});
#endif
#ifdef SEQ
        utimer t("SEQ");
        g.setUpComp();
        v = g.compute_seq(std::vector<float>{9});
#endif
    }

    for (auto res : v) {
        std::cout<<res<< std::endl;
    }




    std::cout << "Finished!\n";
    return 0;
}
