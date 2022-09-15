#ifndef NODE_H_
#define NODE_H_
#include <iostream>
#include <ostream>
#include <functional>
#include <vector>


template<typename T>
class Node{

    using toComputeFun_T = std::function<std::vector<T>(std::vector<T>)>;

    toComputeFun_T task;
	public:
		int id; 

		int input_arity, out_arity;
        // should be > 0, only for source/root nodes
        int input_size;
        int dependence_added;

        std::unordered_map<Node<T> *, int> offset_input;
//        std::vector<Node *> dependence;
        std::vector<Node<T> *> dependant;

        Node();
        Node(int,int,int);
        Node(int,int,int,int);

    //copy constructor
	///	Node(const Node& ){ std::cout<<"Copy constructor invoked!"<<std::endl;};
		//int addDependant(Node);
    //    int addDependence(Node*);
        void addDependant(Node<T>*);
		void addCompute(toComputeFun_T);

		std::vector<Node<T> *> getDependant();
        toComputeFun_T getTask();
		//friend std::ostream& operator<< (std::ostream& out, const Node& node);
		//friend bool operator==(const Node& lhs, const Node& rhs);


};
#endif //NODE_H_ 
