#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <deque>

#include "PCB.h"

#define UNDERLINE "\033[4m" // To put underline to the text

// Colors for the text
#define RED "\033[1;31m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define WHITE "\033[1;37m"
#define RESET "\033[1;0m"

#define CLOSE "\033[0m" // End of any underline or color

using namespace std;


// <======> To Read the input from the file <======>
void read_input(int &quantum, int &context_switch, vector<PCB> &v, int NUMBER_OF_PROCESSES, const string file_name)
{
    ifstream in;        // To read from file stream
    in.open(file_name); // Initialize the file stream from the needed file
    // Read operation
    in >>  quantum >> context_switch;
    for (int i = 0; i < NUMBER_OF_PROCESSES; ++i)
    {
        int process_id, arrival_time, cpu_burst, size_in_bytes;
        in >> process_id >> arrival_time >> cpu_burst;
        v.emplace_back(PCB(process_id, arrival_time, cpu_burst));
    }
    in.close(); // Close the file stream
}

// To print the PCB information
void print(vector<PCB> &v)
{
    sort(begin(v), end(v),
         [](PCB a, PCB b) // Comparator function
         { return a.id <= b.id; });
    for (PCB val : v) // For each for loop to print the PCB information
    {
        cout << "ID = " << val.id << '\n'
             << YELLOW << "-" << RESET << " Finish Time = " << val.finish_time << '\n'
             << YELLOW << "-" << RESET << " Waiting Time = " << val.waiting_time << '\n'
             << YELLOW << "-" << RESET << " Turnaround Time = " << val.turn_around_time << '\n'
             << '\n';
    }
}

// Print the statistics of the PCB
void print_statistics(vector<PCB> &v, int end_of_all_processes_time, int number_of_processes)
{
    double average_waiting_time = 0;
    double average_turnaround_time = 0;
    double cpu_utilization = v.size();
    double execution_time = 0;
    // Calculating the sums of the statistics to find the average
    for (PCB process : v)
    {
        average_turnaround_time += process.turn_around_time;
        average_waiting_time += process.waiting_time;
        execution_time += process.cpu_burst;
    }
    cpu_utilization = 1.0 * execution_time / end_of_all_processes_time;
    average_turnaround_time /= 1.0 * number_of_processes;
    average_waiting_time /= 1.0 * number_of_processes;
    cout << WHITE << UNDERLINE << "\nRESULTS"
         << CLOSE << WHITE << ":\n"
         << CLOSE
         << BLUE << "•" << RESET << " Average Waiting Time = " << average_waiting_time << '\n'
         << BLUE << "•" << RESET << " Average Turnaround Time = " << average_turnaround_time << '\n'
         << BLUE << "•" << RESET << " CPU Utilization = " << cpu_utilization << '\n'
         << CLOSE;
}

// Sort depends on arrival time
void sort_on_arrival_time_then_id(vector<PCB> &v)
{
    sort(begin(v), end(v),
         [v](PCB a, PCB b) // Comparator function
         {
             if (a.arrival_time != b.arrival_time)
                 return (a.arrival_time <= b.arrival_time);
             else
                 return (a.id <= b.id);
         });
}

// Sort depends on cpu burst if the arrival time < current time
void sort_on_SJF(deque<PCB> &d, int time)
{
    sort(begin(d), end(d),
         [time](PCB a, PCB b) // Comparator function
         {
             if (a.arrival_time <= time && b.arrival_time <= time)
                 return a.cpu_burst <= b.cpu_burst;
             else if (a.arrival_time <= time)
                 return true;
             else if (b.arrival_time <= time)
                 return false;
             else
                 return a.id <= b.id;
         });
}

// Sort depends on id if the arrival time < current time
void sort_on_RR(deque<PCB> &d, int time)
{
    sort(begin(d), end(d),
         [time](PCB a, PCB b) // Comparator function
         {
             if (a.arrival_time <= time && b.arrival_time <= time)
                 return a.id <= b.id;
             else if (a.arrival_time <= time)
                 return true;
             else if (b.arrival_time <= time)
                 return false;
             else
                 return a.id <= b.id;
         });
}

// To handle the processing of burst time in the FCFS
void handle_processing_in_FCFS(int time, PCB &p)

{
    p.finish_time = time;
    p.turn_around_time = p.finish_time - p.arrival_time;
    p.waiting_time += (time - p.last_time_in_ready - p.remaining_burst);
    p.last_time_in_ready = time - p.remaining_burst;
    p.remaining_burst = 0;
    if (p.response_time == -1)
        p.response_time = p.last_time_in_ready - p.arrival_time;
}

//To handle the processing of burst time in the SRT
void handle_processing_in_SRT(int time, PCB& process, vector<PCB>& all_processes) {
    // Update the process in the original vector with remaining time and completion time
    for (auto& p : all_processes) {
        if (p.id == process.id) {
            p.remaining_burst = process.remaining_burst;
            if (process.remaining_burst == 0) {
                p.finish_time = time;
                p.turn_around_time = p.finish_time - p.arrival_time;
                p.waiting_time = p.turn_around_time - p.cpu_burst;
            }
            break;
        }
    }
}

// To handle the processing of burst time in the RR
PCB handle_processing_in_RR(int time, PCB &p, vector<PCB> &v, bool last_time, int quantum)
{
    PCB ret = p; // The variable to be returned
    // For loop to find the index of the process in the vector
    int id = p.id, index = -1;
    for (int i = 0; i < v.size(); i++)
    {
        if (id == v[i].id)
        {
            index = i;
            break;
        }
    }
    if (last_time)
    {
        v[index].finish_time = time;
        v[index].turn_around_time = v[index].finish_time - v[index].arrival_time;
        v[index].waiting_time += (time - v[index].last_time_in_ready - v[index].remaining_burst);
        v[index].last_time_in_ready = time - v[index].remaining_burst;
        v[index].remaining_burst = 0;
        if (v[index].response_time == -1)
            v[index].response_time = v[index].last_time_in_ready - v[index].arrival_time;
    }
    else
    {
        v[index].waiting_time += (time - v[index].last_time_in_ready - v[index].remaining_burst);
        ret.waiting_time = (time - v[index].last_time_in_ready - v[index].remaining_burst);

        v[index].last_time_in_ready = time - v[index].remaining_burst;
        ret.last_time_in_ready = time - v[index].remaining_burst;        

        v[index].remaining_burst -= quantum;
        ret.remaining_burst -= quantum;

        if (v[index].response_time == -1)
            v[index].response_time = v[index].last_time_in_ready - v[index].arrival_time;
    }
    return ret;
}