
#ifndef PARSER_HPP
#define PARSER_HPP

#include <string>
enum RedirectionType { NONE, INPUT, OUTPUT } ;

std::vector<std::string> parseInput(const std::string &input);
void parse_command(char *command_line, char **args, RedirectionType *redir_type, char **fileName);
void parse_PipeCommand(char *command_line, char **args1, char **args2);
#endif
