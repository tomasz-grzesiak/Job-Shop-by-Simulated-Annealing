#include <iostream>
#include <cstdlib>
#include <ctime>
#include "include/Graph.h"

using namespace std;

int main(const int argc, const char *argv[])
{
    if (argc < 6) {
        cout << "Uzycie: " << argv[0] << " [b|t] plik_z_danymi temparatura_startowa wspolczynnik_chlodzenia liczba_iteracji <liczba_zadan>\n";
        return 0;
    }

    int howManyJobs;
    char type = *argv[1];

    if (argc == 6)
        howManyJobs = -1;
    else
        howManyJobs = atoi(argv[6]);

    Instance inst;
    if (type == 'b')
        inst = loadBeasley(argv[2], howManyJobs);
    else if (type == 't')
        inst = loadTaillard(argv[2], howManyJobs);
    else {
        cout << "Nieznany format pliku z danymi\n";
        return 0;
    }
    srand(time(NULL));
    Graph g(inst);
    //g.print();
    auto t1 = chrono::system_clock::now();
    g.simulatedAnnealing((double)atof(argv[3]), (double)atof(argv[4]), atoi(argv[5]));
    auto t2 = chrono::system_clock::now();
    double executionTime = chrono::duration<double>(t2-t1).count();
    cout<<g.finalLength<<endl;
    cout<<executionTime<<endl;
    g.save();
    return 0;
}

