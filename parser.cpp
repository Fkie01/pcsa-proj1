
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

  char *token = strtok(command_line, " \n"); // need to be used char not string because of strtok so i easier to split the command line
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
    } else if (strcmp(token, "|") == 0){

    } else {
      args[i++] = token;
    }
    token = strtok(nullptr, " \n");
  }
  args[i] = nullptr; // Null-terminate the array
}

void parse_PipeCommand(char *command_line, char **args1, char **args2) { // same parser logic as redirection parser
  char *token = strtok(command_line, " \n");
  bool ispipe = false;
  int i = 0;
  while(token != nullptr){
    if(strcmp(token, "|") == 0){
      ispipe = true;
      token = strtok(nullptr, " \n");
      i = 0;
      continue;
    }
    else if (ispipe == false){
      args1[i++] = token;
      token = strtok(nullptr, " \n");
    } 
    else{
      args2[i++] = token;
      token = strtok(nullptr, " \n");
    }
  }
}
