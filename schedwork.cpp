#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Helper function prototype
bool backtrack(
    const AvailabilityMatrix& avail,
    size_t dailyNeed,
    size_t maxShifts,
    size_t slot,
    size_t day,
    DailySchedule& sched,
    vector<size_t>& assignedShifts
);

// Main schedule function definition
bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();

    // Size variables for matrices
    size_t n = avail.size();       // Number of days
    size_t k = avail[0].size();    // Number of workers

    // Reassign sched to n x dailyNeed (inital: -1)
    for (size_t i = 0; i < n; ++i) {
        sched.push_back(vector<Worker_T>(dailyNeed, INVALID_ID));
    }

    // Vector of number of assigned shifts for k workers (inital: 0)
    vector<size_t> assignedShifts(k, 0); // k workers

    // Call backtrack helper function
    return backtrack(avail, dailyNeed, maxShifts, 0, 0, sched, assignedShifts);
}

// Helper function definition
bool backtrack(
    const AvailabilityMatrix& avail,
    size_t dailyNeed,
    size_t maxShifts,
    size_t slot,
    size_t day,
    DailySchedule& sched,
    vector<size_t>& assignedShifts
) {
    size_t n = avail.size();
    size_t k = avail[0].size();

    // Case 1: all days scheduled
    if (day == n) {
        return true;
    }

    // Case 2: all slots filled for day, move to next day
    if (slot == dailyNeed) {
        return backtrack(avail, dailyNeed, maxShifts, 0, day + 1, sched, assignedShifts);
    }

    // Case 3: try all workers for current slot
    for (Worker_T w = 0; w < k; ++w) {
        if (avail[day][w] && assignedShifts[w] < maxShifts &&
            find(sched[day].begin(), sched[day].end(), w) == sched[day].end()) {

            sched[day][slot] = w;
            assignedShifts[w]++;

            if (backtrack(avail, dailyNeed, maxShifts, slot + 1, day, sched, assignedShifts)) {
                return true;
            }

            // Backtrack
            sched[day][slot] = INVALID_ID;
            assignedShifts[w]--;
        }
    }

    // No valid worker could be assigned
    return false;
}