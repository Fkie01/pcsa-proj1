
#include "parser.hpp"
#include <sstream>


std::vector<std::string> parseInput(const std::string &input) {
  std::stringstream ss(input);
  std::string token;
  std::vector<std::string> tokens;
  while (ss >> token) {
    tokens.push_back(token);
  }
  return tokens;
}

void parse_command(char *command_line, char **args, RedirectionType *redir_type, char **fileName) {
  *redir_type = NONE;
  *fileName = nullptr;

  char *token = strtok(command_line, " \n");
  int i = 0;
  while (token != nullptr) {
    if (strcmp(token, "<") == 0) {
      *redir_type = INPUT;
      token = strtok(nullptr, " \n");
      if (token != nullptr) {
        *fileName = token;
      }
    } else if (strcmp(token, ">") == 0) {
      *redir_type = OUTPUT;
      token = strtok(nullptr, " \n");
      if (token != nullptr) {
        *fileName = token;
      }
    } else {
      args[i++] = token;
    }
    token = strtok(nullptr, " \n");
  }
  args[i] = nullptr; // Null-terminate the array
}
