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
  bool empty();
  int size();
  void push(const T& item);
  T pop();
  T top();
};


template<typename T, typename Compare>
void mutex_priority_queue<T, Compare>::push(const T& item){
  pthread_mutex_lock(&m);
  while (q.size() >= max_size){
    pthread_cond_wait(&not_full, &m);
  }
  q.push(item);
  if (q.size() == 1)
    pthread_cond_signal(&not_empt);
  pthread_mutex_unlock(&m);
}


template<typename T, typename Compare>
T mutex_priority_queue<T, Compare>::pop(){
  pthread_mutex_lock(&m);
  while (q.empty()){
    pthread_cond_wait(&not_empt, &m);
  }
  T item = q.top();
  q.pop();
  if (q.size() == max_size - 1)
    pthread_cond_signal(&not_full);
  pthread_mutex_unlock(&m);
  return item;
}

template<typename T, typename Compare>
T mutex_priority_queue<T, Compare>::top(){
  pthread_mutex_lock(&m);
  while (q.empty()){
    pthread_cond_wait(&not_empt, &m);
  }
  T item = q.top();
  pthread_mutex_unlock(&m);
  return item;
}



template<typename T, typename Compare>
bool mutex_priority_queue<T, Compare>::empty(){
  pthread_mutex_lock(&m);
  bool is_empty = q.empty();
  pthread_mutex_unlock(&m);
  return is_empty;
}

template<typename T, typename Compare>
int mutex_priority_queue<T, Compare>::size(){
  pthread_mutex_lock(&m);
  int size = q.size();
  pthread_mutex_unlock(&m);
  return size;
}


#endif // MUTEX_PRIORITY_QUEUE_H