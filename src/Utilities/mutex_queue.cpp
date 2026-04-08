#include <queue>
#include <Utilities/mutex_queue.h>
template<typename T>
void mutex_queue<T>::push(const T& item){
  pthread_mutex_lock(&m);
  while (q.size() >= max_size){
    pthread_cond_wait(&not_full, &m);
  }
  q.push(item);
  if (q.size() == 1)
    pthread_cond_signal(&not_empt);
  pthread_mutex_unlock(&m);
}

template<typename T>
T mutex_queue<T>::pop(){
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
template<typename T>
T  mutex_queue<T>::front(){
  pthread_mutex_lock(&m);
  while (q.empty()){
    pthread_cond_wait(&not_empt, &m);
  }
  T item = q.front();
  pthread_mutex_unlock(&m);
  return item;
}