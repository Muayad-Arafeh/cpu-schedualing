#pragma once

using namespace std;

// This file is the place to build the Process control Block (PCB)
class PCB
{
public:
    // The Attributes of the PCB class

    // The id of the process
    int id;

    // The time that the process enter the ready queue for the first time
    int arrival_time;

    // The total cpu burst of the process
    int cpu_burst;

    // The time that the process finish the execution
    int finish_time;

    // The total time the process wait in the ready queue
    int waiting_time;

    // The total time from the request of the process until finish the execution
    int turn_around_time;

    // The last time the process was in the ready queue => used in the calculations
    int last_time_in_ready;

    // The remaining cpu burst after some (zero or more) times in the cpu => used in the calculations
    int remaining_burst;

    // The time from the request of the process until first time enter the cpu
    int response_time;


    // The methods of the PCB class
    PCB(int id, int arrival_time, int cpu_burst) // Constructor
    {
        this->id = id;
        this->arrival_time = arrival_time;
        this->cpu_burst = cpu_burst;
        this->remaining_burst = cpu_burst;
        this->last_time_in_ready = arrival_time;
        this->finish_time = 0;
        this->waiting_time = 0;
        this->turn_around_time = 0;
        this->response_time = -1;
    }
};