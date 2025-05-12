#ifndef IS_REDIRECTION_HPP
#define IS_REDIRECTION_HPP

#include "parser.hpp"

#include <string>
#include <vector>

void handleRedirectionAndExecute(char **args, RedirectionType redir_type, const char *filename);
void handlePipeAndExecute(char **args1, char **args2);
#endif // IS_REDIRECTION_HPP
