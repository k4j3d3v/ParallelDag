#include <vector>
#include "node.h"
//#include "../ThreadPool.cpp"
//#include "Mdfg.h"
class Mdfg;

class Graph{
	std::vector<Node*> nodes;

	private:
	    Mdfg *g_mdf;
		//std::vector<Node*> getIndepentNodes();
        void perThreadWork();
        unsigned thread_count;
       Node* getIndependentNode();


public:
        Graph();
		int addNode(Node*);
		void printNodes();
		void compute();
        void compute_seq();
		std::vector<Node*> getNodes();

        void setUpParallelComp(int);
};
