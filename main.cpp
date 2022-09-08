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
//    //            id, in, out
    Node *A = new Node(1, 0, 40, &g);
    Node *B = new Node(2, 1, 0, &g);
    Node *C = new Node(3, 1, 0, &g);
    Node *D = new Node(4, 1, 0, &g);
    Node *E = new Node(5, 1, 0, &g);
    Node *F = new Node(6, 1, 0, &g);
    Node *G = new Node(7, 1, 0, &g);
    Node *H = new Node(8, 1, 0, &g);
    Node *I = new Node(9, 1, 0, &g);
    Node *J = new Node(10, 1, 0, &g);
    Node *K = new Node(11, 1, 0, &g);


    B->addDependence(A);

    C->addDependence(A);

    D->addDependence(A);

    E->addDependence(A);

    F->addDependence(A);

    G->addDependence(A);


    H->addDependence(A);

    I->addDependence(A);

    J->addDependence(A);

    K->addDependence(A);


    float x = 1269.45, ab, ac;
    A->addCompute([&]() {
        utimer t("A task: ");
        ab = x + 1;
        ac = x + 1;

        for (int i = 0; i < CYCLE; i++) {
            ab = sin(sin(sin(ab + i)));
            ac = cos(cos(sin(ac))) + i;

        }


    });
    float bd, be, bf, bg;
    B->addCompute([&]() {


        bd = be = bf = bg = ab + 1;
        for (int i = 0; i < CYCLE; i++) {
            bg = sin(sin(sin(bg)));
            be = sin(pow(ab, i) + sqrt(i * ab));

            bf = sin(pow(ab, i) + pow(i * ab, i));
            bd = cos(1 + ab) + sqrt(i + pow(ab, bf));
        }
    });
    float ch, ci, cj, ck;
    C->addCompute([&]() {

        ch = ac + 1;
        for (int i = 0; i < CYCLE; i++) {

            ch = cos(cos(sin(ch))) + 5;
            ci = sin(pow(ac, i) + sqrt(i * ch));
            cj = sin(pow(i, ab) + sqrt(ci * ab));
            ck = tan(scalbln(ab, i) + sqrt(i * ab));

        }

    });
    D->addCompute([&]() {
        int res = 1;
        for (int i = 0; i < CYCLE; i++) {
            res *= pow(bd, i);

        }

    });

    E->addCompute([&]() {


        float res = 0;
        for (int i = 0; i < CYCLE; i++) {
            res += cos(i + be) + be * i;

        }
    });
    F->addCompute([&]() {

        bf += 1;
        for (int i = 0; i < CYCLE; i++) {
            bf *= sin(bf) + pow(bf, i) + i * bf;

        }

    });
    int out;
    G->addCompute([&]() {

        bg = bg + 1;
        double s = 0;
        for (int i = 0; i < CYCLE; i++) {
            s = sqrt(bg + i) + pow(bg, i + 1);
        }
        out += s;
    });
    double out_2 = 1;
    H->addCompute([&]() {

        out_2 = ch;
        for (int i = 0; i < CYCLE; i++) {
            ch = pow(ch, i) + sqrt(i * ch) + i;
        }
    });
    float res;
    I->addCompute([&]() {


        for (int i = 0; i < CYCLE; i++) {
            res = cos(cos(cos(ci))) + pow(ci, i) + sqrt(ci * i) + sin(pow(i, ci));

        }

    });
    J->addCompute([&]() {


        for (int i = 0; i < CYCLE; i++) {
            res = cos(cos(cos(cj))) + pow(cj, i) + sqrt(cj * i) + sin(pow(i, cj));

        }

    });
    K->addCompute([&]() {


        for (int i = 0; i < CYCLE; i++) {
            res = cos(cos(cos(ck))) + cos(cos(sin(ck))) + sqrt(ac * i) + sin(pow(i * ac, ck));

        }

    });


    Node *n;
    for(int k = 0, s_id = 12; k< 30; k++)
    {
        n = new Node(s_id+k, 1, 0, &g );
        n->addCompute([&]() {


            std::cout<<"Node n. "<<(s_id+k)<< std::endl;
            for (int i = 0; i < CYCLE; i++) {
                res = cos(cos(cos(ck))) + cos(cos(sin(ck))) +  sqrt(ac*i) + sin(pow(i*ac,ck));

            }

        });
        n->addDependence(A);
        g.addNode(n);

    }


    g.addNode(A);
    g.addNode(B);
    g.addNode(C);
    g.addNode(D);
    g.addNode(E);
    g.addNode(F);
    g.addNode(G);
    g.addNode(H);
    g.addNode(I);
    g.addNode(J);
    g.addNode(K);


    std::cout << "Nodes before any operation: \n";
    g.printNodes();
//    Node A(1,0,2,&g); Node B(2,1,1,&g); Node C(3,1,1,&g); Node D(4,2,1,&g);
//    //A.addDep(B); A.addDep(C); B.addDep(D); C.addDep(D);
//    B.addDependence(&A);
//    C.addDependence(&A);
//    D.addDependence(&B);
//    D.addDependence(&C);
//    int x= 9, yb,yc, zb,zc, res;
//    auto print = [](char id){std::cout<<"["<<std::this_thread::get_id()<<"] Runnning task "<<id<<std::endl;};
//    A.addCompute([&]() {print('A');  yb = x-1; yc = x+1; });
//    B.addCompute([&]() { print('B'); zb = yb*2; });
//    C.addCompute([&]() { print('C');zc = yc*3; });
//    D.addCompute([&]() { print('D');res = zb+zc; });
//
//    g.addNode(&A);
//    g.addNode(&B);
//    g.addNode(&C);
//    g.addNode(&D);
    {
        utimer t("Par");
        g.setUpParallelComp(nw);
        g.compute();
    }


    std::cout << "RES: " << res << std::endl;

    std::cout << "Finished!\n";
    return 0;
}
