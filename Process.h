#ifndef PROCESO_H
#define PROCESO_H

#include <string>

using namespace std;

class Process {
public:
    string etiqueta;
    float burstTime; // BT
    float arrivalTime; // AT
    int queueID; // Q
    int priority; // Prioridad en la cola

    float waitingTime; // WT
    float completionTime; // CT
    float responseTime; // RT
    float turnAroundTime; // TAT

    float tiempoRestante;
    bool yaInicio;

    Process(string et, float bt, float at, int q, int pr) {
        etiqueta = et;
        burstTime = bt;
        arrivalTime = at;
        queueID = q;
        priority = pr;

        tiempoRestante = bt;
        waitingTime = 0;
        completionTime = 0;
        responseTime = -1; // si está -1 es porque no se ha iniciado todavia
        yaInicio = false; // si está en true es porque ya entró a la CPU por primera vez
    }
};

#endif