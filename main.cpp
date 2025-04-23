
#include "parser.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
int main() {
  std::string input;

  while (true) {
    std::cout << "mysh> ";
    std::getline(std::cin, input);

    if (input == "exit")
      break;

    std::vector<std::string> args = parseInput(input);
    if (args.empty())
      continue;

    std::vector<char *> cargs;
    for (auto &arg : args) {
      cargs.push_back(&arg[0]);
    }
    cargs.push_back(nullptr);

    pid_t pid = fork();
    if (pid == 0) {
      // Child process
      if (execvp(cargs[0], cargs.data()) == -1) {
        perror("exec failed");
      }
      exit(EXIT_FAILURE);
    } else if (pid > 0) {
      // Parent process
      waitpid(pid, nullptr, 0);
    } else {
      perror("fork failed");
    }
  }

  return 0;
}
