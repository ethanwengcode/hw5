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
bool scheduleHelper(
  const AvailabilityMatrix& avail,
  const size_t dailyNeed,
  const size_t maxShifts,
  DailySchedule& sched,
  size_t day,
  vector<int>& numShifts
);

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

    size_t days = avail.size(); // number of days
    size_t workers = avail[0].size(); // number of workers 

    sched.resize(days); // resize for "days" number of rows
    vector<int> numShifts(workers, 0); // tracks the number of shifts for each worker
    bool success = scheduleHelper(avail, dailyNeed, maxShifts, sched, 0, numShifts);
    return success;
}

bool scheduleHelper(
  const AvailabilityMatrix& avail,
  const size_t dailyNeed,
  const size_t maxShifts,
  DailySchedule& sched,
  size_t day,
  vector<int>& numShifts
) {
  if(day == sched.size()) { // finished scheduling for every day; done
    return true; 
  }

  if(sched[day].size() == dailyNeed) { // current day is filled with shifts, go to next day 
    return scheduleHelper(avail, dailyNeed, maxShifts, sched, day +1, numShifts);
  }

  // itereate through each worker
  for(size_t w = 0; w < avail[0].size(); w++) {
    // is the current worker available ?
    bool workerAvail = avail[day][w];
    if( !workerAvail ) {
      continue;  // go to next worker
    }
    // has the current worker reached their shift limit?
    int workerNumShift = numShifts[w];
    if(maxShifts <= workerNumShift) {
      continue;
    }
    // has the current worker already been schedule for this day?
    bool workerScheduled = false;
    for(vector<Worker_T>::iterator it = sched[day].begin(); it!= sched[day].end(); ++it) {
      if(*it == w){
        workerScheduled = true;
        break;
      }
    }
    if(workerScheduled) {
      continue;
    }

    sched[day].push_back(w); // schedule current worker for this day
    numShifts[w]++; // increment curr worker's shifts

    // able to schedule for all other days 
    if(scheduleHelper(avail, dailyNeed, maxShifts, sched, day, numShifts)) {
      return true;
    }

    sched[day].pop_back(); // backtrack
    numShifts[w]--; // decrement curr worker's shifts
  }
  return false; // indicate that we weren't able to schedule for all days
}