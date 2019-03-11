#include <iostream>
#include <fstream>
#include <vector>
#include <utility>
#include <limits>
#include <string.h>
#include <ctime>
#include <chrono>
#include <cstdlib>

using namespace std;

int jobsCounter, machinesCounter, JobShopLength = 0;
vector<vector<pair<int, int>>> jobs;
vector<int> vertex;
vector<pair<int, int>> edges;
vector<pair<int, int>> same_machines;

void loadBeasley(const char *filename, int check)
{
    ifstream plik(filename);
    plik >> jobsCounter >> machinesCounter;
    if (check > -1 && check < jobsCounter)
        jobsCounter = check;
    jobs.resize(jobsCounter);
    for (int i = 0; i < jobsCounter; i++)
    {
        jobs[i].resize(machinesCounter);
        for (int j = 0; j < machinesCounter; j++)
            plik >> jobs[i][j].first >> jobs[i][j].second;
    }
    plik.close();
}

void loadTaillard(const char *filename, int check)
{
    ifstream plik(filename);
    plik >> jobsCounter >> machinesCounter;

    jobs.resize(jobsCounter);
    plik.ignore(numeric_limits<streamsize>::max(), '\n');
    plik.ignore(numeric_limits<streamsize>::max(), '\n');
    plik.clear();
    for (int i = 0; i < jobsCounter; i++)
    {
        jobs[i].resize(machinesCounter);
        for (int j = 0; j < machinesCounter; j++)
            plik >> jobs[i][j].second;
    }
    if (check > -1 && check < jobsCounter)
        jobsCounter = check;
    jobs.resize(jobsCounter);
    plik.ignore(numeric_limits<streamsize>::max(), '\n');
    plik.ignore(numeric_limits<streamsize>::max(), '\n');
    for (int i = 0; i < jobsCounter; i++)
        for (int j = 0; j < machinesCounter; j++)
        {
            plik >> jobs[i][j].first;
            jobs[i][j].first--;
        }
    plik.close();
}

void add_vertex()
{

    for (int i = 0; i < jobsCounter; i++)
    {
        for (int j = 0; j < machinesCounter; j++)
        {
            vertex.push_back(jobs[i][j].second);
        }
    }
}

void dodaj_edges()
{

    for (int i = 0; i < jobsCounter; i++)
    {
        edges.push_back(make_pair(0, jobs[i][0].second));

        for (int j = 1; j < (machinesCounter); j++)
        {
            edges.push_back(make_pair(jobs[i][j - 1].second, jobs[i][j].second));
        }
        edges.push_back(make_pair(jobs[i][machinesCounter - 1].second, 0));
    }
}

void dodaj_do_same_machines()
{

    vector<int> v;
    for (int machine = 0; machine < machinesCounter; machine++)
    {
        for (int j = 0; j < jobsCounter; j++)
        {
            for (int k = 0; k < machinesCounter; k++)
            {
                if (jobs[j][k].first == machine)
                {
                    v.push_back(jobs[j][k].second);
                }
            }
        }

        for (int x = 1; x < v.size(); x++)
        {
            same_machines.push_back(make_pair(v[x - 1], v[x]));
        }
        same_machines.push_back(make_pair(v[v.size() - 1], v[0]));

        v.clear();
    }
}

int main(const int argc, const char *argv[])
{
    for (int i = 0; i < 10; i++)
    {
        if (argc < 3)
        {
            cout << "Uzycie: " << argv[0] << " [b|t] plik_z_danymi <liczba_zadan>\n";
            return 0;
        }

        int howManyJobs;
        if (argc == 3)
            howManyJobs = -1;
        else
            howManyJobs = atoi(argv[3]);
        char type = *argv[1];
        if (type == 'b')
            loadBeasley(argv[2], howManyJobs);
        else if (type == 't')
            loadTaillard(argv[2], howManyJobs);
        else
        {
            cout << "Nieznany format pliku z danymi\n";
            return 0;
        }
        int iterator = jobsCounter * machinesCounter;
        add_vertex();
        dodaj_edges();
        dodaj_do_same_machines();

        for (int i = 0; i < iterator; i++)
        {

            cout << same_machines[i].first << ' ' << same_machines[i].second << ' ';
            if ((i + 1) % 5 == 0)
            {
                cout << endl;
            }
        }

        return 0;
    }
}
