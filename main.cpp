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
    //            id, in, out, input_tot
    Node A(1,0,2,1);
    Node B(2,1,1);
    Node C(3,1,1);
    Node D(4,2,0);
    A.addDependant(&B);
    A.addDependant(&C);
    B.addDependant(&D);
    C.addDependant(&D);
//    B.addDependence(&A);
//    C.addDependence(&A);
//    D.addDependence(&B);
//    D.addDependence(&C);
    auto print = [](char id){std::cout<<"["<<std::this_thread::get_id()<<"] Runnning task "<<id<<std::endl;};
    A.addCompute([&print](std::vector<int> v) {print('A'); int x = v[0]; int yb = x-1; int yc = x+1;
        return std::vector{yb, yc} ;});
    B.addCompute([&print](std::vector<int> v) { print('B'); int yb = v[0]; int zb = yb*2;
        return std::vector{zb} ; });
    C.addCompute([&print](std::vector<int> v) { print('C'); int yc = v[1]; int zc = yc*3;
        return std::vector{zc} ; });
    D.addCompute([&print](std::vector<int> v) { print('D');int zb = v[0], zc = v[1]; int res = zb+zc;
        return std::vector{res} ;});

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
        g.compute(std::vector{9});
    }
//    std::cout << "RES: " << res << std::endl;
//    std::cout << "x: " << x << std::endl;
//    {
//        utimer t("SEQ");
//        g.setUpParallelComp();
//        g.compute_seq();
//    }


    std::cout << "Finished!\n";
    return 0;
}
