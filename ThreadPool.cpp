#include <iostream>
#include <mutex>
#include <thread>
#include <functional>
#include <vector>
#include <atomic>
#include <latch>
#include "ThreadSafeQueue.cpp"
#include "JoinThreads.cpp"

class thread_pool
{
    std::atomic_bool done;
    threadsafe_queue<std::function<void()>> work_queue;

    unsigned thread_count = std::thread::hardware_concurrency();
    std::vector<std::thread> threads;
    //join_threads joiner;
    std::mutex pm;
    //std::chrono::system_clock::time_point start; 
    void worker_thread()
    {
        std::cout << "[ "<<std::this_thread::get_id()<<" ] Executing instruction"<<std::endl;
        while(!work_queue.empty())
        {
            std::function<void()> task;
            if(work_queue.try_pop(task))
            {
               
                task();
            }
            else
            {

                std::this_thread::yield();
            }
        }
    }
    public:
        thread_pool():
            done(false)//,joiner(threads,start)
        {

                //std::cout<<"THREAD POOL CREATED \n";
        }
        ~thread_pool()
        {
            done=true;
        }
        void run(int nw)
        {

            thread_count = nw;
            //start = std::chrono::system_clock::now();
             try
            {
                for(unsigned i=0;i<thread_count;++i)
                {
                threads.push_back(
                    std::thread(&thread_pool::worker_thread,this));
                }
            }
            catch(...)
            {
                std::cout<<"Exception happened \n";
                done=true;

                throw;
            }


        }
    
        void join()
        {

            while(!threads.empty())
    		{
                std::thread * t = &threads.back();
    			t->join();
                threads.pop_back();
                    
    		}

        }
        template<typename FunctionType>
        void submit(FunctionType f)

        {
            std::cout<<"SUBMIT! \n";
            work_queue.push(std::function<void()>(f));
    
        }
};
