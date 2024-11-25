#pragma once

#include "myQueue.h"
#include <string>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <list>
#include <random>
#include <ctime> 

using namespace std;

struct TProgram
{
	string name; // ID of program
	int tStart; // number of the occurrence cycle
	int p; // number of needed cores
	int tWork; // program runtime
	double alpha; // random factor
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
};

class TCluster {
	vector<TProgram> programs;
	int cores;
	TQueue<TProgram> queue;
	vector<LogInfo> log;
	int tMax; // amount of cycles to обработать
public:
	TCluster(const vector<TProgram>& ops, int cores, int tmax): programs(ops), cores(cores), tMax(tmax) {
		if (cores < 16 && cores > 64) throw invalid_argument("Number of cores must be an integer in range [16, 64]");
	}

	void perform() {
		std::srand(static_cast<unsigned int>(std::time(0)));
		int currentCores = 0; // currently working cores
		log.push_back({ 0, 0, 0, 0 });
		list<TProgram> curWorking;

		// logging info
		int numberOfEx = 0; // number of currently executed programs
		int numberOfRunning = 0; // number of currently running programs
		float totalCoreUsage = 0.0; // value needed to calculate the average load of cluster
		int numberOfPr = 0;

		int curCycle = 0; // current cycle number, starting from 0
		while (curCycle < tMax) {
			curCycle++;

			double r = 0;
			while (r == 0) r = (float)(rand()) / (float)(RAND_MAX);

			for (TProgram x : programs) {
				if (r <= x.alpha) {
					queue.push(x);
					numberOfPr++;
				}
			}

			//  OK find all finished programs
			vector<string> curProgramms; // we will also find names of currently working programs
			vector<int> toErase; // vector of indexes of elements in list to be erased
			list<TProgram>::iterator pnt = curWorking.begin();
			int working = curWorking.size(); // number of currently working programs
			for (int i = 0; i < working; i++) {
				((*pnt).tWork)--;
				if ((*pnt).tWork == 0) { // program is finished
					toErase.push_back(i);
					currentCores -= (*pnt).p;
				}
				pnt++;
			}

			//  OK delete all finished programs
			pnt = curWorking.begin();
			int ind = 0; // index of next program to be erased
			for (int i = 0; i < working && ind < toErase.size(); i++) {
				if (i == toErase[ind]) {
					ind++;
					curWorking.erase(pnt); // „≈ Ќ”“№
				}
				else {
					pnt++;
				}
			}

			while (!queue.isEmpty() &&  curCycle >= queue.top().tStart) {
				if (queue.top().p <= (cores - currentCores)) {
					curWorking.push_back(queue.top());
					currentCores += queue.top().p;
					totalCoreUsage += queue.top().p;
					queue.pop();
				}
				else {
					break;
				}
			}

			// заполнить log
			LogInfo li = { numberOfPr, numberOfEx, numberOfRunning, totalCoreUsage / (float)(cores * curCycle) };
			log.push_back(li);
		}
	}

	LogInfo getLogInfo(int id) const {
		if (id < 0 || id >= log.size()) {
			throw std::out_of_range("Invalid log index");
		}
		return log[id];
	}

};
