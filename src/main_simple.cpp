#include <iostream>
#include <cmath>
#include "graph.h"
#include "../utimer.cpp"


int main(int argc, char *argv[]) {


    int nw = 0;
    if (argc > 1)
        nw = atoi(argv[1]);
    std::cout << "Specified num of worker: " << nw << std::endl;
    Graph<int> g ;
    auto A = new Node<int >(1 , 0 , 2 ,  1) ;
    auto B = new Node<int >(2 , 1 , 1 ) ;
    auto C = new Node<int >(3 , 1 , 1 ) ;
    auto D = new Node<int >(4 , 2 , 0 ) ;
    A->addDependant (B ) ;
    A->addDependant (C ) ;
    B->addDependant (D) ;
    C->addDependant (D) ;
    A->addCompute ([](std::vector<int> input){ int yb, yc, x = input[0]; yb = x-1; yc = x+1; return std::vector<int> {yb,yc};} );
    B->addCompute ([](std::vector<int> input){ int zb, yb = input[0]; zb = yb* 2 ; return std::vector<int> {zb};});
    C->addCompute ( [](std::vector<int> input){ int zc, yc = input[0]; zc = yc* 3 ; return std::vector<int> {zc};});
    D->addCompute ( [](std::vector<int> input){ int zb = input[0], zc = input[1], res; res = zb+zc; return std::vector<int> {res};});

    g.addNode (A) ;
    g.addNode (B) ;
    g.addNode (C) ;
    g.addNode (D) ;

    {
#ifndef SEQ
        utimer t("PAR");
        g.setUpComp(nw);
        std::vector<int> v = g.compute(std::vector<int>{9});
        for (auto res : v) {
            std::cout<<res<< std::endl;
        }
#endif
#ifdef SEQ
        utimer t("SEQ");
        g.setUpComp();
        g.compute_seq(std::vector<int>{9});
#endif
    }




    std::cout << "Finished!\n";
    return 0;
}
