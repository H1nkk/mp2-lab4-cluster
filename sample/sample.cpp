#include <iostream>
#include <iomanip>
#include "cluster.h"
#include <locale.h>

using namespace std;

int main()
{
    setlocale(LC_ALL, "rus");

    int cores, tMax;
    double spawn;
    int ppt, maxcores, maxwt;

    cout << "Введите число ядер в кластере: ";
    cin >> cores;
    cout << "Введите число тактов: ";
    cin >> tMax;
    cout << "Введите вероятность появления новой программы: ";
    cin >> spawn;
    cout << "Введите максимальное число программ, которые могут появиться за такт: ";
    cin >> ppt;
    cout << "Введите максимальное число ядер, требуемых для выполнения одной программы: ";
    cin >> maxcores;
    cout << "Введите максимальное время исполнения программы: ";
    cin >> maxwt;
    TCluster cluster(cores, tMax);
    cout << cluster.perform(spawn, ppt, maxcores, maxwt);
    return 0;
}
