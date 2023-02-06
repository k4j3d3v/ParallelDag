#include <iostream>
#include <cmath>
#include "graph.h"
#include "../utimer.cpp"

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
Node<float>* appendNodesChain(int &startingId, Node<float> * startingNode, int chainLen, Graph<float> &g)
{
    std::vector<Node<float> * > dependence= {startingNode};
    for (int i=0, out_a=1; i< chainLen; i++)
    {
        auto dep = new Node<float>(startingId, 1, out_a);
        dependence.push_back(dep);
        dep->addCompute(fun);
        dependence[i]->addDependant(dep);
        g.addNode(dep);
        startingId++;
    }
    return dependence.back();
}
int main(int argc, char *argv[]) {

    int nw = 0;
    if (argc > 1)
        nw = atoi(argv[1]);
    std::cout << "Specified num of worker: " << nw << std::endl;

    Graph<float> g;
	auto *A = new Node<float>(1, 0, 11, 1);
	auto *B = new Node<float>(2, 1, 1);
	auto *C = new Node<float>(3, 1, 1);
	auto *D = new Node<float>(4, 1, 1);
	auto *E = new Node<float>(5, 1, 1);
	auto *F = new Node<float>(6, 1, 1);
	auto *G = new Node<float>(7, 1, 1);
	auto *H = new Node<float>(8, 1, 1);
	auto *I = new Node<float>(9, 1, 1);
	auto *J = new Node<float>(10, 1, 1);
	auto *K = new Node<float>(11, 1, 1);

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

    B->addCompute([](vector in) {

        float bd, be, bf, bg, ab = in[0];

        bd = be = bf = bg = ab + 1;
        for (int i = 0; i < CYCLE; i++) {
            bg = sin(sin(sin(bg)));
            be = sin(pow(ab, i) + sqrt(i * ab));

            bf = sin(pow(ab, i) + pow(i * ab, i));
            bd = cos(1 + ab) + sqrt(i + pow(ab, bf));
        }
        return vector {bd,be,bf, bg};

    });
    const int chain_len = 4;
    std::vector<Node<float> * > dependence= {B};
    int base_id = 12;
    std::vector<Node<float>*> lastChainNodes;
    lastChainNodes.push_back(appendNodesChain(base_id, B, chain_len, g));

    C->addCompute([](vector in) {
        float ch, ci, cj, ck,ac=in[0], ab=0;

        ch = ac + 1;
        for (int i = 0; i < CYCLE; i++) {

            ch = cos(cos(sin(ch))) + 5;
            ci = sin(pow(ac, i) + sqrt(i * ch));
            cj = sin(pow(i, ab) + sqrt(ci * ab));
            ck = tan(scalbln(ab, i) + sqrt(i * ab));

        }
        return vector {ch,ci,cj,ck};
    });



    lastChainNodes.push_back(appendNodesChain(base_id, C, chain_len, g));

    D->addCompute([](vector in) {
        float res = 1;
        float bd = in[0];
        for (int i = 0; i < CYCLE; i++) {
            res *= pow(bd, i);

        }
        return vector {res};

    });


    lastChainNodes.push_back(appendNodesChain(base_id, D, chain_len, g));


    E->addCompute([](vector in) {

        float res = 0;
        float be = in[0];
        for (int i = 0; i < CYCLE; i++) {
            res += cos(i + be) + be * i;
        }
        return vector {res};

    });



    lastChainNodes.push_back(appendNodesChain(base_id, E, chain_len, g));

    F->addCompute([](vector in) {
        float bf = in[0];
        bf += 1;
        for (int i = 0; i < CYCLE; i++) {
            bf *= sin(bf) + pow(bf, i) + i * bf;

        }
        return vector {bf};

    });


    lastChainNodes.push_back(appendNodesChain(base_id, F, chain_len, g));


    G->addCompute([](vector in) {
        float bg = in[0];
        float out=0;
        bg = bg + 1;
        double s = 0;
        for (int i = 0; i < CYCLE; i++) {
            s = sqrt(bg + i) + pow(bg, i + 1);
        }
        out += s;
        return vector {out};

    });


    lastChainNodes.push_back(appendNodesChain(base_id, G, chain_len, g));

    H->addCompute([](vector in) {
        float ch = in[0];
        for (int i = 0; i < CYCLE; i++) {
            ch = pow(ch, i) + sqrt(i * ch) + i;
        }
        return vector {ch};

    });

    lastChainNodes.push_back(appendNodesChain(base_id, H, chain_len, g));

    float res;
    I->addCompute([](vector in) {
        float ci = in[0], res;

        for (int i = 0; i < CYCLE; i++) {
            res = cos(cos(cos(ci))) + pow(ci, i) + sqrt(ci * i) + sin(pow(i, ci));
        }

        return vector {res};

    });


    lastChainNodes.push_back(appendNodesChain(base_id, I, chain_len, g));

    J->addCompute([](vector in) {

        float cj = in[0], res;

        for (int i = 0; i < CYCLE; i++) {
            res = cos(cos(cos(cj))) + pow(cj, i) + sqrt(cj * i) + sin(pow(i, cj));

        }
        return vector {res};


    });


    lastChainNodes.push_back(appendNodesChain(base_id, J, chain_len, g));


    K->addCompute([](vector in) {
        float ck = in[0];
        float ac = ck*3,res;
        for (int i = 0; i < CYCLE; i++) {
            res = cos(cos(cos(ck))) + cos(cos(sin(ck))) + sqrt(ac * i) + sin(pow(i * ac, ck));

        }
        return vector {res};

    });

    lastChainNodes.push_back(appendNodesChain(base_id, K, chain_len, g));

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
    std::cout<<"base id before: "<<base_id<<"\n";
    auto first_collector = new Node<float>(base_id++, lastChainNodes.size(), 7);

    std::cout<<"base id after: "<<base_id<<"\n";
    first_collector->addCompute(fun);
    g.addNode(first_collector);
    for(auto lc_n : lastChainNodes)
    {
        lc_n->addDependant(first_collector);
    }

    lastChainNodes.clear();
    for(int i= 0; i < 7; i++)
    {
        std::cout<<"base id oho: "<<base_id<<"\n";
        auto descendant = new Node<float>(base_id++, 1, 1);;
        descendant->addCompute(fun);
        first_collector->addDependant(descendant);
        g.addNode(descendant);
        lastChainNodes.push_back(appendNodesChain(base_id, descendant, chain_len, g));
    }
    auto second_collector = new Node<float>(base_id++, 7, 0);;
    second_collector->addCompute(fun);
    g.addNode(second_collector);
    for(auto &lc_n : lastChainNodes)
    {
        lc_n->addDependant(second_collector);
    }
    std::cout << "Nodes before any operation: \n";

    {
#ifndef SEQ
        utimer t("PAR");
        g.setUpComp(nw);
        g.compute(std::vector<float>{9});
#endif
#ifdef SEQ
        utimer t("SEQ");
        g.setUpComp();
        g.compute_seq(std::vector<float>{9});
#endif
    }




    std::cout << "Finished!\n";
    return 0;
}
