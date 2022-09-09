#ifndef NODE_H_
#define NODE_H_
#include <iostream>
#include <ostream>
#include <functional>
#include <vector>
//#include "graph.h"
class Graph;

class Node{
    typedef std::function<std::vector<int>(std::vector<int>)> multiIntFun;

    multiIntFun task;
	public:
		int id; 

		int input_arity, out_arity;
        // should be > 0, only for source/root nodes
        int input_size;
        int dependence_added;

        std::unordered_map<Node *, int> offset_input;
        std::vector<Node *> dependence;
        std::vector<Node *> dependant;

		Node();
		Node(int,int,int);
        Node(int,int,int,int);

    //copy constructor
		Node(const Node& ){ std::cout<<"Copy constructor invoked!"<<std::endl;};
		//int addDependant(Node);
        int addDependence(Node*);
        void addDependant(Node*);
		void addCompute(multiIntFun);

		std::vector<Node *> getDependant();
        multiIntFun getTask();
		friend std::ostream& operator<< (std::ostream& out, const Node& node);
		friend bool operator==(const Node& lhs, const Node& rhs);


};
#endif //NODE_H_ 
