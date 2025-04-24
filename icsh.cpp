
#include "parser.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
int main() {
  std::string input;
  std::string lastCommand;
  std::cout << "  -------------welcome to kanut shell <;<--------------\n";

  while (true) {
    std::cout << "icsh $ ";
    std::getline(std::cin, input);

    // Handle !!
    if (input == "!!") {
      if (lastCommand.empty())
        continue;
      std::cout << lastCommand << std::endl;
      input = lastCommand;
    } else {
      lastCommand = input;
    }

    // Handle exit with optional code
    if (input.rfind("exit", 0) == 0) {
      std::istringstream iss(input);
      std::string cmd;
      int code = 0;
      iss >> cmd >> code;
      std::cout << "bye\n";
      return code & 0xFF;
    }

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
      // child process
      if (execvp(cargs[0], cargs.data()) == -1) {
        // perror("Bad Command");
        std::cout << "Bad command: noob! \n";
      }
      exit(EXIT_FAILURE);
    } else if (pid > 0) {
      // parent process
      waitpid(pid, nullptr, 0);
    } else {
      perror("fork failed");
    }
  }

  return 0;
}
