#include <iostream>
#include <cmath>
#include "include/graph.h"
#include "utimer.cpp"

#define CYCLE 100000

int main(int argc, char *argv[]) {

    /*
     *    /-> (B) \-v
     * (A)            (D)
     *  \ \-> (C) /-^  ^
     *   \             |
     *    \-> (E) --> (F)
     */
    int nw = 0;
    if (argc > 1)
        nw = atoi(argv[1]);
    std::cout << "Specified num of worker: " << nw << std::endl;

    Graph g;
    //            id, in, out
    Node A(1,0,2,&g); Node B(2,1,1,&g); Node C(3,1,1,&g); Node D(4,2,1,&g);
    //A.addDep(B); A.addDep(C); B.addDep(D); C.addDep(D);
    B.addDependence(&A);
    C.addDependence(&A);
    D.addDependence(&B);
    D.addDependence(&C);
    int x= 9, yb,yc, zb,zc, res;
    auto print = [](char id){std::cout<<"["<<std::this_thread::get_id()<<"] Runnning task "<<id<<std::endl;};
    A.addCompute([&]() {print('A');  yb = x-1; yc = x+1; });
    B.addCompute([&]() { print('B'); zb = yb*2; });
    C.addCompute([&]() { print('C');zc = yc*3; });
    D.addCompute([&]() { print('D');res = zb+zc; });

    g.addNode(&A);
    g.addNode(&B);
    g.addNode(&C);
    g.addNode(&D);

    std::cout << "Nodes before any operation: \n";
    g.printNodes();

    // Mdfg g_mdf(&g);
    {
        utimer t("Par");
        g.setUpParallelComp(nw);
        g.compute();
    }
    std::cout << "RES: " << res << std::endl;
    std::cout << "x: " << x << std::endl;
//    {
//        utimer t("SEQ");
//        g.setUpParallelComp();
//        g.compute_seq();
//    }

    std::cout << "RES: " << res << std::endl;

    std::cout << "Finished!\n";
    return 0;
}
