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
    void loadProcesses(string namefile)
    {
        ifstream file(namefile);
        string line;

        if (!file.is_open()) {
            cout << "Error: No se pudo abrir el archivo " << namefile << endl;
            return;
        }

        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;

            stringstream ss(line);
            string et, bt_str, at_str, q_str, pr_str;

            getline(ss, et, ';');
            getline(ss, bt_str, ';');
            getline(ss, at_str, ';');
            getline(ss, q_str, ';');
            getline(ss, pr_str, ';');

            float bt = stof(bt_str);
            float at = stof(at_str);
            int q = stoi(q_str);
            int pr = stoi(pr_str);

            Process p(et,bt,at,q,pr);
            allProcesses.push_back(p);
        }
        file.close();
        cout << "Procesos cargados exitosamente: " << allProcesses.size() << endl;
    }

    void runSimulation() {
        float actualtime = 0;

        float quantumCola2Consumido = 0;

        while (!allProcesses.empty() || !cola1.empty() || !cola2.empty() || !cola3.empty()) {
            for (int i = 0; i < allProcesses.size(); i++) {
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

            if (!cola1.empty()) {
                quantumCola2Consumido = 0;

                Process p = cola1[0];
                cola1.erase(cola1.begin());

                if (!p.yaInicio) {
                    p.responseTime = actualtime - p.arrivalTime;
                    p.yaInicio = true;
                }

                actualtime += 1.0;
                p.tiempoRestante -= 1.0;

                if (p.tiempoRestante <= 0) {
                    p.completionTime = actualtime;
                    p.turnAroundTime = p.completionTime - p.arrivalTime;
                    p.waitingTime = p.turnAroundTime - p.burstTime;

                    ProcessFinished.push_back(p);
                } else {
                    cola1.push_back(p);
                }

            }
            else if (!cola2.empty()) {
                Process p = cola2[0];
                cola2.erase(cola2.begin());

                if (!p.yaInicio) {
                    p.responseTime = actualtime - p.arrivalTime;
                    p.yaInicio = true;
                }

                actualtime += 1.0;
                p.tiempoRestante -= 1.0;
                quantumCola2Consumido += 1.0;

                if (p.tiempoRestante <= 0) {
                    p.completionTime = actualtime;
                    p.turnAroundTime = p.completionTime - p.arrivalTime;
                    p.waitingTime = p.turnAroundTime - p.burstTime;

                    ProcessFinished.push_back(p);
                    quantumCola2Consumido = 0;
                } else {
                    if (quantumCola2Consumido >= 3.0) {
                        cola2.push_back(p);
                        quantumCola2Consumido = 0;
                    } else {
                        cola2.insert(cola2.begin(), p);
                    }
                }
            }
            else if (!cola3.empty()) {
                quantumCola2Consumido = 0;

                int indiceMasCorto = 0;
                float menorBT = cola3[0].burstTime;

                for (int i = 1; i < cola3.size(); i++) {
                    if (cola3[i].burstTime < menorBT) {
                        menorBT = cola3[i].burstTime;
                        indiceMasCorto = i;
                    }
                }

                Process p = cola3[indiceMasCorto];

                if (!p.yaInicio) {
                    p.responseTime = actualtime - p.arrivalTime;
                    p.yaInicio = true;
                }

                actualtime += p.burstTime;
                p.tiempoRestante = 0;

                p.completionTime = actualtime;
                p.turnAroundTime = p.completionTime - p.arrivalTime;
                p.waitingTime = p.turnAroundTime - p.burstTime;

                ProcessFinished.push_back(p);
                cola3.erase(cola3.begin() + indiceMasCorto);
            }
            else {
                actualtime += 1;
            }
        }

        calcularresultados();
    }
private:
    void calcularresultados() {
        if (ProcessFinished.empty()) {
            cout << "No hay procesos terminados para calcular metricas" << endl;
            return;
        }

        float sumaWT = 0, sumaCT = 0, sumaRT = 0, sumaTAT = 0;

        cout << "\n# ====== RESULTADOS DE LA SIMULACION ======" << endl;
        cout << "# etiqueta; BT; AT; Q; Pr; WT; CT; RT; TAT" << endl;

        for (int i = 0; i < ProcessFinished.size(); i++) {
            Process &p = ProcessFinished[i];

            cout << p.etiqueta << ";"
                 << p.burstTime << "; "
                 << p.arrivalTime << "; "
                 << p.queueID << "; "
                 << p.priority << "; "
                 << p.waitingTime << "; "
                 << p.completionTime << "; "
                 << p.responseTime << "; "
                 << p.turnAroundTime << endl;

            sumaWT += p.waitingTime;
            sumaCT += p.completionTime;
            sumaRT += p.responseTime;
            sumaTAT += p.turnAroundTime;
        }

        int totalProcesses = ProcessFinished.size();
        float promWT = sumaWT / totalProcesses;
        float promCT = sumaCT / totalProcesses;
        float promRT = sumaRT / totalProcesses;
        float promTAT = sumaTAT / totalProcesses;

        cout << "# WT=" << promWT
             << " CT=" << promCT
             << " RT=" << promRT
             << " TAT=" << promTAT << ";" << endl;
        cout << "===========================================\n" << endl;
    }
};
#endif