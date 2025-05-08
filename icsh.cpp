
#include "execute.hpp"
#include "isRedirection.hpp"
#include "parser.hpp"
#include "signalHandle.hpp"
#include "jobs.hpp"
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




#define MAX_LINE 1024
#define MAX_ARGS 64
// std::vector<Job> jobs;

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
    // setupSignalHandlers(); // handle ctrl-c and ctrl-z
    bool isRedirection = false;
    bool background = false;
    // int redirectionPosition;
    if (inputStream == &std::cin) {
      
      write(STDOUT_FILENO, "icsh $ ",7);
      std::cout << std::flush;
      // std::cout << "icsh $ " << std::flush;
    }
  //   if (need_new_prompt) {
  //     need_new_prompt = 0;
  //     std::cout << "icsh $ " << std::flush;
  // }
  
    
    if (!std::getline(*inputStream, input)) {
      // break;
      if (!inputStream) break;

  // Handle Ctrl-C / Ctrl-Z interruption
      std::cin.clear();                     // Clear error state
      std::cout << std::endl;              // Force newline
      continue;   
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
    // std::cout << "args size: " << args.size() << std::endl;
    // std::cout << args[2] << std::endl;

    if (args.size() >= 1) {
      for (size_t i = 1; i < args.size(); ++i) {
        if (std::string(args[i]) == ">" || std::string(args[i]) == "<") {
          isRedirection = true;
          break;
        }
      }
    }

    if (args.size() >= 1 && strcmp(args[0].c_str(), "jobs") == 0) { // print jobs 
      print_jobs();
      continue;
    }

    if (args.size() >= 1 && strcmp(args[0].c_str(), "fg") == 0) { // bring to foreground
      if (args.size() == 2) {

        if (args[1][0] == '%') {
          args[1].erase(0, 1); // Remove the '%' character
        }

        // std::cout << args[1] << std::endl;
        int jobId = std::stoi(args[1]);
        
        bringToForeground(jobId);
      } else {
        std::cerr << "Usage: fg <job_id>\n";
      }
      continue;
    }
    if (args.size() >= 1 && strcmp(args[0].c_str(), "bg") == 0) { // bring to background
      if (args.size() == 2) {
        if (args[1][0] == '%') {
          args[1].erase(0, 1); // Remove the '%' character
        }
        int jobId = std::stoi(args[1]);
        // std::cout << args[1] << std::endl;
        bringToBackground(jobId);
      } else {
        std::cerr << "Usage: bg <job_id>\n";
      }
      continue;
    }

    if (!args.empty() && args.back() == "&") { // Check if the last argument is "&"
      background = true;
      args.pop_back();

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
      exitCode = executeCommand(args, background);
      // std::cout << "test" << std::flush;
    }
  }
  


  return exitCode;
}
