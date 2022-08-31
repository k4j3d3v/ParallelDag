#include "MDFG.cpp"
#include <iostream>
#include <thread>
#include <vector>

int main()
{

    /*
     *    /-> (B) \-v
     * (A)            (D)
     *  \ \-> (C) /-^  ^
     *   \             |
     *    \-> (E) --> (F)
     */
	Graph g;

    /*Node A(1,0,3,&g);
    Node B(2,1,1,&g);
	Node C(3,1,1,&g);
    Node D(4,2,1,&g);
    Node E(5,1,1,&g);
    Node F(6,1,1,&g);
*/

    Node A(1,0,3,&g);
    Node B(2,1,1,&g);
    Node C(3,1,1,&g);
    Node D(4,2,1,&g);
    Node E(5,1,1,&g);
    Node F(6,1,1,&g);

	B.addDependence(A); 
	C.addDependence(A); 
	D.addDependence(B);
	D.addDependence(C);
    E.addDependence(A);
    F.addDependence(E);
    D.addDependence(F);
	g.addNode(C);
	g.addNode(A);
	g.addNode(B); 
	g.addNode(D);
    g.addNode(E);
    g.addNode(F);

    auto s = []{std::this_thread::sleep_for(std::chrono::seconds(2));};
	A.addCompute([s]{s(); std::cout<<"Hi it's A task \n";});
	B.addCompute([s]{std::cout<<"Hi it's B task \n";s();});
	C.addCompute([s]{std::cout<<"Hi it's C task \n";s();});
	D.addCompute([s]{s();std::cout<<"Hi it's D task \n";});
    E.addCompute([s]{s();std::cout<<"Hi it's E task \n";});
    F.addCompute([s]{std::cout<<"Hi it's F task \n";s();});

	std::cout<<"Nodes before any operation: \n";
	g.printNodes();
	
    Mdfg g_mdf(&g);
    auto work = [&]{
        std::cout<<"[ " << std::this_thread::get_id()<<" ]\n";
        while (true) {
            Mdfi *f = g_mdf.getFirable();
            if(f != nullptr) {
                std::cout << "[ "<<std::this_thread::get_id()<<" ] Executing instruction dag n. " << f->dagNode->id << std::endl;
                f->run();
                g_mdf.sendToken(f);
            }
            else //if(g_mdf.countMissingInstructions() == 0)
            {
                std::cout<<"[ " << std::this_thread::get_id()<<" ]"<< " Computation finished\n";
                break;
            }
        }
    };
    unsigned const thread_count = 4;//std::thread::hardware_concurrency();
    std::cout<<"Worker num: "<<thread_count<< std::endl;
    std::vector<std::thread> threads;
    for(unsigned i=0; i < thread_count; i++)
    {
        std::cout<<"Spawned \n";
        threads.push_back(std::thread(work));
    }

    for(unsigned i=0;i<threads.size();++i)
    {
        std::cout<<"Joining thread:  "<<threads[i].get_id()<< std::endl;
        threads[i].join();
    }


/*    f = g_mdf.getFirable();
    std::cout<<"Executing instruction dag n. "<<f->dagNode->id<<std::endl;
    f->run();*/
    //std::cout<<"First node is firable? "<<f->checkFirable()<<" Node id: "<<f->dagNode->id<<std::endl;
    std::cout<<"Finished!\n";
    return 0;
}