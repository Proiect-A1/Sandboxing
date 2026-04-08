#ifndef MUTEX_PRIORITY_QUEUE_H
#define MUTEX_PRIORITY_QUEUE_H
#include <queue>
#include <pthread.h>
template<typename T, typename Compare = std::less<T>>
class mutex_priority_queue {
  //Max size dupa nr de elemente, ca daca dupa fac memorie o sa putem ajunge in deadlock, ex: e plina coada, un thread ia un job si dupa ce il termina vrea sa adauge unu mai mare 
  //Manager-ul sa ia in calcul si size-ul eventual al task-ului, ca sa nu ajunga in deadlock worker-thread-urile
  const long long max_size = 1e6;
  std::priority_queue<T, std::vector<T>, Compare> q;
  pthread_cond_t not_empt = PTHREAD_COND_INITIALIZER;
  pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;
  pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
  public:
  void push(const T& item);
  T pop();
  T front();
};



#endif // MUTEX_PRIORITY_QUEUE_H