
#include "execute.hpp"
#include "parser.hpp"
#include "signalHandle.hpp"
#include <csignal>
#include <cstdlib> // free
#include <cstring> // strdup
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

int main(int argc, char *argv[]) {
  setupSignalHandlers();
  std::istream *inputStream = &std::cin;
  std::ifstream fileStream;

  if (argc == 2) {
    fileStream.open(argv[1]);
    if (!fileStream.is_open()) {
      std::cerr << "Failed to open scripted file: " << argv[1] << std::endl;
      return 1;
    }
    inputStream = &fileStream;
  }

  std::string input;
  std::string lastCommand;
  int exitCode = 0;

  std::cout << "  -------------welcome to kanut shell--------------\n";

  while (true) {
    if (inputStream == &std::cin) {
      std::cout << "icsh $ ";
    }

    if (!std::getline(*inputStream, input)) {
      break;
    }

    // setupSignalHandlers();

    // Handle !!
    if (input == "!!") {
      if (lastCommand.empty()) {
        std::cerr << "No previous command.\n";
        continue;
      }
      // std::cout << lastCommand << std::endl;
      input = lastCommand;
    } else {
      lastCommand = input;
    }

    std::vector<std::string> args = parseInput(input);
    if (args.empty())
      continue;

    if (args[0] == "exit") {
      if (args.size() == 2) {
        try {
          exitCode = std::stoi(args[1]) & 0xFF;
        } catch (...) {
          exitCode = 0;
        }
      }
      std::cout << "bye\n";
      break;
    }

    exitCode = executeCommand(args);
  }

  return exitCode;
}
