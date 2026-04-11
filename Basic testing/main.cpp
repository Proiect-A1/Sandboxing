#include <Tasks/evaluator_task.h>
#include <Tasks/stdio_grader_task.h>
#include <iostream>
#include <Utilities/general_utilities.h>
#include <Singletoni/problem_manager.h>
#include <Singletoni/submission_manager.h>
#include <vector>
#include <Singletoni/user_queue.h>
#include <Singletoni/task_queue.h>
#include <pthread.h>
using namespace std;

#define handle_error(ret_code , ...) { fprintf(stderr , __VA_ARGS__); exit(ret_code); }
const int num_of_threads = 2;


struct worker_thread_struct{
  pthread_t thread_id;
  task* current_task;
};

void * worker_thread(void *arg){
  struct worker_thread_struct *wts = (struct worker_thread_struct *) arg;
  pthread_t thread_id = wts->thread_id;
  task* current_task = wts->current_task;

  int user_id = user_queue::get_instance().pop();
  current_task->print_log(thread_id, user_id, "Starting task execution");fflush(stdout);
  current_task->execute(thread_id, user_id);
  current_task->print_log(thread_id, user_id, "Finished task execution");fflush(stdout);
  user_queue::get_instance().push(user_id);

  delete current_task;
  delete wts;
  return nullptr;
}

void * main_thread(void *arg)
{
    int thread_count = *((int *) arg);
    //campeaza coada
    while (true){
      task* next_task = task_queue::get_instance().pop();

      worker_thread_struct* wts = new worker_thread_struct;
      wts->current_task = next_task;

      if(pthread_create(&(wts->thread_id), nullptr, worker_thread , (void *) wts) != 0) handle_error(1 , "pthread_create()");
      if(pthread_detach(wts->thread_id) != 0) handle_error(1 , "pthread_detach()");
    }

    return nullptr;
}







void create_threads()
{
    pthread_t *thread_ids = (pthread_t *) malloc(num_of_threads * sizeof(pthread_t));
    int *thread_counts = (int *) malloc(num_of_threads * sizeof(int));

    for(int i = 0 ; i < num_of_threads ; i++)
    {
        thread_counts[i] = i;
        if(pthread_create(&thread_ids[i] , nullptr , main_thread , (void *) &thread_counts[i]) != 0) handle_error(1 , "pthread_create()");
        if(pthread_detach(thread_ids[i]) != 0) handle_error(1 , "pthread_detach()");
    }
}

int main(){

  problem_manager& pm = problem_manager::get_instance();
  submission_manager& sm = submission_manager::get_instance();

  problem_metadata new_problem;
  new_problem.problem_id = "Problem";
  new_problem.rev_id = 0;
  new_problem.group_count = 10;
  new_problem.test_count = 10;
  new_problem.time_limit = 10000;
  new_problem.memory_limit = 1024ll * 1024 * 1024 * 15; // 15 GB
  new_problem.total_points = 100.0;
  new_problem.problem_status = problem_status_enum::DONE;
  new_problem.groups = std::vector<group_metadata>(10, {10.0, 1, group_type_enum::GROUP_MIN});
  new_problem.tests = std::vector<test_metadata>(10, {"", "", "", {}});
  for (int i = 0; i < 10; i++){
    new_problem.tests[i].groups.push_back(i);
  }

  pm.add_revision(new_problem);

  create_threads();

  
  for (int i = 1; i <= 10; i++){
    architecture_utilities::change_dir_to_user(i);
    system("rm -f *");
    user_queue::get_instance().push(i);
  }
  
  // cout << "sa inceapa aventura" << endl;
  // system ("sleep 5");

  sm.insert("ANDREI", language_enum::CPP, "Problem", 0, 1);

  submission_data submission = sm.get_submission("ANDREI");


  evaluator_task* eva = new evaluator_task("ANDREI", "Problem", 0);

  task_queue::get_instance().push(eva);
  // sleep(20);
  while (1){
    sleep(5);
  }

  return 0;
}
