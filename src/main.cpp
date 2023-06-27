/**
 * @mainpage CS361 - Thread Search
 * @section Description
 * 
 * Using threadsafe_queue, a threadpool and main thread this program searches for a target string given by the user within files in a directory given by the user (or the local if none is given) and outputs the line it occured on from the file.
 * 
 * Make commands:
 * 
 *  make
 * 
 * will build the binary.
 * 
 *  make run
 * 
 * will run the boat with 7 adults and 9 children
 * 
 *  make clean
 * 
 * will clear out compiled code.
 * 
 *  make doc
 * 
 * will build the doxygen files.
 */

/**
 * @file
 * @author Nicholas Pelletier
 * @date 2023
 * @section Description
 * 
 * description above. This is the main file, everything occurs here.
*/

#include <string>
#include <iostream>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <memory>
#include <fstream>
#include <functional>
#include <queue>
#include <future>
#include <filesystem>
//make things easier for me
namespace fs = std::filesystem;

//Create threadsafe_queue used from the book (Anthony Williams Concurrency in Action) and modified a bit because the book didn't check its code for bool try_pop
template<typename T>
class threadsafe_queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::unique_ptr<node> head;
    std::mutex tail_mutex;
    node* tail;
    std::condition_variable data_cond;
    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<node> pop_head() 
    {
        std::unique_ptr<node> old_head=std::move(head);
        head=std::move(old_head->next);
        return old_head;
    }
    std::unique_lock<std::mutex> wait_for_data() 
    {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock,[&]{return head.get()!=get_tail();});
        return std::move(head_lock); 
    }
    std::unique_ptr<node> wait_pop_head()
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data()); 
        return pop_head();
    }
    std::unique_ptr<node> wait_pop_head(T& value)
    {
        std::unique_lock<std::mutex> head_lock(wait_for_data()); 
        value=std::move(*head->data);
        return pop_head();
    }
    std::unique_ptr<node> try_pop_head()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if(head.get()==get_tail())
        {
            return std::unique_ptr<node>();
        }
        return pop_head();
    }
    std::unique_ptr<node> try_pop_head(T& value)
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        if(head.get()==get_tail())
        {
            return std::unique_ptr<node>();
        }
        value=std::move(*head->data);
        return pop_head();
    }
public:
    threadsafe_queue():
        head(new node),tail(head.get())
    {}
    threadsafe_queue(const threadsafe_queue& other)=delete;
    threadsafe_queue& operator=(const threadsafe_queue& other)=delete;
    std::shared_ptr<T> try_pop()
    {
        std::unique_ptr<node> old_head=try_pop_head();
        return old_head?old_head->data:std::shared_ptr<T>();
    }
    bool try_pop(T& value)
    {
        bool old_head=try_pop_head(value);
        return old_head;
    }
    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_ptr<node> const old_head=wait_pop_head();
        return old_head->data;
    }
    void wait_and_pop(T& value)
    {
        std::unique_ptr<node> const old_head=wait_pop_head(value);
    }
    void push(T new_value)
    {
        std::shared_ptr<T> new_data(
            std::make_shared<T>(std::move(new_value)));
        std::unique_ptr<node> p(new node);
        {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data=new_data;
        node* const new_tail=p.get();
        tail->next=std::move(p);
        tail=new_tail;
        }
        data_cond.notify_one();
    }
    bool empty()
    {
        std::lock_guard<std::mutex> head_lock(head_mutex);
        return (head.get()==get_tail());
    }
};
//Joiner for join_threads in thread_pool, courtesy of the book (Anthony Williams Concurrency in Action), necessary for the pool.
class join_threads
{
    std::vector<std::thread>& threads;
public:
    explicit join_threads(std::vector<std::thread>& threads_):
        threads(threads_)
    {}
    ~join_threads()
    {
        for(unsigned long i=0;i<threads.size();++i)
        {
            if(threads[i].joinable())
                threads[i].join();
        }
    }
};
//Thread Pool Class as seen in the book (Anthony Williams Concurrency in Action), put together and modified to work with this.
class thread_pool
{
    std::atomic_bool done;
    threadsafe_queue<std::function<void()> > work_queue;
    std::vector<std::thread> threads; 
    join_threads joiner; 
    void worker_thread()
    {
        while(!done) 
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
        done(false),joiner(threads)
        {
            unsigned const thread_count=std::thread::hardware_concurrency()-1; 
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
                done=true; 
                throw;
            }
        }
    ~thread_pool()
    {
    done=true; 
    }
    template<typename FunctionType>
    void submit(FunctionType f)
    {
        work_queue.push(std::function<void()>(f)); 
    }
};
/**
 * Producer is used as a thread to find all files of expected types and add to the pool.
 * 
 * @param mainP the main path, necessary for worker threads to know where to start.
 * @param pool reference to the thread pool. necessary for giving work to the threadpool.
 * @param target string reference to the target string the worker threads are given to look for.
 * @param mut the mutex used for simple I/O locking across all threads.
 * @return no return
 */
