#include "jobs.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <csignal> 




using namespace std;

vector<Job> jobs;


void jobs_handler(int) {
    // Wait for any child process to terminate
    // cout << "Child process terminated. from signal" << endl;
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
      for (auto it = jobs.begin(); it != jobs.end(); ++it) {
          if (it->pid == pid) {
              if (WIFEXITED(status)) {
                std::cout << "\n[" << it->id << "] + " << pid << " done           ";
                for (const auto& word : it->command) {
                    std::cout << word << " ";
                }
                
                std::cout << std::endl;
                
                fflush(stdout);

                if(it->isBackground) {
                    std::cout << "icsh $ " << std::flush;
                } else {
                    std::cout << "\n" << std::flush;
                }
                // std::cout << "icsh $ " << std::flush;
                
                // need_new_prompt = 1;

                
              } else {
                  std::cout << "\nJob [" << it->id << "] (" << pid << ") terminated abnormally" << std::endl;
              }
              jobs.erase(it);
              break;
          }
      }
        // Reprompt if interrupted
    }
    
}

void print_jobs(){
    int cut = 1;
    std::string current_status;
    if (jobs.empty()){
        cout << "No jobs running" << endl;
        return;
    }
    for (auto it = jobs.begin(); it != jobs.end(); ++it){
        // cout << "Job ID: " << it->id << "    " << it->status<< endl;
        if (it->status == 1){
            current_status = " suspended        ";
            
        }
        
        else{
            current_status = " running          ";
        }
        
        if (cut % 2 != 0){
            cout << "[" << it->id << "] - " << it->pid << current_status;
            for(const auto& word : it->command) {
                std::cout << word << " ";
            }
            // cout << it->isBackground;
            cut++;
            cout << endl;
        }
        else {
            cout << "[" << it->id << "] + " << it->pid << current_status;
            for(const auto& word : it->command) {
                std::cout << word << " ";
            }
            // cout << it->isBackground;
            cut++;
            cout << endl;
        }
    }
}

void bringToForeground(int jobId) {
    // cout << jobId << endl;
    // cout << "jobs size: " << jobs.size() << endl;

    // if (jobId < 0 || jobId > jobs.size()) {
    //     cout << "Invalid job ID" << endl;
    //     return;
    // }
    
    for (auto it = jobs.begin(); it != jobs.end(); ++it) {
        if (it->id == jobId) {
            // Send SIGCONT to the job to bring it to the foreground
            kill(it->pid, SIGCONT);

            cout << "[" << it->id << "]   " << it->pid << " running          ";
            for (const auto& word : it->command) {
                std::cout << word << " ";
            }
            it->isBackground = false;
            cout << endl;
            // Wait for the job to finish
            int status;
            waitpid(it->pid, &status, 0);
            
            break;
        }
        else{
            cout << "Job not found" << endl;
            return;
        }
    }
}

void bringToBackground(int jobId){
    for (auto it = jobs.begin(); it != jobs.end(); ++it) {
        if (it->id == jobId) {
            // Send SIGCONT to the job to bring it to the background
            kill(it->pid, SIGCONT);
            it->isBackground = true;
            it->status = 0;
            cout << "[" << it->id << "]   " << it->pid << "                  ";
            for (const auto& word : it->command) {
                std::cout << word << " ";
            }
            cout << endl;
            break;
        }
    }
}