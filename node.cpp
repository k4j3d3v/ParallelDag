#include <iostream>
#include <ostream>
#include <algorithm> 
#include "include/node.h"
#include "include/graph.h"

Node::Node()
{
	Node(-1, 0, 0);

}
Node::Node(int id, int in_a, int out_a, int input)
{
	Node::id = id;
	input_arity = in_a;
	out_arity = out_a;
    Node::input_size = input;
    dependence_added = 0;
	//father = dagRef;
	std::cout<<"Node created, id: "<<Node::id<<std::endl;

}
Node::Node(int id, int in_a, int out_a):Node(id, in_a,out_a, 0) {}
int Node::addDependence(Node *dp)
{
	std::cout<<"addDep method"<<std::endl;
	//std::cout<<"HERE?"<<std::endl;
	if(dependence.size() < input_arity)
		Node::dependence.emplace_back(dp);
	//std::cout<<"END HERE?"<<std::endl;

	return 0;
}
void Node::addDependant(Node *dp)
{
    if(dependant.size() < out_arity) {
        Node::dependant.emplace_back(dp);
        offset_input[dp] = dp->dependence_added++;
        std::cout<<"Offset: "<<offset_input[dp]<<std::endl;
    }
}

std::vector<Node *> Node::getDependant()
{
//	std::vector<Node *> nodes = father->getNodes();
//	std::vector<Node *> dependant;
//	Node * me  = this;
//	std::copy_if (nodes.begin(), nodes.end(), std::back_inserter(dependant),
//		[this](Node* i)
//		{
//			std::vector<Node*> v = i->dependence;
//			return std::find(v.begin(), v.end(),this)!=v.end();
//
//		});

	return dependant;

}

void Node::addCompute(multiIntFun to_compute)
{
	std::cout<<"addComputed invoked on "<<this->id<<std::endl;
	task = to_compute;
}

std::function<std::vector<int>(std::vector<int>)> Node::getTask()
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


