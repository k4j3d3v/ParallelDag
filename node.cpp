#include <iostream>
#include <ostream>
#include <algorithm> 
#include "include/node.h"

template class Node<int>;

template<typename T>
Node<T>::Node()
{
	Node<T>(-1, 0, 0);

}
template<typename T>
Node<T>::Node(int id, int in_a, int out_a, int input)
{
	Node::id = id;
	input_arity = in_a;
	out_arity = out_a;
    Node::input_size = input;
    dependence_added = 0;
	//father = dagRef;
	std::cout<<"Node created, id: "<<Node::id<<std::endl;
}
template<typename T>
Node<T>::Node(int id, int in_a, int out_a):Node(id, in_a,out_a, 0) {}

//int Node::addDependence(Node *dp)
//{
//	std::cout<<"addDep method"<<std::endl;
//	if(dependence.size() < input_arity)
//		Node::dependence.emplace_back(dp);
//
//	return 0;
//}

template<typename T>
void Node<T>::addDependant(Node<T> *dp)
{
    if(dependant.size() < out_arity) {
        Node::dependant.emplace_back(dp);
        offset_input[dp] = dp->dependence_added++;
        std::cout<<"Offset: "<<offset_input[dp]<<std::endl;
    }
}

template<typename T>
std::vector<Node<T> *> Node<T>::getDependant()
{
////	std::vector<Node *> nodes = father->getNodes();
////	std::vector<Node *> dependant;
////	Node * me  = this;
////	std::copy_if (nodes.begin(), nodes.end(), std::back_inserter(dependant),
////		[this](Node* i)
////		{
////			std::vector<Node*> v = i->dependence;
////			return std::find(v.begin(), v.end(),this)!=v.end();
////
////		});
//
	return dependant;

}

template<typename T> void Node<T>::addCompute(toComputeFun_T to_compute)
{
	std::cout<<"addComputed invoked on "<<this->id<<std::endl;
	task = to_compute;
}

template<typename T>  typename Node<T>::toComputeFun_T Node<T>::getTask()
{
	return task;
} 
//bool operator==(const Node& lhs, const Node& rhs)
//{
//	return lhs.id==rhs.id;
//}

/*std::ostream& operator<< (std::ostream& out, const Node& node)
{
    // Since operator<< is a friend of the Point class, we can access Point's members directly.
    out << "Node(id=" << node.id<< ", in_a" << node.input_arity  << ", out_a" << node.out_arity << ')';

    return out;
}*/


