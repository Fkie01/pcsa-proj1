#include "isRedirection.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <cstdlib>
using namespace std;


void handleRedirectionAndExecute(char **args, RedirectionType redir_type, const char *filename) {
  if (args[0] == nullptr) {
    cerr << "No command to execute." << endl;
    return;
  }
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        // Child
        if (redir_type == INPUT) {
            int fd = open(filename, O_RDONLY);
            if (fd < 0) {
                perror("open input");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        } else if (redir_type == OUTPUT) {
            int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (fd < 0) {
                perror("open output");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(args[0], args);
        perror("execvp");
        exit(1);
    } else {
        // Parent
        waitpid(pid, nullptr, 0);
    }
}
