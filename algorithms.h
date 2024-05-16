#pragma once

#include <deque>

#include "PCB.h"
#include "handle.h"

using namespace std;

// First Come First Serve Algorithm
pair<int, vector<PCB>> first_come_first_serve(vector<PCB> v, int context_switch = 0)
{
    sort_on_arrival_time_then_id(v);
    int time = 0;
    cout << "0 | ";
    for (PCB &current_process : v)
    {
        time += current_process.cpu_burst;
        cout << UNDERLINE << "P" << current_process.id << CLOSE << " | " << time << " | ";
        handle_processing_in_FCFS(time, current_process);
        if (context_switch != 0)
        {
            cout << UNDERLINE << "CS = " << context_switch << CLOSE << " | ";
            time += context_switch;
            cout << time << " | ";
        }
    }
    cout << '\n';
    return {time, v};
}
// Shortest Remaining Time
pair<int, vector<PCB>> shortest_remaining_time(vector<PCB> v, int context_switch = 0) {
    int time = 0;
    cout << "0 | ";
    deque<PCB> remain_processes(begin(v), end(v)); // Give the processes to the deque

    // Initialize remaining_time for each process
    for (auto& process : remain_processes) {
        process.remaining_burst = process.cpu_burst;
        process.waiting_time = 0;
        process.turn_around_time = 0;
    }

    while (!remain_processes.empty()) {
        // Sort by remaining time, if equal, by arrival time
        sort(remain_processes.begin(), remain_processes.end(), [time](const PCB& a, const PCB& b) {
            if (a.remaining_burst != b.remaining_burst)
                return a.remaining_burst < b.remaining_burst;
            return a.arrival_time < b.arrival_time;
        });

        PCB current_process = remain_processes.front();
        remain_processes.pop_front();
        
        while (current_process.remaining_burst > 0) {
            // Simulate one unit of time
            current_process.remaining_burst--;
            time++;
            // Check if a new process arrives with a shorter remaining burst time
            bool preempted = false;
            for (auto& process : remain_processes) {
                if (process.arrival_time <= time && process.remaining_burst < current_process.remaining_burst) {
                    // Push the current process back to the deque
                    remain_processes.push_back(current_process);
                    cout << UNDERLINE << "P" << current_process.id << CLOSE << " | " << time << " | ";
                    if (context_switch != 0) {
                        cout << UNDERLINE << "CS = " << context_switch << CLOSE << " | ";
                        time += context_switch;
                        cout << time << " | ";
                    }
                    preempted = true;
                    break;
                }
            }
            if (preempted) break;
        }

        if (current_process.remaining_burst == 0) {
            cout << UNDERLINE << "P" << current_process.id << CLOSE << " | " << time << " | ";
            handle_processing_in_SRT(time, current_process, v);
        }

        if (context_switch != 0 && !remain_processes.empty() && current_process.remaining_burst == 0) {
            cout << UNDERLINE << "CS = " << context_switch << CLOSE << " | ";
            time += context_switch;
            cout << time << " | ";
        }
    }
    cout << '\n';
    return {time, v};
}
// Round Robin
pair<int, vector<PCB>> round_robin(int quantum, vector<PCB> v, int context_switch = 0)
{
    int time = 0;
    cout << "0 | ";
    deque<PCB> remain_processes(begin(v), end(v)); // Give the processes to the deque
    int last_process_id = -1;
    sort_on_RR(remain_processes, time);
    while (!remain_processes.empty())
    {
        PCB current_process = remain_processes.front();
        remain_processes.pop_front();
        last_process_id = current_process.id;
        if (quantum >= current_process.remaining_burst)
        {
            time += current_process.remaining_burst;
            cout << UNDERLINE << "P" << current_process.id << CLOSE << " | " << time << " | ";
            handle_processing_in_RR(time, current_process, v, true, quantum);
        }
        else
        {
            time += quantum;
            cout << UNDERLINE << "P" << current_process.id << CLOSE << " | " << time << " | ";
            PCB new_process = handle_processing_in_RR(time, current_process, v, false, quantum);
            remain_processes.emplace_front(new_process);
        }
        sort_on_RR(remain_processes, time);

        if (context_switch != 0 && remain_processes[0].id != last_process_id)
        {
            cout << UNDERLINE << "CS = " << context_switch << CLOSE << " | ";
            time += context_switch;
            cout << time << " | ";
        }
    }
    cout << '\n';
    return {time, v};
}