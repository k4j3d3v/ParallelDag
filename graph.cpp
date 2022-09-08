#include "include/graph.h"
#include "include/Mdfg.h"

#include <iostream>
#include <ostream>
#include <vector>

#include <thread>
Graph::Graph() {


}
int Graph::addNode(Node *node)
{
	nodes.emplace_back(node);
	std::cout<<"Node added, id: "<<node->id<<std::endl;

	return 0;
}

std::vector<Node*> Graph::getNodes()
{
	return nodes;
}

void Graph::setUpParallelComp(int nw)
{
    g_mdf = new Mdfg(this);
    if(nw > 0)
        thread_count = nw;
    else
        thread_count = std::thread::hardware_concurrency();
}
void Graph::perThreadWork() {
    //std::cout << "[ "<<std::this_thread::get_id()<<" ] starting. " << std::endl;

    while (true) {
        Mdfi *f = g_mdf->getFirable();
        if(f != nullptr) {

            std::cout << "[ "<<std::this_thread::get_id()<<" ] Executing instruction dag n. " << f->dagNode->id << std::endl;
            f->run();
            g_mdf->sendToken(f);
        }
        else
        {
            std::cout<<"[ " << std::this_thread::get_id()<<" ]"<< " Computation finished\n";
            return;
        }
    }
}
Node* Graph::getIndependentNode()
{
    for (auto &n : nodes) {
        if(n->dependence.empty())
            return n;
            //v.push_back(n);
    }
    return nullptr;
}
void Graph::compute() {
    std::cout<<"Starting computations \n";
//    std::vector<std::thread> threads;
//    std::function<void()> thW = std::bind(&Graph::perThreadWork, this);
//    tp.submit(thW);
//
//    tp.run(thread_count);
//
//    tp.join();
    g_mdf->start(thread_count);


}
void Graph::compute_seq() {
   perThreadWork();
}
void Graph::printNodes()
{
	for (auto i: nodes)
		std::cout << (*i) << ' ';
}

