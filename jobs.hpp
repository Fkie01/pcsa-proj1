#ifndef JOBS_HPP
#define JOBS_HPP
#pragma once
#include <string>
#include <vector>
#include <csignal>






struct Job {
    int id;
    pid_t pid;
    std::vector<std::string> command;
    int status; // 0: running, 1: stopped
    bool isBackground;
};
extern std::vector<Job> jobs;

void jobs_handler(int);
void print_jobs();
void bringToForeground(int jobId);
void bringToBackground(int jobId);

#endif // JOBS_HPP