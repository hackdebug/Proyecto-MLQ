#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Process.h"

using namespace std;

class Scheduler {
private:
    vector<Process> allProcesses;

    vector<Process> cola1; // RR(1)
    vector<Process> cola2; // RR(2)
    vector<Process> cola3; // SJF

    vector<Process> ProcessFinished;

public:
    void loadProcesses(string namefile);
    {
        ifstream file(filename);
    string line;

    if (file.is_open()) {
        cout << "Error: No se pudo abrir el archivo " << filename << endl;
        return;
    }

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue; [cite: 21, 22]

        stringstream ss(line);
        string et, bt_str, at_str, q_str, pr_str;

        getline(ss, et, ';'); [cite: 30]
        getline(ss, bt_str, ';'); [cite:30]
        getline(ss, at_str, ';'); [cite:30]
        getline(ss, q_str, ';'); [cite:30]
        getline(ss, pr_str, ';'); [cite:30]

        float bt = stof(bt_str);
        float at = stof(at_str);
        int q = stoi(q_str);
        int pr = stoi(pr_str);

        process p(et,bt,at,q,pr);
        allprocesses.push_back(p);
    }
    file.close();
    cout << "Procesos cargados exitosamente: " << allProcesses.size() << endl;
    }

    void runsimulation() {
        float actualtime = 0;

        while (!allprocesses.empty() || !cola1.empty() || !cola2.empty() || !cola3.empty()) {
            for (int i = 0; i < allprocesses.size(); i++) {
                if (allProcesses[i].arrivalTime <= actualtime) {
                    if (allProcesses[i].queueID == 1) {
                        cola1.push_back(allProcesses[i]);
                    } else if (allProcesses[i].queueID == 2) {
                        cola2.push_back(allProcesses[i]);
                    } else if (allProcesses[i].queueID == 3) {
                        cola3.push_back(allProcesses[i]);
                    }

                    allProcesses.erase(allProcesses.begin() + i);
                    i--;
                }
            }
        }

        if (!cola1.empty()) {
            actualtime += 1;
        }
        else if (!cola2.empty()) {
            actualtime += 1;
        }
        else if (!cola3.empty()) {
            actualtime += 1;
        }
        else {
            actualtime += 1;
        }

        calcularresultados();
    }
private:
    void calcularresultados() {
        cout << "Simulación finalizada." << endl;
    }
};