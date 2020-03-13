#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <vector>
#include <functional>


class ThreadPool final{
public:
  ThreadPool(const std::size_t& size);
  ~ThreadPool();

  template<class F, class... Args>
  auto enqueue(F&& f, Args&&... args)
    ->std::future<typename std::result_of<F(Args...)>::type>;

private:
  std::vector<std::thread> workers_;
  std::queue<std::function<void()> > task_que_;
  std::condition_variable cv_;
  std::mutex task_que_mutex_;
  bool stop_=false;
};

inline ThreadPool::ThreadPool(const std::size_t& size){
  for(size_t i = 0; i < size; ++i){
    workers_.emplace_back(
                          [this](){
                            std::cout<<"worker start "<<std::this_thread::get_id()<<std::endl;
                            while(!this->stop_){
                              std::function<void()> task;
                              {
                                std::unique_lock<std::mutex> lock(this->task_que_mutex_);
                                this->cv_.wait(lock,[this](){return this->stop_ || !this->task_que_.empty();});
                                if(!this->task_que_.empty()){
                                  task = std::move(this->task_que_.front());
                                  this->task_que_.pop();
                                }
                              }
                              if(task)
                                task();
                            }
                            std::cout<<"worker end "<<std::this_thread::get_id()<<std::endl;
                          });
  }
}

inline ThreadPool::~ThreadPool(){
  {
    std::unique_lock<std::mutex> lock(task_que_mutex_);
    stop_ = true;
  }
  cv_.notify_all();

  for(auto& t : workers_)
    t.join();
}

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
  ->std::future<typename std::result_of<F(Args...)>::type>{

  using reture_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared<std::packaged_task<reture_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<reture_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(task_que_mutex_);

    if(stop_){
      throw std::runtime_error("enqueue stoped");
    }

    task_que_.emplace([task](){(*task)();});
  }

  cv_.notify_one();
  return res;
}

int func(const int& i){
  return i*i;
}

void plan(const int& n){
  ThreadPool pool(4);
  std::vector<std::future<int>> fv;
  for(int i = 0; i<=n; ++i){
    fv.emplace_back(pool.enqueue(func, i));
  }
  for(auto&& f : fv){
    std::cout<<f.get()<<std::endl;
  }
  std::cout<<"plan end"<<std::endl;
}



int main(int argc, char* argv[]){
  ThreadPool pool(4);

  std::vector<std::future<int>> fv;
  fv.emplace_back(pool.enqueue(func, 1));
  for(auto&& f : fv){
    std::cout<<f.get()<<std::endl;
  }

  std::cout<<"main end"<<std::endl;
  return 0;
}
