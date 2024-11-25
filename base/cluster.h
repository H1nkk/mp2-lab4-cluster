#pragma once

#include "myQueue.h"
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <list>

using namespace std;

struct TProgram
{
	string name; // ID of program
	int tStart; // number of the occurrence cycle
	int p; // number of needed cores
	int tWork; // program runtime
};

bool compareTProgramByStartingTimeAndP(TProgram a, TProgram b) { // мб заменить аргументы на ссылки
	if (a.tStart == b.tStart) {
		return a.p < b.p;
	}
	return a.tStart < b.tStart;
}

struct LogInfo
{
	int numberOfPrograms;
	int numberOfExecuted; // number of executed programs
	int numberOfRunning; // number of currently running programs (at the end of the cycle)
	float averageLoad; // average load of the cluster
	vector<string> currentPrograms;
};

class TCluster {
	vector<TProgram> programs;
	int cores;
	TQueue<TProgram> queue;
	vector<LogInfo> log;
	
public:
	TCluster(const vector<TProgram>& ops, int cores): programs(ops), cores(cores) {}

	void perform() {
		sort(programs.begin(), programs.end(), compareTProgramByStartingTimeAndP);
		for (auto x : programs) {
			if (x.p > cores) {
				string report = "Not enough cores to do program " + x.name;
				throw report;
			}
			queue.push(x);
		}
		int time = 0; // current cycle number, starting from 0
		int currentCores = 0; // currently working cores
		float totalCoreUsage = 0.0; // value needed to calculate the average load of cluster
		int numberOfEx = 0; // number of currently executed programs
		int numberOfRunning = 0; // number of currently running programs
		log.push_back({ 0, 0, 0, 0 });
		if (queue.isEmpty()) return;
		TProgram curPr = queue.top();
		list<TProgram> curWorking;

		bool keepGoing = true; // false if there is no more programs in queue
		while (true) {
			time++;

			// find all already completed programs
			vector<int> toErase;
			list<TProgram>::iterator pnt = curWorking.begin();
			int working = curWorking.size(); // number of currently working programs
			for (int i = 0; i < working; i++) {
				((*pnt).tWork)--;
				if ((*pnt).tWork == 0) { // program is completed
					toErase.push_back(i);
					currentCores -= (*pnt).p;
				}
				pnt++;
			}

			// delete all already completed programs
			pnt = curWorking.begin();
			int ind = 0; // index of next program to be erased
			for (int i = 0; i < working && ind < toErase.size(); i++) {
				if (i == toErase[ind]) {
					ind++;
					curWorking.erase(pnt);
				}
				else {
					pnt++;
				}
			}


			while (keepGoing && time >= curPr.tStart) {
				if (curPr.p <= (cores - currentCores)) {
					curWorking.push_back(curPr);
					currentCores += curPr.p;
					if (!queue.isEmpty()) {
						curPr = queue.pop();
					}
					else {
						keepGoing = false;
					}
				}
				else {
					break;
				}
			}

			// заполнить log
		}
	}

	LogInfo getLogInfo(int id) const {
		if (id < 0 || id >= log.size()) {
			throw std::out_of_range("Invalid log index");
		}
		return log[id];
	}

};

