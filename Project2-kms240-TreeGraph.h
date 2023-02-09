#ifndef PROJECT2_PROJECT2_TREEGRAPH_H
#define PROJECT2_PROJECT2_TREEGRAPH_H
#include <vector>
#include <list>
#include <chrono>
#include <math.h>
#include <algorithm>
#include "Project2-kms240-PegStack.h"

using namespace std;
using namespace std::chrono;

class Project2_TreeGraph {
private:
    // For each tree node
    struct PegState
    {
        vector<int> discList;
        int discSum;
    };

    struct GraphNode
    {
        int nodeIndex;
        PegState stateA;
        PegState stateB;
        PegState stateC;
        vector<PegState> state;         // Vector to hold the current state of the disks on the three pegs

        Project2_PegStack Peg_A = Project2_PegStack('A');
        Project2_PegStack Peg_B = Project2_PegStack('B');
        Project2_PegStack Peg_C = Project2_PegStack('C');

        // Goldstein's addition for RBFS implementation
        int moveValue;                  //This shows the direction the disk was moved(-1:Left, 1:Right);
        int diskMoved;                  //This holds the disk that was moved
        vector<GraphNode *> succ;       //Holds the Successors of the tree

        int heuristicValue = 0;         // Heuristic value assigned to a node from a heuristic function - h(n)
        int cumulativeCost = 0;         // The cumulative cost from the root node to the current node - g(n)
        int totalCost = 0;              // The total cost estimated from root node to goal node F(n) = g(n) + h(n)
        GraphNode *parent;              // The parent node of the current node
        GraphNode *left;                // Pointer to the left child node. (if it exists)
        GraphNode *middle;              // Pointer to the middle child node (if it exists)
        GraphNode *right;               // Pointer to the right child node (if it exists)
        vector<GraphNode*> Successors;  // List of successors for this node
    };

    void setIndex(GraphNode &, int);                 // set the node index
    void setInitState(int);                          // set the node's state
    void setHeuristicValue(GraphNode &, int);        // set the Heuristic value for the node
    int getCumulativeCost(GraphNode &);              // retrieve the cumulative cost for the node
    void setCumulativeCost(GraphNode &, int);        // set the cumulative cost for the node
    int getTotalCost(GraphNode &);                   // get the total estimated cost for the node
    void setTotalCost(GraphNode &);                  // set the total estimated cost for the node
    GraphNode* getParent(GraphNode &);               // retrieve parent node
    void setParent(GraphNode &, GraphNode *);        // assign the parent node
    GraphNode* getLeftChild(GraphNode &);            // retrieve left child node
    void setLeftChild(GraphNode &, GraphNode *);     // assign the left child node
    GraphNode* getMiddleChild(GraphNode &);          // retrieve middle child node
    void setMiddleChild(GraphNode &, GraphNode *);   // assign the middle child node
    GraphNode* getRightChild(GraphNode &);           // retrieve right child node
    void setRightChild(GraphNode &, GraphNode *);    // assign the right child node
    void determineHeuristic(GraphNode &, int, char); // determine the heuristic value of the node based on the state it
                                                     // represents.
    // For the tree
    vector<GraphNode * > treeNodes;  // Vector containing all the nodes that have been generated in the tree
    vector<GraphNode *> open;        // Open list for search algorithm
    vector<GraphNode *> closed;      // Closed list for search algorithm

    // PLACE GOAL STATE DEFINITION HERE FOR COMPARISON DURING SEARCH
    PegState goal_A;
    PegState goal_B;
    PegState goal_C;
    vector<PegState> goalState;
    GraphNode *root;                 // Pointer to the root node of the tree.
    GraphNode* StartNode;            // Pointer to the starting node on the tree.
    GraphNode* Old;
    GraphNode* Successor;

    int n;                           // Number of discs for the game
    char h;                          // Heuristic label

    int nodeIndexCounter = 0;
    int moveCounter = 0;
    int generated = 0;
    int expanded = 0;

public:
    struct RunData
    {
        double execTime;             // Execution time for search
        int numNodesGen;             // Number of nodes generated on the three
        int numNodesExp;             // Number of nodes expanded on the tree
    };

    bool failureRBFS = false;
    bool solutionFound = false;

    Project2_TreeGraph(int, char);   // Constructor
    ~Project2_TreeGraph();           // Destructor
    bool moveDisc(Project2_PegStack &, Project2_PegStack &);
    void generateChildren(GraphNode *, int);
    bool checkOpenList(GraphNode *, int);
    bool checkClosedList(GraphNode *, int);
    void placeOnOpen(GraphNode *, int);
    void depthFirstSearch(GraphNode *);
    RunData A_StarSearch();
    void setSuccessor(GraphNode *, int);
    RunData startRBFS();
    GraphNode RBFS(GraphNode *, int);
    static bool sortByF_Value(const GraphNode*, const GraphNode*);
};

#endif //PROJECT2_PROJECT2_TREEGRAPH_H