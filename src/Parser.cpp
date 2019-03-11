#include "../include/Parser.h"

Instance loadBeasley(const char *filename, int check)
{
    Instance inst;
    ifstream plik(filename);
    plik >> inst.jobsCounter >> inst.machinesCounter;
    if (check > -1 && check < inst.jobsCounter)
        inst.jobsCounter = check;
    inst.jobs.resize(inst.jobsCounter);
    for (int i = 0; i < inst.jobsCounter; i++)
    {
        inst.jobs[i].resize(inst.machinesCounter);
        for (int j = 0; j < inst.machinesCounter; j++)
            plik >> inst.jobs[i][j].first >> inst.jobs[i][j].second;
    }
    plik.close();
    return inst;
}

Instance loadTaillard(const char *filename, int check)
{
    Instance inst;
    ifstream plik(filename);
    plik >> inst.jobsCounter >> inst.machinesCounter;

    inst.jobs.resize(inst.jobsCounter);
    plik.ignore(numeric_limits<streamsize>::max(), '\n');
    plik.ignore(numeric_limits<streamsize>::max(), '\n');
    plik.clear();
    for (int i = 0; i < inst.jobsCounter; i++)
    {
        inst.jobs[i].resize(inst.machinesCounter);
        for (int j = 0; j < inst.machinesCounter; j++)
            plik >> inst.jobs[i][j].second;
    }
    if (check > -1 && check < inst.jobsCounter)
        inst.jobsCounter = check;
    inst.jobs.resize(inst.jobsCounter);
    plik.ignore(numeric_limits<streamsize>::max(), '\n');
    plik.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int i = 0; i < inst.jobsCounter; i++)
        for (int j = 0; j < inst.machinesCounter; j++)
        {
            plik >> inst.jobs[i][j].first;
            inst.jobs[i][j].first--;
        }
    plik.close();
    return inst;
}
