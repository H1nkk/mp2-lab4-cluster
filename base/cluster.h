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

int getRnd(int l, int r);

struct TProgram
{
	string name; // ID of program
	int tStart; // number of the occurrence cycle
	int p; // number of cores required
	int tWork; // program runtime
	TProgram(string name, int ts, int p, int tw): name(name), tStart(ts), p(p), tWork(tw) {
		if (p < 0 || ts < 0 || tw < 0) throw "Invalid member value";
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
	int tMax; // amount of cycles to process

public:
	TCluster(int cores, int tmax): cores(cores), tMax(tmax) {
		if (cores < 16 || cores > 64) throw invalid_argument("Number of cores must be an integer in range [16, 64]");
		if (tmax <= 0) throw invalid_argument("Number of cycles must be positive");
	}

	LogInfo perform(double spawnChance, int ppt, int maxCores, int maxWorkTime);

};
