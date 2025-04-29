#include "execute.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>

int executeCommand(const std::vector<std::string> &args) {
  if (args.empty())
    return 0;

  std::vector<char *> c_args;
  for (const auto &arg : args) {
    c_args.push_back(strdup(arg.c_str()));
  }
  c_args.push_back(nullptr); // Null-terminate the array
  pid_t pid = fork();
  int exitCode = 0;

  if (pid == 0) {
    if (execvp(c_args[0], c_args.data()) ) {
      std::cerr << "Error executing command: " << c_args[0] << std::endl;
      exit(EXIT_FAILURE);
    }
  } else if (pid > 0) {
    int status;
    waitpid(pid, &status, 0);
    if (WIFEXITED(status)) {
      exitCode = WEXITSTATUS(status);
    } else {
      perror("fork failed");
      // std::cerr << "Child process terminated abnormally" << std::endl;
    }
    for (auto arg : c_args) {
      free(arg);
    }
  }
  return exitCode;
}

