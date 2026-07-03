#include <iostream>
#include "Scheduler.h"

using namespace std;

int main() {
    Scheduler simulation;

    simulation.loadProcesses("mlq001.txt");
    cout << "Iniciando el planificador MLQ..." << endl;
    simulation.runSimulation();

    return 0;
}