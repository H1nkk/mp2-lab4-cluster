#pragma once

#include "myQueue.h"
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <list>
#include <random>
#include <ctime> 
#include <ostream>

using namespace std;

struct TProgram
{
	string name; // ID of program
	int tStart; // number of the occurrence cycle
	int p; // number of cores required
	int tWork; // program runtime
	double alpha; // random factor
};

struct LogInfo
{
	int numberOfPrograms;
	int numberOfExecuted; // number of executed programs
	int numberOfRunning; // number of currently running programs (at the end of the cycle)
	float averageLoad; // average load of the cluster
	bool operator==(const LogInfo& li) const;
	friend ostream& operator<<(ostream& os, const LogInfo& li);
};

class TCluster { // in this realization, added programs are instantly started running (on the same cycle)
	int cores;
	vector<LogInfo> log;
	int tMax; // amount of cycles to process
	short mode; // mode: 0 - each program can be added unlimited number of times, 1 - each program can only be added once 
public:
	TCluster(int cores, int tmax, short mode = 0): cores(cores), tMax(tmax), mode(mode) {
		if (cores < 16 && cores > 64) throw invalid_argument("Number of cores must be an integer in range [16, 64]");
	}

	void perform(const vector<TProgram>& programs);

	LogInfo getLogInfo(int id) const; // must be used after perform() method is used. id is starting from 0

	void switchMode(short md = -1);
};
