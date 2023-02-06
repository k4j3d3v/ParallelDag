#ifndef NODE_H_
#define NODE_H_
#include <iostream>
#include <ostream>
#include <functional>
#include <vector>


template<typename T>
class Node{
    /**
     * Implementation of a graph node.
     *
     * Here are all defined the properties for the graph relations, task to compute and so on.
     */

    using toComputeFun_T = std::function<std::vector<T>(std::vector<T>)>;
    private:

        int dependantNum;
        toComputeFun_T task;

        std::vector<Node<T> *> dependant;

	public:
        int id, inputArity, outputArity;
        // only for the root nodes
        int inputSize;
        std::unordered_map<Node<T> *, int> offsetInput;


    Node(int id, int inArity, int outArity, int input)
        {
        /**
         * @id Node's id
         * @inArity num of dependences
         * @outArity num of dependants
         * @input how many values a root node must read in input (for the computation). Only for the roots.
         *
         */
            this->id = id;
            inputArity = inArity;
            outputArity = outArity;
            inputSize = input;
            dependantNum = 0;
        }

        Node(int id, int in_a, int out_a):Node(id, in_a,out_a, 0) {}

        Node():Node(-1, 0, 0){}


        void addDependant(Node<T> *dp)
        {
            if(dependant.size() < outputArity) {
                Node::dependant.emplace_back(dp);
                offsetInput[dp] = dp->dependantNum++;
            }
        }
        std::vector<Node<T> *> getDependant() {
            return dependant;
        }
    void addCompute(toComputeFun_T to_compute)
        {
            task = to_compute;
        }
        toComputeFun_T getTask()
        {
            return task;
        }


};
#endif //NODE_H_ 