void producer(std::filesystem::path mainP,  thread_pool &pool, std::string target, std::mutex &mut);

/**
 * lookforString is a child function used as task for the threads in the pool.
 * 
 * @param file the actual file this thread is tasked with looking through.
 * @param target string reference to the target string the worker threads are given to look for.
 * @param cLock the mutex used for simple I/O locking across all threads.
 * @param numItems the number of items left within the producer that it's pushing out. thread removes 1 from numItems when it's complete.
 * @return no return
 */
void lookforString(fs::path file, std::string target, std::mutex &cLock, int &numItems);

/**
 * @brief 
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{
    fs::path startingDir;
    std::string target;
    if(argv[1]==NULL){
        std::cout<<"Please Enter a target. I.e. \"<thread>\""<<std::endl;
    }
    else{
         //Assume the user will be smart enough to use quotes correctly so whatever is in argv[1] will be the search string
        if(argc<3){
            target = argv[1];
            startingDir=fs::current_path();
        }
        else{
            target = argv[1];
            startingDir =argv[2];
            fs::current_path(startingDir);
           
        }
        //create simple locking mutex for I/O
        std::mutex cLock;
        //create the threadpool
        thread_pool pool;
        //generate the producer Thread.
        std::thread producerThread(producer, startingDir, std::ref(pool), target, std::ref(cLock));
        producerThread.join();
        cLock.lock();
        std::cout<<"!----Search Completed----!"<<std::endl;
        cLock.unlock();
        //for loop setup from coliru via cppstories do this in the producer thread
    }


    return 0;
}
//task given to threads. Looks through file and notifies via I/O when it finds a file with the given target string 
void lookForString(fs::path file, std::string target, std::mutex &cLock, int &numItems){
    int offset;
    int lineNum=0;
    std::string line;
    std::ifstream myFile;
    myFile.open(file);
    //start the search
    if(myFile.is_open()){
        while(!myFile.eof()){
            getline(myFile, line);
            //line num for giving instructions on where the target is in the file
            lineNum+=1;
            if((offset = line.find(target, 0)) !=std::string::npos)
            {
                cLock.lock();
                std::cout<<"--------------\nThread "<<std::this_thread::get_id()<<" found a match.\nFile: "<<file<<"\nLine "<<lineNum<<": "<<line<<"\n--------------"<<std::endl;
                cLock.unlock();
            }
        }
        //close after completely going through the file
        myFile.close();
    }
    //remove from number of items within the list of items to go through.
    cLock.lock();
    numItems-=1;
    cLock.unlock();
}

void producer(fs::path mainPath, thread_pool &pool, std::string target, std::mutex &cLock){
    int count=0;
    bool countZero=false;
    cLock.lock();
    std::cout<<"!---- Search Started ----!\nTarget Folder: "<<mainPath<<"\nTarget Text: "<<target<<"\nUsing a Pool of "<<std::thread::hardware_concurrency()-1<<" threads to search"<<std::endl;
    cLock.unlock();
    //recursively go through the directory and all subdirectories and if a file matches the asked for extension(s) then it will be submitted to the pool.
    for(auto iterEntry = fs::recursive_directory_iterator(fs::current_path()); iterEntry != fs::recursive_directory_iterator(); ++iterEntry ) {
            const auto filenameStr = iterEntry->path().filename().string();
            if (iterEntry->is_regular_file() && (iterEntry->path().extension()==".cc" || iterEntry->path().extension()==".c" || iterEntry->path().extension()==".cpp" || iterEntry->path().extension()==".h" 
                || iterEntry->path().extension()==".hpp" || iterEntry->path().extension()==".pl" || iterEntry->path().extension()==".sh" || iterEntry->path().extension()==".py" || iterEntry->path().extension()==".txt")) {
                cLock.lock();
                count+=1;
                cLock.unlock();
                //submit to the pool a task with the given path, the target string, the mutex and the count.
                pool.submit(std::bind(lookForString,iterEntry->path(), target, std::ref(cLock), std::ref(count)));
            }
    }
    //this makes sure that producer thread doesn't stop before the threadpool threads (it does if this isn't here.)
    while(!countZero){
        cLock.lock();
        if(count==0){
            countZero=true;
        }
        cLock.unlock();
    }   
}

