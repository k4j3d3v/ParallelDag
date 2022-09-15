#include <vector>
#include "node.h"
//#include "Mdfg.h"
template<typename T>
class Mdfg;


template<typename T>
class Graph{
	std::vector<Node<T> *> nodes;

	private:
	    Mdfg<T> *g_mdf;
		//std::vector<Node*> getIndepentNodes();
        void perThreadWork();
        unsigned thread_count;
        Node<T>* getIndependentNode();
        void initializeSources(const std::vector<T> &sourceInput);


public:
        Graph();
		int addNode(Node<T>*);
		void printNodes();
		void compute(std::vector<T>);
        void compute_seq(std::vector<T>);
		std::vector<Node<T> *> getNodes();
		//std::vector<std::vector<Node*>> getSortedPerLevel();
        void setUpParallelComp(int);

};
