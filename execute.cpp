#include "execute.hpp"
#include "jobs.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

extern std::vector<Job> jobs;
int executeCommand(const std::vector<std::string> &args, bool background) {
  if (args.empty())
    return 0;

  std::vector<char *> c_args;
  for (const auto &arg : args) {
    c_args.push_back(strdup(arg.c_str()));
  }
  
  std::vector<std::string> command;
  for (const auto &arg : args) {
    command.push_back(arg);
  }

  
  c_args.push_back(nullptr); // Null-terminate the array
  command.push_back(nullptr);
  pid_t pid = fork();
  int exitCode = 0;
  // std::cout << c_args[0] << std::endl;
  if (pid == 0) {
    setpgid(0, 0); // Set the process group ID to the child process ID
    // signal(SIGINT, SIG_DFL);
    // signal(SIGTSTP, SIG_DFL);
    if (execvp(c_args[0], c_args.data()) ) {
      
      std::cerr << "Error executing command: " << c_args[0] << std::endl;
      exit(EXIT_FAILURE);
    }

    
  } else if (pid > 0) {
    if (background){
      setpgid(pid, pid); // Set the process group ID to the child process ID
      // std::cout << "background" << std::endl;
      static int jobId = 1;
      std::cout << "[" << jobId << "] " << pid << "   "  << std::endl;
      jobs.push_back({jobId, pid, command, 0,background});
      jobId++;
    }
    else{
      int status;
      waitpid(pid, &status, 0);
      if (WIFEXITED(status)) {
        exitCode = WEXITSTATUS(status);
      }
      // tcsetpgrp(STDIN_FILENO, getpgrp());
    }
  } else {
      perror("fork failed");
      // std::cerr << "Child process terminated abnormally" << std::endl;
    }
    // for (auto arg : c_args) {
    //   free(arg);
    // }
  
  return exitCode;
}

