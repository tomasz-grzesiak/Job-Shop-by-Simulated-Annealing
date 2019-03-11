#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <vector>
#include <utility>
#include <limits>

using namespace std;

struct Instance {
    int jobsCounter;
    int machinesCounter;
    vector<vector<pair<int, int> > > jobs;
};

Instance loadBeasley(const char *filename, int check);

Instance loadTaillard(const char *filename, int check);


#endif // PARSER_H
