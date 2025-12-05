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


// Add prototypes for any helper functions here

// declaration for scheduleHelper, gonna need this later for the actual recursion logic
bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<size_t>& shiftsAssigned,
    size_t day,
    size_t slot);


// Add your implementation of schedule() and other helper functions here

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
    // Add your code below

    // need to set up some params for helper later
    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();

    // NEEDS TO BE RESIZED!!! WILL NOT WORK OTHERWISE
    sched.resize(numDays, std::vector<Worker_T>(dailyNeed, INVALID_ID));

    // store the actual work the helper does btwn recursions
    std::vector<size_t> shiftsAssigned(numWorkers, 0);
    
    // start us off at the very beginning of the schedule
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsAssigned, 0, 0);

}

bool scheduleHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<size_t>& shiftsAssigned,
    size_t day,
    size_t slot
){
    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();

    // base case
    if (day == numDays) {
        return true;
    }

    // get what our next spots are now before we do anything to day/slot
    // DO NOT COMMENT THIS OUT OR ALTER!!! BROKE MY CODE EARLIER!!!
    size_t nextDay = day;
    size_t nextSlot = slot + 1;
    if (nextSlot == dailyNeed) {
        nextDay = day + 1;
        nextSlot = 0;
    }

    // make sure the first worker is set, needs to be INCREASING
    Worker_T startWorker = 0;
    if (slot > 0) {
        startWorker = sched[day][slot - 1] + 1;
    }

    // not as nested but still pretty ugly, sorry!!! i tried refactoring a few times but it got wayyyy too complex
    for (Worker_T worker = startWorker; worker < numWorkers; ++worker) {
        // only schedule if available and under max shifts
        if (!avail[day][worker]) {
            continue;
        }
        if (shiftsAssigned[worker] < maxShifts) {
            continue;
        }


        sched[day][slot] = worker;
        shiftsAssigned[worker]++;

        // move on to next slot
        if (scheduleHelper(avail, dailyNeed, maxShifts, sched, shiftsAssigned, nextDay, nextSlot)) {
            return true; // send as signal to confirm we actually scheduled everything
        }

        // go back, path doesn't work
        shiftsAssigned[worker]--;
    }

    // shouldn't reach here unless there is no actual solution
    return false;
}

