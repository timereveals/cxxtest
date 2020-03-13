#include <boost/thread/thread.hpp>
// #include <thread>
#include <boost/lockfree/queue.hpp>
// #include <atomic>
#include <boost/atomic.hpp>
#include <iostream>

boost::atomic_int produced_count(0);
boost::atomic_int consumed_count(0);

boost::lockfree::queue<int, boost::lockfree::capacity<64>> queue;

boost::atomic_bool done(false);

const int n = 1000000;
const int producer_thread_count = 4;
const int consumer_thread_count = 4;

void producer(void){
  for(int i = 0; i < n; ++i){
    int value = ++produced_count;
    while(!queue.push(value));
  }
}

void consumer(void){
  int value;
  while(!done){
    while(queue.pop(value))
      ++consumed_count;
  }
  while(queue.pop(value))
    ++consumed_count;
}

int main(int argc, char* argv[]){
  // std::cout<<"boost::lockfree::queue lockfree: "<<queue.is_lock_free()<<std::endl;
  std::cout<<queue.is_lock_free()<<std::endl;
  boost::thread_group producer_threads, consumer_threads;
  for(int i=0;i<producer_thread_count; ++i){
    producer_threads.create_thread(producer);
  }
  for(int i=0;i<consumer_thread_count;++i){
    consumer_threads.create_thread(consumer);
  }
  producer_threads.join_all();
  done = true;
  consumer_threads.join_all();
  std::cout<<"produced: "<<produced_count<<std::endl;
  std::cout << "consumed: " << consumed_count << std::endl;

  return 0;
}


