#include "include/graph.h"
#include <ios>
#include <iostream>
#include <iterator>
#include <ostream>
#include <vector>
#include <queue>
#include <algorithm> 

int Graph::addNode(Node &node)
{
	nodes.emplace_back(&node);
	std::cout<<"Node added, id: "<<node.id<<std::endl;

	return 0;
}

std::vector<Node*> Graph::getNodes()
{
	return nodes;
}
std::vector<std::vector<Node*>> Graph::getSortedPerLevel()//topologicalSort()
{
	/*

	*/

	// L will contain ordered nodes
	// triggering the nodes creation with id -1
	// Added per level abstraction (subvector of nodes).
	//std::vector<Node*> L;//(nodes.size());
	std::vector<std::vector<Node*>> L;//(nodes.size());
	// S a support "queue"
	std::vector<Node *> S = Graph::getIndepentNodes();
	//std::queue<Node*> S;
	//auto indipNodes = Graph::getIndepentNodes();
	// for (auto &elem : indipNodes) {
  	// 	S.push(std::move(elem));
	// }

	std::cout<<"Independent nodes: \n ";
	for (Node* i_n : S) {
		std::cout<<"\t Node: "<<i_n->id<<std::endl;
	}
	int numofInsertionInL= 0, i_L=0;
	while (!S.empty()) {
		std::vector<Node *> subL;
		subL.resize(S.size());
		//std::copy(std::begin(S), std::end(S), std::begin(subL));
		// Attempting to use copy back for filtering node already inserted
		
		//std::copy_if(std::begin(S), std::end(S), std::back_inserter(subL),
		//[&](Node* i)
		//{	
		//	for(std::vector<Node *>  lev : L)
		//		if(std::find(lev.begin(), lev.end(),i)!=lev.end())
		//			return false;
		//	return true;
		//});
		// More traditional way (still not working)
		bool copy_items = true;
		for(unsigned i = 0; i < L.size(); i++) // lev is each inserted subL
		{
			std::cout<<"Analyzing level "<<i<<"\n";
			std::vector<Node *> lev = L[i];
			for(Node * n : S)
			{
				std::cout<<"Checking if node n. "<<n->id<<" is in lev "<<i<<" \n";
				if(std::find(lev.begin(), lev.end(),n)!=lev.end())
				{
					std::cout<<"Found \n";
					copy_items = false;
					break;
				}
			}
		}
		if(copy_items)
		{
			std::cout<<"Doing COPY! \n";
			std::copy(std::begin(S), std::end(S), std::begin(subL));
			L.emplace_back(subL);	
			numofInsertionInL++;

		}
		Node *n =  S.back();
		S.pop_back();

		
		// n->m edges: nodes m that depends from n aka that have n in dependences
		std::vector<Node *> n_adj;
		std::copy_if (nodes.begin(), nodes.end(), std::back_inserter(n_adj), 
		[&n](Node* i)
		{	
			std::vector<Node*> v = i->dependence;
			return std::find(v.begin(), v.end(),n)!=v.end();

		});

		for (Node *m : n_adj) {

			m->dependence.erase(std::remove(m->dependence.begin(), m->dependence.end(), n), m->dependence.end());		
		
			if(m->dependence.empty())
					S.push_back(m);
		}
		std::cout<<"End of "<<i_L<<"-th iteration. \n Current independent nodes: \n";
		i_L++;
		for(auto n : Graph::getIndepentNodes())
			std::cout<<"\t Node: "<<n->id<<" \n";
	//std::queue<Node*> S;
	//auto indipNodes = Graph::getIndepentNodes();
	// for (auto &elem : indipNodes) {
  	// 	S.push(std::move(elem));
	}
	std::cout<<"Remaining edges analysis \n";
	bool remaining_edges = false;
	for(Node* n : nodes)
	{
		std::cout<<"n: "<<n->id<<" remaining dependences: \n";
		// foreach node that depends from n
		for (Node * d_n : n->dependence) {
				std::cout<<"\t "<<d_n->id<<std::endl;
			}

		if(!n->dependence.empty())
			remaining_edges=true;
	}
	if(remaining_edges)
		std::cout<<"Error: exist a cycle!";
	else
	{

		std::cout<<"L insertion count: "<<numofInsertionInL<<"\n\n";
		std::cout<<"Printing L: the ordered nodes' array \n\n";
		for(unsigned i = 0; i< L.size(); i++)
		{

			std::cout<<"Level "<<(i)<<"\n";
			for (auto node : L[i]) {
				std::cout<<"\t "<<node->id;
			}
			std::cout<<std::endl;
		}
	}	
	std::cout<<"What's happen? \n\n";
	return L;
}
std::vector<Node*> Graph::getIndepentNodes()
{
	std::vector<Node*> v;
	for (auto &n : nodes) {
		if(n->dependence.empty())	
			v.push_back(n);
	}
	return v;
}

void Graph::printNodes()
{
	for (auto i: nodes)
		std::cout << (*i) << ' ';
}

