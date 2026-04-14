#include <Utilities/mutex_priority_queue.h>
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
T mutex_priority_queue<T, Compare>::front(){
  pthread_mutex_lock(&m);
  while (q.empty()){
    pthread_cond_wait(&not_empt, &m);
  }
  T item = q.top();
  pthread_mutex_unlock(&m);
  return item;
}


