#include <iostream>
#include <ostream>
#include <algorithm> 
#include "include/node.h"
#include "include/graph.h"

Node::Node()
{
	Node(-1, 0, 0,nullptr);

}
Node::Node(int id, int in_a, int out_a, Graph * dagRef)
{
	Node::id = id;
	input_arity = in_a;
	out_arity = out_a;
	father = dagRef;
	std::cout<<"Node created, id: "<<Node::id<<std::endl;

}
int Node::addDependence(Node *dp)
{
	std::cout<<"addDep method"<<std::endl;
	//std::cout<<"HERE?"<<std::endl;
	if(dependence.size() < input_arity)
		Node::dependence.emplace_back(dp);
	//std::cout<<"END HERE?"<<std::endl;

	return 0;
}

std::vector<Node *> Node::getDependant()
{
	std::vector<Node *> nodes = father->getNodes(); 
	std::vector<Node *> dependant; 
	Node * me  = this;
	std::copy_if (nodes.begin(), nodes.end(), std::back_inserter(dependant), 
		[this](Node* i)
		{	
			std::vector<Node*> v = i->dependence;
			return std::find(v.begin(), v.end(),this)!=v.end();

		});

	return dependant;

}

void Node::addCompute(std::function<void()> to_compute)
{
	std::cout<<"addComputed invoked on "<<this->id<<std::endl;
	task = to_compute;
}
std::function<void()> Node::getTask()
{
	return task;
} 
bool operator==(const Node& lhs, const Node& rhs)
{ 
	return lhs.id==rhs.id;
}

std::ostream& operator<< (std::ostream& out, const Node& node)
{
    // Since operator<< is a friend of the Point class, we can access Point's members directly.
    out << "Node(id=" << node.id<< ", in_a" << node.input_arity  << ", out_a" << node.out_arity << ')';

    return out;
}


