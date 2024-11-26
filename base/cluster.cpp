#include "cluster.h"

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

void TCluster::perform(const vector<TProgram>& programs) {
	log.clear();
	std::srand(static_cast<unsigned int>(std::time(0)));
	int currentCores = 0; // currently working cores
	list<TProgram> curWorking;
	TQueue<TProgram> queue;
	vector<bool> isAdded;

	if (mode == 1) {
		isAdded.resize(programs.size());
		for (int i = 0; i < programs.size(); i++) {
			isAdded[i] = false;
		}
	}

	// logging info
	int numberOfEx = 0; // number of currently executed programs
	int numberOfRunning = 0; // number of currently running programs
	float totalCoreUsage = 0.0; // value needed to calculate the average load of cluster
	int numberOfPr = 0;

	int curCycle = 0; // current cycle number, starting from 0
	while (curCycle < tMax) {

		for (int i = 0; i < programs.size(); i++) {
			if (mode == 1)
				if (isAdded[i]) continue;

			TProgram x = programs[i];

			double r = 0;
			while (r == 0) r = (float)(rand()) / (float)(RAND_MAX);
			if (r <= x.alpha) {
				queue.push(x);
				numberOfPr++;
				if (mode == 1)
					isAdded[i] = true;
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
				pnt = curWorking.erase(pnt); // веймсрэ
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
				break;
			}
		}

		// create new log
		LogInfo li = { numberOfPr, numberOfEx, numberOfRunning, totalCoreUsage / (float)(cores * (curCycle + 1)) };
		log.push_back(li);

		curCycle++;
	}

}

LogInfo TCluster::getLogInfo(int id) const { 
	if (id < 0 || id >= log.size()) {
		throw std::out_of_range("Invalid log index");
	}
	return log[id];
}

void TCluster::switchMode(short md) {
	if (md == 0 || md == 1) {
		mode = md;
	}
	else {
		mode = !mode;
	}
}