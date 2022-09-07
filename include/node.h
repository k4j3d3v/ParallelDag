#ifndef NODE_H_
#define NODE_H_
#include <iostream>
#include <ostream>
#include <functional>
#include <vector>
//#include "graph.h"
class Graph;

class Node{
	std::function<void()> task;

	public:
		int id; 

		int input_arity, out_arity;

		Graph * father;
		std::vector<Node *> dependence;

		Node();
		Node(int,int,int,Graph*);
		//copy constructor
		Node(const Node& ){ std::cout<<"Copy constructor invoked!"<<std::endl;};
		//int addDependant(Node);
		int addDependence(Node*);
		void addCompute(std::function<void()>);

		std::vector<Node *> getDependant();
		std::function<void()> getTask();
		friend std::ostream& operator<< (std::ostream& out, const Node& node);
		friend bool operator==(const Node& lhs, const Node& rhs);


};
#endif //NODE_H_ 
