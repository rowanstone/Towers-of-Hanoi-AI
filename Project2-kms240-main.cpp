#include <iostream>
#include "Project2-kms240-TreeGraph.h"
using namespace std;

int main(){
    Project2_TreeGraph::RunData data;
    int n = 0;       // The number of disks
    char h;          // The char label for the heuristic function to use for the current run.
    char searchType; // The char label for the search to be conducted.

    while(n == 0)
    {
        cout << "Enter the number of disks (Value must be greater than 0): " << endl;
        cin >> n;
    }


    cout << "Enter the label for the heuristic function to be used: " << endl;
    cin >> h;

    cout << "Enter 'A' for A* Search. Enter 'R' for RBFS Search: " << endl;
    cin >> searchType;

    // Create a tree to hold game states to search.
    // Project2_TreeGraph *stateTree = new Project2_TreeGraph(n, h);
    // Create a tree to hold the game states.

    Project2_TreeGraph * stateTree = new Project2_TreeGraph(n, h);

    cout << "stateTree created in Main()" << endl;

    switch(searchType){

        case 'A':
            data = stateTree->A_StarSearch();
            cout << "Data: " << endl;
            cout << "    Expanded Nodes: " << data.numNodesExp << endl;
            cout << "    Generated Nodes: " << data.numNodesGen << endl;
            cout << "    Execution Time: " << data.execTime << " seconds" << endl;
            break;

        case 'R':
            data = stateTree->startRBFS();
            cout << "Data: " << endl;
            cout << "    Expanded Nodes: " << data.numNodesExp << endl;
            cout << "    Generated Nodes: " << data.numNodesGen << endl;
            cout << "    Execution Time: " << data.execTime << " seconds" << endl;
            break;

        default:
            cout << "Invalid search selected!" << endl;
            break;
    }
    system("pause");
    return 0;
}
