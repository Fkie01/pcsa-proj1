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

void handlePipeAndExecute(char **args1, char **args2) { // it will execute the first command and pipe the output to the second command that will receive the input from first command
    int pipefd[2];
    
    // Create pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }

    pid_t pid1 = fork();
    if (pid1 < 0) {
        perror("fork");
        return;
    }

    if (pid1 == 0) {
        // First child (executes args1 and sends output to pipe)
        dup2(pipefd[1], STDOUT_FILENO); // redirect stdout to pipe write end
        close(pipefd[0]);               // close unused read end
        close(pipefd[1]);

        execvp(args1[0], args1);
        perror("execvp left");
        exit(1);  // Exit if execvp fails
    }

    pid_t pid2 = fork();
    if (pid2 < 0) {
        perror("fork");
        return;
    }

    if (pid2 == 0) {
        // Second child (executes args2 and reads input from pipe)
        dup2(pipefd[0], STDIN_FILENO); // redirect stdin to pipe read end
        close(pipefd[0]);               // close unused write end
        close(pipefd[1]);

        execvp(args2[0], args2);
        perror("execvp right");
        exit(1);  // Exit if execvp fails
    }

    // Parent: Close pipe and wait for both children
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, nullptr, 0);
    waitpid(pid2, nullptr, 0);
}