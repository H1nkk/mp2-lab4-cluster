#include <iostream>
#include <iomanip>
#include "cluster.h"

using namespace std;

int main()
{
    int numPr;
    cout << "Enter number of programs: ";
    cin >> numPr;
    vector<TProgram> programs;
    for (int i = 1; i <= numPr; i++) {
        TProgram tp("A",0,0,0,0);
        if (i == 1) {
            cout << "Enter the parameters of the " << i << "st program:\n";
        }
        else if (i == 2) {
            cout << "Enter the parameters of the " << i << "nd program:\n";
        }
        else if (i == 3) {
            cout << "Enter the parameters of the " << i << "rd program:\n";
        }
        else {
            cout << "Enter the parameters of the " << i << "th program:\n";
        }
        cout << "Name: ";
        cin >> tp.name;
        cout << "Time of start: ";
        cin >> tp.tStart;
        cout << "Number of cores required: ";
        cin >> tp.p;
        cout << "Program runtime: ";
        cin >> tp.tWork;
        cout << "Probability of occurrence: ";
        cin >> tp.alpha;
        programs.push_back(tp);
    }

    int cores;
    cout << "Enter the number of cores: ";
    cin >> cores;

    int tMax;
    cout << "Enter the number of cycles to be emulated: ";
    cin >> tMax;

    short mode;
    cout << "This version of program supports 2 operating modes: \n";
    cout << "Mode 0 - each program can be added unlimited number of times.\nMode 1 - each program can only be added once\n";
    cout << "Enter the mode of the cluster: ";
    cin >> mode;

    TCluster cluster(cores, tMax, mode);
    cluster.perform(programs);

    cout << std::fixed << std::setprecision(3);
    for (int i = 0; i < tMax; i++) {
        cout << '\n';
        cout << "Log for cycle " << i << ":\n";
        LogInfo li = cluster.getLogInfo(i);
        cout << li;
    }

    return 0;
}
