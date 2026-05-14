#include <Tasks/evaluator_task.h> 
#include <Tasks/download.h>

result_enum evaluator_task::execute(pthread_t thread_id, int user_id) {
  if (user_id <= 0) {
    LOG_ERROR_USER(user_id, "Invalid user ID");
    return result_enum::FAIL;
  }

    LOG_DEBUG_USER(user_id , "executinv evaluator task");
  
  result_enum result;
  problem_manager& pm = problem_manager::get_instance();

  if(pm.exists_revision(problem_id , rev_id) == 0)
  {
      problem_metadata meta;
      meta.founding_submission_id = submission_id;
      meta.problem_id = problem_id;
      meta.rev_id = rev_id;
      //add the rem things in generator or whatever
      meta.problem_status = problem_status_enum::NOT_EXISTS;
      pm.add_revision(meta); 

      submission_manager &sm = submission_manager::get_instance();
    //  IO::pull_problem_request(problem_id , rev_id , sm.get_submission(submission_id).socket_fd);
     download_task *dt = new download_task(sm.get_submission(submission_id).download_link , problem_id , rev_id); 
      dt -> priority = 1000; //prioritate mare
      task_queue::get_instance().push(dt);

      evaluator_task *ev = new evaluator_task(submission_id , problem_id , rev_id);
      ev -> priority++;
      task_queue::get_instance().push(ev);
      LOG_DEBUG_USER(user_id , "problem needs download");
      return result_enum::NONE;
  }
  else if(pm.get_problem_status(problem_id , rev_id) != problem_status_enum::DONE)
  {
      //sleep(2);
      evaluator_task *ev = new evaluator_task(submission_id , problem_id , rev_id);
      ev -> priority++; //ar trebui pe puteri de 2 
      task_queue::get_instance().push(ev);
      LOG_DEBUG_USER(user_id , "still not done");
      return result_enum::NONE;
  } 

  LOG_DEBUG("problem downloaded");
  //return result_enum::OTHER; //cand e gata scoate asta
  //LOG_DEBUG("problem exists");
  //return result_enum::OK;
  auto compiler = stdio_compiler_factory(
    language_enum::CPP,
    submission_id,
    0 // priority
  );
  if (!compiler) {
    LOG_ERROR_USER(user_id, "Failed to create compiler task");
    return result_enum::FAIL;
  }
  result = compiler->execute(thread_id, user_id);
  if (result != result_enum::OK) {
    LOG_INFO_USER(user_id, "Compilation failed on submission " + submission_id + " with result " + general_utilities::enum_to_string(result));
    submission_manager::get_instance().set_verdict(submission_id, result_enum::CPE, 0, 0.0, 0);
    return result;
  }


  problem_metadata problem = pm.get_metadata(problem_id, rev_id);
  for (int test_id = 0; test_id < problem.test_count; test_id++) {
      task_queue::get_instance().push(new stdio_grader_task(
      submission_id,
      problem_id,
      rev_id,
      test_id
    ));
  }

  LOG_INFO_USER(user_id, "Evaluation tasks for submission " + submission_id + " have been created and added to the queue");
  return result_enum::OK;
}
