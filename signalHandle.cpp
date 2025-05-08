#include "signalHandle.hpp"
#include <csignal>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "jobs.hpp"
#include <csignal> 



using namespace std;
// vector<Job> jobs;



void signalHandler(int signum) {
  const char* msg = nullptr;
  sigset_t mask, prev_mask;
  sigemptyset(&mask);
  sigaddset(&mask, SIGTSTP);
  sigprocmask(SIG_BLOCK, &mask, &prev_mask);

  switch (signum) {
   

      case SIGINT:
          msg = "Signal Ctrl-C received.\n";
          // if (foreground_pid > 0) kill(foreground_pid, SIGINT);
          break;
      case SIGTSTP:
          msg = "Signal Ctrl-Z received.\n";
          pid_t foreground_pid = tcgetpgrp(STDIN_FILENO); // Get the foreground process group

          if (foreground_pid > 0) {
              // cout << "Foreground PID: " << foreground_pid << endl;

              // Send SIGTSTP to suspend the foreground process
              

              // Optionally update the job status to 'stopped'
              for (auto& job : jobs) {
                  // if (job.pid == foreground_pid) {
                      if (job.isBackground == false) {
                      job.status = 1;  // Mark as stopped
                      job.isBackground = true; // Set to background
                      cout << "\n[" << job.id << "] " << job.pid << " suspended.\n";
                      kill(job.pid, SIGTSTP);  // Suspend the process
                  }
              }
          } else {
              cout << "No foreground process group found.\n";
          }
          break;
  }

  if (msg) {
      write(STDOUT_FILENO, msg, strlen(msg));
      fflush(stdout);
  }
}



void setupSignalHandlers() {
  struct sigaction sa_child;
  sa_child.sa_handler = jobs_handler;
  sigemptyset(&sa_child.sa_mask);
  sa_child.sa_flags = SA_RESTART | SA_NOCLDSTOP;
  sigaction(SIGCHLD, &sa_child, nullptr);

  struct sigaction sa;
  sa.sa_handler = signalHandler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;  // <-- DO NOT set SA_RESTART here

  sigaction(SIGINT, &sa, nullptr);   // Ctrl-C
  sigaction(SIGTSTP, &sa, nullptr);  // Ctrl-Z
}
