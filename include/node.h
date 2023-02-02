#ifndef NODE_H_
#define NODE_H_
#include <iostream>
#include <ostream>
#include <functional>
#include <vector>


template<typename T>
class Node{

    using toComputeFun_T = std::function<std::vector<T>(std::vector<T>)>;
    private:

        int dependence_added;
        toComputeFun_T task;

        std::vector<Node<T> *> dependant;

	public:
        int id, input_arity, out_arity;
        int input_size;
        std::unordered_map<Node<T> *, int> offset_input;


    Node(int id, int in_a, int out_a, int input)
        {
            this->id = id;
            input_arity = in_a;
            out_arity = out_a;
            input_size = input;
            dependence_added = 0;
//            std::cout<<"Node created, id: "<<Node::id<<std::endl;
        }

        Node(int id, int in_a, int out_a):Node(id, in_a,out_a, 0) {}

        Node():Node(-1, 0, 0){}

    //copy constructor
	//	Node(const Node& ){ std::cout<<"Copy constructor invoked!"<<std::endl;};

        void addDependant(Node<T> *dp)
        {
            if(dependant.size() < out_arity) {
                Node::dependant.emplace_back(dp);
                offset_input[dp] = dp->dependence_added++;
                //std::cout<<"Offset: "<<offset_input[dp]<<std::endl;
            }
        }
        std::vector<Node<T> *> getDependant() {
            return dependant;
        }
    void addCompute(toComputeFun_T to_compute)
        {
            //std::cout<<"addComputed invoked on "<<this->id<<std::endl;
            task = to_compute;
        }
        toComputeFun_T getTask()
        {
            return task;
        }
    //friend std::ostream& operator<< (std::ostream& out, const Node& node);
		//friend bool operator==(const Node& lhs, const Node& rhs);


};
#endif //NODE_H_ 
