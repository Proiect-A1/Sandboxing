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
  void push(const T& item);
  T pop();
  T front();
};

#endif // MUTEX_QUEUE_H
