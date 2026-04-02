#ifndef MUTEX_QUEUE_H
#define MUTEX_QUEUE_H
#include <queue>
#include <pthread.h>
template<typename T>
class mutex_queue {
  //Max size dupa nr de elemente, ca daca dupa fac memorie o sa putem ajunge in deadlock, ex: e plina coada, un thread ia un job si dupa ce il termina vrea sa adauge unu mai mare 
  const long long max_size = 1e6;
  std::queue<T> q;
  pthread_cond_t not_empt = PTHREAD_COND_INITIALIZER;
  pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
  pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
  public:
  void push(const T& item){
    pthread_mutex_lock(&m);
    while (q.size() >= max_size){
      pthread_cond_wait(&not_full, &m);
    }
    q.push(item);
    if (q.size() == 1)
      pthread_cond_signal(&not_empt);
    pthread_mutex_unlock(&m);
  }
  T pop(){
    pthread_mutex_lock(&m);
    while (q.empty()){
      pthread_cond_wait(&not_empt, &m);
    }
    T item = q.front();
    q.pop();
    if (q.size() == max_size - 1)
      pthread_cond_signal(&not_full);
    pthread_mutex_unlock(&m);
    return item;
  }
  T front(){
    pthread_mutex_lock(&m);
    while (q.empty()){
      pthread_cond_wait(&not_empt, &m);
    }
    T item = q.front();
    pthread_mutex_unlock(&m);
    return item;
  }
};



#endif // MUTEX_QUEUE_H