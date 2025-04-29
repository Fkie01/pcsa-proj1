
#include "execute.hpp"
#include "isRedirection.hpp"
#include "parser.hpp"
#include "signalHandle.hpp"
#include <csignal>
#include <cstdlib> // free
#include <cstring> // strdup
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <string>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main(int argc, char *argv[]) {
  setupSignalHandlers(); // handle ctrl-c and ctrl-z
  std::istream *inputStream = &std::cin;
  std::ifstream fileStream;

  // std::cout << argc << std::flush;

  if (argc == 2) {
    fileStream.open(argv[1]);
    if (!fileStream.is_open()) {
      std::cerr << "Failed to open scripted file: " << argv[1] << std::endl;
      return 1;
    }
    inputStream = &fileStream;
  }

  std::string input;
  std::string output;
  std::string lastCommand;
  int exitCode = 0;

  std::cout << "  -------------welcome to shell--------------\n";

  while (true) {
    bool isRedirection = false;
    // int redirectionPosition;
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
    std::cout << "args size: " << args.size() << std::endl;
    // std::cout << args[2] << std::endl;

    if (args.size() >= 1) {
      for (size_t i = 1; i < args.size(); ++i) {
        if (std::string(args[i]) == ">" || std::string(args[i]) == "<") {
          isRedirection = true;
          break;
        }
      }
    }

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
    
    char command_line[MAX_LINE];
    char *argss[MAX_ARGS];
    RedirectionType redir_type;
    char *fileName;
    std::strncpy(command_line, input.c_str(), MAX_LINE);
    if (isRedirection) {
      parse_command(command_line, argss, &redir_type, &fileName);
      handleRedirectionAndExecute(argss, redir_type, fileName);

      // exitCode = redirectExecuteCommand(args);
      // std::cout << "icsh $ " << std::flush;

    } else {
      exitCode = executeCommand(args);
    }
  }

  return exitCode;
}
