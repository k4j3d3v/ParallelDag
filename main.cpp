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

    Graph<int> g;
//    //            id, in, out
    Node<int> *A = new Node<int>(1, 0, 40, 1);
    Node<int> *B = new Node<int>(2, 1, 0);
    Node<int> *C = new Node<int>(3, 1, 0);
    Node<int> *D = new Node<int>(4, 1, 0);
    Node<int> *E = new Node<int>(5, 1, 0);
    Node<int> *F = new Node<int>(6, 1, 0);
    Node<int> *G = new Node<int>(7, 1, 0);
    Node<int> *H = new Node<int>(8, 1, 0);
    Node<int> *I = new Node<int>(9, 1, 0);
    Node<int> *J = new Node<int>(10, 1, 0);
    Node<int> *K = new Node<int>(11, 1, 0);

    A->addDependant(B);
    A->addDependant(C);
    A->addDependant(D);
    A->addDependant(E);
    A->addDependant(F);
    A->addDependant(G);
    A->addDependant(H);
    A->addDependant(I);
    A->addDependant(J);
    A->addDependant(K);

    using vector = std::vector<int>;

    A->addCompute([](vector in) {

        float x = in[0];
        float ab = x + 1;
        float ac = x + 1;

        for (int i = 0; i < CYCLE; i++) {
            ab = sin(sin(sin(ab + i)));
            ac = cos(cos(sin(ac))) + i;

        }
        return vector {static_cast<int>(ab),static_cast<int>(ac)};

    });
    B->addCompute([](vector in) {

        float bd, be, bf, bg, ab = in[0];

        bd = be = bf = bg = ab + 1;
        for (int i = 0; i < CYCLE; i++) {
            bg = sin(sin(sin(bg)));
            be = sin(pow(ab, i) + sqrt(i * ab));

            bf = sin(pow(ab, i) + pow(i * ab, i));
            bd = cos(1 + ab) + sqrt(i + pow(ab, bf));
        }
        return vector {static_cast<int>(bd),static_cast<int>(be),static_cast<int>(bf), static_cast<int>(bg)};

    });
    C->addCompute([](vector in) {
        float ch, ci, cj, ck,ac=in[0], ab=0;

        ch = ac + 1;
        for (int i = 0; i < CYCLE; i++) {

            ch = cos(cos(sin(ch))) + 5;
            ci = sin(pow(ac, i) + sqrt(i * ch));
            cj = sin(pow(i, ab) + sqrt(ci * ab));
            ck = tan(scalbln(ab, i) + sqrt(i * ab));

        }
        return vector {static_cast<int>(ch),static_cast<int>(ci),static_cast<int>(cj), static_cast<int>(ck)};
    });

    D->addCompute([](vector in) {
        int res = 1;
        float bd = in[0];
        for (int i = 0; i < CYCLE; i++) {
            res *= pow(bd, i);

        }
        return vector {static_cast<int>(res)};

    });

    E->addCompute([](vector in) {

        float res = 0;
        float be = in[0];
        for (int i = 0; i < CYCLE; i++) {
            res += cos(i + be) + be * i;
        }
        return vector {static_cast<int>(res)};

    });
    F->addCompute([](vector in) {
        float bf = in[0];
        bf += 1;
        for (int i = 0; i < CYCLE; i++) {
            bf *= sin(bf) + pow(bf, i) + i * bf;

        }
        return vector {static_cast<int>(bf)};

    });
    G->addCompute([](vector in) {
        float bg = in[0];
        int out=0;
        bg = bg + 1;
        double s = 0;
        for (int i = 0; i < CYCLE; i++) {
            s = sqrt(bg + i) + pow(bg, i + 1);
        }
        out += s;
        return vector {static_cast<int>(out)};

    });
    H->addCompute([](vector in) {
        float ch = in[0];
        for (int i = 0; i < CYCLE; i++) {
            ch = pow(ch, i) + sqrt(i * ch) + i;
        }
        return vector {static_cast<int>(ch)};

    });
    float res;
    I->addCompute([](vector in) {
        float ci = in[0], res;

        for (int i = 0; i < CYCLE; i++) {
            res = cos(cos(cos(ci))) + pow(ci, i) + sqrt(ci * i) + sin(pow(i, ci));
        }

        return vector {static_cast<int>(res)};

    });
    J->addCompute([](vector in) {

        float cj = in[0], res;

        for (int i = 0; i < CYCLE; i++) {
            res = cos(cos(cos(cj))) + pow(cj, i) + sqrt(cj * i) + sin(pow(i, cj));

        }
        return vector {static_cast<int>(res)};


    });
    K->addCompute([](vector in) {
        float ck = in[0];
        float ac = ck*3,res;
        for (int i = 0; i < CYCLE; i++) {
            res = cos(cos(cos(ck))) + cos(cos(sin(ck))) + sqrt(ac * i) + sin(pow(i * ac, ck));

        }
        return vector {static_cast<int>(res)};

    });


    Node<int> *n;
    for(int k = 0, s_id = 12; k< 30; k++)
    {
        n = new Node<int>(s_id+k, 1, 0);
        n->addCompute([&](vector in) {
            float ck = in[0];
            float ac = ck*3,res;

            std::cout<<"Node n. "<<(s_id+k)<< std::endl;
            for (int i = 0; i < CYCLE; i++) {
                res = cos(cos(cos(ck))) + cos(cos(sin(ck))) +  sqrt(ac*i) + sin(pow(i*ac,ck));

            }
            return vector {static_cast<int>(res)};

        });
        A->addDependant(n);
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

    {
        utimer t("Par");
        g.setUpParallelComp(nw);
#ifdef SEQ
        g.compute_seq(std::vector{9});
#endif
      g.compute(std::vector{9});
    }


    std::cout << "Finished!\n";
    return 0;
}
