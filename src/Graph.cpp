#include <algorithm>
#include <fstream>
#include "../include/Graph.h"

Graph::Graph(Instance inst)
{
    verticesCounter = 0;
    jobsCounter = inst.jobsCounter;
    machinesCounter = inst.machinesCounter;
    data.push_back(Vertex(0));
    verticesCounter++;
    sameMachines.resize(inst.machinesCounter);
    for(int i=0;i<inst.jobsCounter;i++)
        data[0].neighbours.insert((i*inst.machinesCounter)+1);
    for (int i = 0; i < inst.jobsCounter; i++)
    {
        for (int j = 0; j < inst.machinesCounter; j++)
        {
            data.push_back(Vertex(inst.jobs[i][j].second));
            sameMachines[inst.jobs[i][j].first].insert(verticesCounter);
            if(j!=inst.machinesCounter-1)
                data[verticesCounter].neighbours.insert(verticesCounter+1);
            else
                data[verticesCounter].neighbours.insert(inst.jobsCounter*inst.machinesCounter+1);
            verticesCounter++;
        }
    }
    data.push_back(Vertex(0));
    for(int i=0;i<machinesCounter;i++)
        for(int j : sameMachines[i])
            for(int k : sameMachines[i])
                if(k != j && data[k].neighbours.find(j) == data[k].neighbours.end())
                    data[j].neighbours.insert(k);

}

Graph::~Graph()
{
    //dtor
}

void Graph::print()
{
    for (int i =0;i<=verticesCounter;i++)
    {
        cout<<"Numer: "<<i<<". Czas: "<<data[i].time<<endl;
        for(int j : data[i].neighbours)
            cout<<j<<" ";
        cout<<endl;
    }
}

void Graph::visit(int vertex, vector<int> &visited)
{
    if(visited[vertex] == 2)
        return;
    if(visited[vertex] == 1)
        throw(string("Not a DAG"));
    visited[vertex] = 1;
    for(int i : data[vertex].neighbours)
        visit(i, visited);
    visited[vertex] = 2;
    topologicallySorted.push_back(vertex);
}

void Graph::topologicalSort()
{
    topologicallySorted.clear();
    vector<int> visited;
    visited.resize(data.size(), 0);
    for(int i=0;i<data.size();i++)
        visit(i, visited);
    reverse(topologicallySorted.begin(), topologicallySorted.end());
    //for(int a : topologicallySorted)
      //  cout<<a<<" ";
    //cout<<endl;
}

vector<int> Graph::calculateDistancesAndLongestPath()
{
    vector<int> distances;
    distances.resize(data.size(), -1);
    distances[0] = 0;
    vector<int> followed(data.size());
    for(auto a = topologicallySorted.begin(); a != topologicallySorted.end(); a++)
        for(auto b = data[*a].neighbours.begin(); b != data[*a].neighbours.end(); b++)
            if(distances[*b] < data[*b].time + distances[*a])
            {
                distances[*b] = data[*b].time + distances[*a];
                followed[*b] = *a;
            }
    longestPath.clear();
    int tmp = jobsCounter*machinesCounter+1;
    longestPath.push_back(tmp);
    while(tmp!=0)
    {
        longestPath.push_back(followed[tmp]);
        tmp = followed[tmp];
    }
    reverse(longestPath.begin(), longestPath.end());
    //cout<<"Distances: ";
    //for(int i : distances)
      //  cout<<i<<" ";
   // cout<<endl;
    //for(int i=0;i<longestPath.size(); i++)
       //cout<<longestPath[i]<<" ";
    //cout<<endl;
    return distances;
}

int Graph::calculateLength()
{
    int length = 0;
    for(auto a = longestPath.begin(); a != longestPath.end(); a++)
        length += data[*a].time;
    //cout<<"Length: "<<length<<endl;
    return length;
}

vector<pair<int, int> > Graph::findEdgesToReverse()
{
    vector<pair<int, int> > edges;
    edges.clear();
    for(int i=0; i<longestPath.size()-1;++i)
        for(set<int> a : sameMachines)
            if(a.find(longestPath[i])!=a.end() && a.find(longestPath[i+1])!=a.end())
            {
                edges.push_back(make_pair(longestPath[i], longestPath[i+1]));
                break;
            }
    return edges;
}

void Graph::flipEdge(pair<int, int> edge)
{
    if(data[edge.first].neighbours.find(edge.second) != data[edge.first].neighbours.end())
    {
        data[edge.first].neighbours.erase(edge.second);
        data[edge.second].neighbours.insert(edge.first);
    }
    else
    {
        data[edge.second].neighbours.erase(edge.first);
        data[edge.first].neighbours.insert(edge.second);
    }
}

void Graph::simulatedAnnealing(double temp, double alpha, int maxIter, int maxTime)
{
    topologicalSort();
    vector<int> distances = calculateDistancesAndLongestPath();
    finalDistances = distances;
    int length = calculateLength();
    finalLength = length;
    finalLongestPath = longestPath;
    auto beginTime = chrono::system_clock::now();
    for(int i=0;i<maxIter;i++)
    {
        //cout<<i<<endl;
        vector<pair<int, int> > edges = findEdgesToReverse();
        pair<int, int> theOne;
        if(edges.empty())
            break;
        else
        {
           theOne = edges[rand()%edges.size()];
        }
        bool bad = true;
        while(bad){
            bad = false;
            try{
                flipEdge(theOne);
                topologicalSort();
            }catch(string e)
            {
                flipEdge(theOne);
                bad = true;
                theOne = edges[rand()%edges.size()];
            }
        }
        vector<int> distances2 = calculateDistancesAndLongestPath();
        int length2 = calculateLength();
        if(length2 < length)
        {
            length = length2;
            if(length2 < finalLength)
            {
                finalLength = length2;
                finalDistances = distances2;
                finalLongestPath = longestPath;
            }
        }
        else
        {
            if(exp((double)(length-length2)/temp) > (double(rand()%10001))/10000)
                length = length2;
            else
                flipEdge(make_pair(theOne.second, theOne.first));
        }
        temp = temp*alpha;
        //cout<<finalLength<<endl;
        if(chrono::duration<double>(chrono::system_clock::now()-beginTime).count() > maxTime)
            break;
    }
}

void Graph::save()
{
    ofstream plik("answers/ans3.txt");
    plik<<finalLength<<endl;
    for(int i=0;i<jobsCounter;i++)
    {
        for(int j=i*machinesCounter+1;j<=(i+1)*machinesCounter;j++)
            plik<<finalDistances[j]-data[j].time<<" ";
        plik<<endl;
    }
    plik.close();
}
