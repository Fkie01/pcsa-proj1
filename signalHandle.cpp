#include "signalHandle.hpp"
#include <csignal>
#include <iostream>
#include <unistd.h>

using namespace std;

void signalHandler(int signum) {
  string signal;
  switch (signum) {
  case 2:
    signal = "Ctrl-C";
    break;
  case 18:
    signal = "Ctrl-Z";
    break;
  }

  cout << " Signal " << signal << " received." << endl;
  cout << "icsh $ " << flush;
}

void setupSignalHandlers() {
  signal(SIGTSTP, signalHandler);
  signal(SIGINT, signalHandler);
}
