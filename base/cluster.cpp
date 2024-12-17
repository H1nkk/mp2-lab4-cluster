#include "cluster.h"

int getRnd(int l, int r) {
	return rand() % (r - l + 1) + l;
}

bool LogInfo::operator==(const LogInfo& li) const {
	return
		li.numberOfPrograms == numberOfPrograms &&
		li.numberOfExecuted == numberOfExecuted &&
		li.numberOfRunning == numberOfRunning &&
		fabs(li.averageLoad - averageLoad) <= 1e-6; // to prevent errors caused by rounding
}

ostream& operator<<(ostream& os, const LogInfo& li) {
	os << "Total number of programs: " << li.numberOfPrograms << '\n';
	os << "Number of executed programs: " << li.numberOfExecuted << '\n';
	os << "Number of running programs: " << li.numberOfRunning << '\n';
	os << "Average load of the cluster at this point: " << li.averageLoad << '\n';
	return os;
}

LogInfo TCluster::perform(double spawnChance, int ppt, int maxCores, int maxWorkTime) { // ppt is programs per tact
	if (spawnChance <= 0.0 || spawnChance > 1.0 || ppt <= 0 || maxCores < 1 || maxWorkTime < 1)
		throw "Invalid perform arguments";
	std::srand(std::time(0));
	int currentCores = 0; // currently working cores
	list<TProgram> curWorking;
	TQueue<TProgram> queue;

	// logging info
	int numberOfEx = 0; // number of currently executed programs
	int numberOfRunning = 0; // number of currently running programs
	float totalCoreUsage = 0.0; // value needed to calculate the average load of cluster
	int numberOfPr = 0;

	int progId = 0;
	int curCycle = 0; // current cycle number, starting from 0
	while (curCycle < tMax) {

		for (int i = 0; i < ppt; i++) {
			double r = (float)(rand()) / (float)(RAND_MAX);
			if (r <= spawnChance) { // create program
				numberOfPr++;
				string name = "ID" + to_string(progId);
				int tStart = curCycle;
				int p = getRnd(1, maxCores);
				int tWork = getRnd(1, maxWorkTime);
				TProgram prog(name, tStart, p, tWork);
				queue.push(prog);
			}
			
		}

		// Find all finished programs
		vector<int> toErase; // vector of indexes of elements in list to be erased
		list<TProgram>::iterator pnt = curWorking.begin();
		int working = curWorking.size(); // number of currently working programs
		for (int i = 0; i < working; i++) {
			((*pnt).tWork)--;
			if ((*pnt).tWork == 0) { // program is finished
				toErase.push_back(i);
				numberOfEx++;
				numberOfRunning--;
				currentCores -= (*pnt).p;
			}
			else {
				totalCoreUsage += (*pnt).p;
			}
			pnt++;
		}

		// Delete all finished programs
		pnt = curWorking.begin();
		int ind = 0; // index of next program to be erased
		for (int i = 0; i < working && ind < toErase.size(); i++) {
			if (i == toErase[ind]) {
				ind++;
				pnt = curWorking.erase(pnt);
			}
			else {
				pnt++;
			}
		}

		// Try to start running programs
		while (!queue.isEmpty() && curCycle >= queue.top().tStart) {
			if (queue.top().p <= (cores - currentCores)) {
				curWorking.push_back(queue.top());
				currentCores += queue.top().p;
				totalCoreUsage += queue.top().p;
				numberOfRunning++;
				queue.pop();
			}
			else {
				if (queue.top().p > cores) queue.pop();
				else break;
			}
		}

		// Create new log

		curCycle++;
	}

	LogInfo li = { numberOfPr, numberOfEx, numberOfRunning, totalCoreUsage / (float)(cores * curCycle) };

	return li;
}