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
	TProgram(string name, int ts, int p, int tw, double alpha): name(name), tStart(ts), p(p), tWork(tw), alpha(alpha) {
		if (p < 0 || alpha < 0.0 || alpha > 1.0 || ts < 0 || tw < 0) throw "Invalid member value";
	}
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

	// mode: 0 - each program can be added unlimited number of times, 1 - each program can only be added once 
	short mode; // short in case of adding new modes
public:
	TCluster(int cores, int tmax, short mode = 0): cores(cores), tMax(tmax), mode(mode) {
		if (cores < 16 || cores > 64) throw invalid_argument("Number of cores must be an integer in range [16, 64]");
		if (tmax < 0) throw invalid_argument("Number of cycles must be non-negative");
		if (mode != 0 && mode != 1) throw invalid_argument("Mode can be either 0 or 1");

	}

	void perform(const vector<TProgram>& programs);

	LogInfo getLogInfo(int id) const; // must be used after perform() method is used. id is starting from 0

	void switchMode(short md = -1);
};
