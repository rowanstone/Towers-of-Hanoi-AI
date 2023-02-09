#include "Project2-kms240-TreeGraph.h"
//**********************************************************************************************************************
// Project2_TreeGraph() - Constructor function
//**********************************************************************************************************************
Project2_TreeGraph::Project2_TreeGraph(int n, char h)
{
    Old = nullptr;
    Successor = nullptr;
    // Create the goal state for each peg
    this->h = h;
    this->n = n;

    // Create a tree with a root node based on n.
    root = new GraphNode;
    int disc;
    // Place n disks on Peg_A, and generate the disc value list for the goal_B.
    int discArray[n];


    for(int i = 0; i < n; i++)
    {
        disc = n-i;
        discArray[i] = disc;
    }

    for(int j = 0; j < n ; j++)
    {
        root->Peg_A.push(discArray[j]);
    }

    goal_A.discList = root->Peg_B.getDiscList();    // Goal_A is empty, as is initial B.
    goal_A.discSum = root->Peg_B.getPegSum();
    goal_B.discList = root->Peg_A.getDiscList();    // Goal_B needs to look like initial A.
    goal_B.discSum = root->Peg_A.getPegSum();
    goal_C.discList = root->Peg_C.getDiscList();    // Goal_C is empty, just like initial C.
    goal_C.discSum = root->Peg_C.getPegSum();

    // Create the goal state as a composition of all three peg goals.
    goalState = {goal_A, goal_B, goal_C};

    setIndex(*root, nodeIndexCounter);
    setParent(*root, root);
    setInitState(n);       // Initialize the left peg with n disks and set the initial state
    setMiddleChild(*root, nullptr);          // Initial state only has two possible moves (right or left).

    treeNodes.push_back(root);
    StartNode = root;          // Set the starting node for search at the root node.
}
//**********************************************************************************************************************
// Destructor
//**********************************************************************************************************************
Project2_TreeGraph::~Project2_TreeGraph()
{
    for(int i = 0; i < open.size(); i++){
        delete(open[i]);
    }
    for(int j = 0; j < closed.size(); j++){
        delete(closed[j]);
    }
    delete(StartNode);
    delete(Old);
    delete(Successor);
}
//**********************************************************************************************************************
// setIndex() takes an integer value and sets it as the node index value
//**********************************************************************************************************************
void Project2_TreeGraph::setIndex(GraphNode &node, int index)
{
    node.nodeIndex = index;
}
//**********************************************************************************************************************
// setState() returns the node index value
//**********************************************************************************************************************
void Project2_TreeGraph::setInitState(int n)
{
    root->stateA.discList = root->Peg_A.getDiscList();
    root->stateA.discSum = root->Peg_A.getPegSum();
    root->stateB.discList = root->Peg_B.getDiscList();
    root->stateB.discSum = root->Peg_B.getPegSum();
    root->stateC.discList = root->Peg_C.getDiscList();
    root->stateC.discSum = root->Peg_C.getPegSum();

    root->state = {root->stateA, root->stateB, root->stateC};
}
//**********************************************************************************************************************
// setHeuristicValue() takes an integer value and sets it as the node index value - h(n)
//**********************************************************************************************************************
void Project2_TreeGraph::setHeuristicValue(GraphNode &node, int h)
{
    node.heuristicValue = h;
}
//**********************************************************************************************************************
// getCumulativeCost() returns the cumulative cost for the node.
//**********************************************************************************************************************
int Project2_TreeGraph::getCumulativeCost(GraphNode &node)
{
    return node.cumulativeCost;
}
//**********************************************************************************************************************
// setCumulativeCost() takes an integer value and sets it as the cumulative cost for the node - g(n)
//**********************************************************************************************************************
void Project2_TreeGraph::setCumulativeCost(GraphNode &node, int cost)
{
    // For Goldstein's heuristic
    node.cumulativeCost = cost;
    if(h == '2')
        node.cumulativeCost = moveCounter;
}
//**********************************************************************************************************************
// getTotalCost() returns the cumulative cost for the node.
//**********************************************************************************************************************
int Project2_TreeGraph::getTotalCost(GraphNode &node)
{
    return node.totalCost;
}
//**********************************************************************************************************************
// setCumulativeCost() takes an integer value and sets it as the cumulative cost for the node - F(n)
//**********************************************************************************************************************
void Project2_TreeGraph::setTotalCost(GraphNode &node)
{
    node.totalCost = node.cumulativeCost + node.heuristicValue; // F(n) = g(n) + h(n)
}
//**********************************************************************************************************************
// getParent() returns the parent node of the current node
//**********************************************************************************************************************
Project2_TreeGraph::GraphNode* Project2_TreeGraph::getParent(GraphNode &node)
{
    return node.parent;
}
//**********************************************************************************************************************
// setParent() assigns the parent node for the current node
//**********************************************************************************************************************
void Project2_TreeGraph::setParent(GraphNode &node, GraphNode * pnode)
{
    node.parent = pnode;
}
//**********************************************************************************************************************
// getLeftChild() returns the left child node of the current node
//**********************************************************************************************************************
Project2_TreeGraph::GraphNode * Project2_TreeGraph::getLeftChild(GraphNode &node)
{
    return node.left;
}
//**********************************************************************************************************************
// setLeftChild() assigns the parent node for the current node
//**********************************************************************************************************************
void Project2_TreeGraph::setLeftChild(GraphNode &node, GraphNode * child)
{
    node.left = child;
}
//**********************************************************************************************************************
// getMiddleChild() returns the middle child node of the current node
//**********************************************************************************************************************
Project2_TreeGraph::GraphNode* Project2_TreeGraph::getMiddleChild(GraphNode &node)
{
    return node.middle;
}
//**********************************************************************************************************************
// setMiddleChild() assigns the middle child node for the current node
//**********************************************************************************************************************
void Project2_TreeGraph::setMiddleChild(GraphNode &node, GraphNode * child)
{
    node.middle = child;
}
//**********************************************************************************************************************
// getRightChild() returns the right child node of the current node
//**********************************************************************************************************************
Project2_TreeGraph::GraphNode* Project2_TreeGraph::getRightChild(GraphNode &node)
{
    return node.right;
}
//**********************************************************************************************************************
// setRightChild() assigns the right child node for the current node
//**********************************************************************************************************************
void Project2_TreeGraph::setRightChild(GraphNode &node, GraphNode  * child)
{
    node.right = child;
}
//**********************************************************************************************************************
// determineHeuristic() will generate the heuristic value for the current node based on the current state and the
// heuristic function specified by the char value passed as an argument
//**********************************************************************************************************************
void Project2_TreeGraph::determineHeuristic(GraphNode &node, int n, char h)
{
    int h_value = 0;
    int nSum = 0;                 // Sum of values of all n discs. Optional variable for use in heuristic function.
    int nextMove = moveCounter+1; // Next Move Value in Goldstein's heuristic function (Case '2')
    int mNum;                     // Move Value in Goldstein's heuristic function (Case '2')
    int dNum;                     // Disk Value in Goldstein's heuristic function (Case '2')
    double equation =(nextMove - pow(2,node.diskMoved - 1))/pow(2,node.diskMoved) + 1;
                                  // Move Number in Goldstein's heuristic function (Case '2')
    int temp = 0;                 // Temp Value in Kadon's heuristic (Case '3')

    for(int i = 0; i < n; i++)
    {
        nSum += n - i;
    }

    switch(h){

        case '1':

            cout << "Rowan's heuristic selected." << endl;
            h_value = nSum - node.state[1].discSum;
            break;

        case '2':

            //The heuristic comes in two parts.  It checks if the disc being moved is correct.
            //It also checks if a disc is moving correctly.
            //Disc moves are 0 or 1. 0 being correct movement, 1 being wrong movement.
            //Disc values are 0 or 1. 0 being correct disk, 1 being wrong disc.

            cout << "Goldstein's heuristic selected." << endl;
            if(floor(equation) == equation)
                dNum = 0;
            else
                dNum = pow(n,n);

            if(n % 2  != 0)
            {
                if(node.diskMoved % 2 == 0)
                {
                    if(node.moveValue == -1)
                        mNum = 0;
                    else
                        mNum = 999;
                }
                else
                {
                    if(node.moveValue == 1)
                        mNum = 0;
                    else
                        mNum = 999;
                }
            }

            else
            {
                if(node.diskMoved % 2 == 0)
                {
                    if(node.moveValue == 1)
                        mNum = 0;
                    else
                        mNum = 999;
                }
                else
                {
                    if(node.moveValue == -1)
                        mNum = 0;
                    else
                        mNum = 999;
                }
            }

            h_value = dNum + mNum - moveCounter;
            break;

        case '3':
            cout << "Kadon's heuristic selected." << endl;

            if  ((node.state[0].discSum + node.state[2].discSum) == nSum)
            {
                h_value = nSum - 0;
            }

            else if (0 < (node.state[0].discSum + node.state[2].discSum) &&
                    (node.state[0].discSum + node.state[2].discSum) < nSum)
            {
                temp = nSum - (node.state[0].discSum + node.state[2].discSum);
                h_value = nSum - temp;
            }

            else ((node.state[0].discSum + node.state[2].discSum) == 0);
            {
                h_value = nSum - nSum;
            }
            break;

        default:
            cout << "Invalid heuristic label selected." << endl;
    }

    setHeuristicValue(node, h_value);
}
//**********************************************************************************************************************
// moveDisc() moves the top disc from one peg onto another for a single turn.
//**********************************************************************************************************************
bool Project2_TreeGraph::moveDisc(Project2_PegStack &source, Project2_PegStack &destination)
{
    int discNum;
    bool moveSuccess = false;

    if(!source.isEmpty()){
        discNum = source.pop();

        if(discNum < destination.getTopDisc() || destination.isEmpty()){
            destination.push(discNum);
            moveSuccess = true;
        }

        else{
            source.push(discNum);     // Put the disc back on its source peg if it can't be placed anywhere.
        }
    }
    return moveSuccess;
}
//**********************************************************************************************************************
//generateChildren() will generate the child nodes for the current node based on the current node's state value.
//**********************************************************************************************************************
void Project2_TreeGraph::generateChildren(GraphNode * parent, int i)
{
    // Each node has 2 to 3 child nodes to generate with a unique state from its siblings (NOT predecessors),
    // in the order of left, middle, and right.

    int j = 0;
    bool moveSuccess = false;
    GraphNode *child = new GraphNode;

    if(parent != nullptr) {
        child->Peg_A = parent->Peg_A;
        child->Peg_B = parent->Peg_B;
        child->Peg_C = parent->Peg_C;

        nodeIndexCounter++;
        switch (i)
        {
            case 0:     // To Generate the Left child
                setIndex(*child, nodeIndexCounter);
                while (!moveSuccess && j < 6)
                {
                    switch (j)
                    {
                        case 0:     // Try move from Peg A to Peg B
                            cout << "Attempt move from A to B..." << endl;
                            moveSuccess = moveDisc(child->Peg_A, child->Peg_B);
                            if (moveSuccess)
                            {
                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                child->stateA.discList = child->Peg_A.getDiscList();
                                child->stateA.discSum = child->Peg_A.getPegSum();

                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                child->state = {child->stateA, child->stateB, child->stateC};
                                cout << "Left Case 0: Child state assigned, A to B" << endl;
                                child->diskMoved = child->Peg_B.getTopDisc();
                                child->moveValue = 1;
                            }
                            break;

                        case 1:     // Try move from Peg A to Peg C
                            cout << "Attempt move from A to C..." << endl;
                            moveSuccess = moveDisc(child->Peg_A, child->Peg_C);
                            if (moveSuccess)
                            {
                                child->stateB.discList = parent->stateB.discList;   // No change of stateB for this move
                                child->stateB.discSum = parent->stateB.discSum;

                                child->stateA.discList = child->Peg_A.getDiscList();
                                child->stateA.discSum = child->Peg_A.getPegSum();

                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                child->state = {child->stateA, child->stateB, child->stateC};
                                cout << "Left Case 1: Child state assigned, A to C" << endl;
                                child->diskMoved = child->Peg_C.getTopDisc();
                                child->moveValue = -1;
                            }
                            break;

                        case 2:     // Try move from Peg B to Peg C
                            cout << "Attempt move from B to C..." << endl;
                            moveSuccess = moveDisc(child->Peg_B, child->Peg_C);
                            if (moveSuccess)
                            {
                                child->stateA.discList = parent->stateA.discList;   // No change of stateA for this move
                                child->stateA.discSum = parent->stateA.discSum;

                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                child->state = {child->stateA, child->stateB, child->stateC};
                                cout << "Left Case 2: Child state assigned, B to C" << endl;
                                child->diskMoved = child->Peg_C.getTopDisc();
                                child->moveValue = 1;
                            }
                            break;

                        case 3:     // Try move from Peg B to Peg A
                            cout << "Attempt move from B to A..." << endl;
                            moveSuccess = moveDisc(child->Peg_B, child->Peg_A);
                            if (moveSuccess)
                            {
                                child->stateC.discList = parent->stateC.discList;   // No change of stateC for this move
                                child->stateC.discSum = parent->stateC.discSum;

                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                child->stateA.discList = child->Peg_A.getDiscList();
                                child->stateA.discSum = child->Peg_A.getPegSum();

                                child->state = {child->stateA, child->stateB, child->stateC};
                                cout << "Left Case 3: Child state assigned, B to A" << endl;
                                child->diskMoved = child->Peg_A.getTopDisc();
                                child->moveValue = -1;
                            }
                            break;

                        case 4:     // Try move from Peg C to Peg B
                            cout << "Attempt move from C to B..." << endl;
                            moveSuccess = moveDisc(child->Peg_C, child->Peg_B);
                            if (moveSuccess)
                            {
                                child->stateA.discList = parent->stateA.discList;   // No change of stateA for this move
                                child->stateA.discSum = parent->stateA.discSum;

                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                child->state = {child->stateA, child->stateB, child->stateC};
                                cout << "Left Case 4: Child state assigned, C to B" << endl;
                                child->diskMoved = child->Peg_B.getTopDisc();
                                child->moveValue = -1;
                            }
                            break;

                        case 5:     // Try move from Peg C to Peg A
                            cout << "Attempt move from C to A..." << endl;
                            moveSuccess = moveDisc(child->Peg_C, child->Peg_A);
                            if (moveSuccess)
                            {
                                child->stateB.discList = parent->stateB.discList;   // No change of stateB for this move
                                child->stateB.discSum = parent->stateB.discSum;

                                child->stateA.discList = child->Peg_A.getDiscList();
                                child->stateA.discSum = child->Peg_A.getPegSum();

                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                child->state = {child->stateA, child->stateB, child->stateC};
                                cout << "Left Case 5: Child state assigned, C to A" << endl;
                                child->diskMoved = child->Peg_A.getTopDisc();
                                child->moveValue = 1;
                            }
                            break;
                    }
                    j++;
                }

                if (!moveSuccess)
                {
                    parent->left = nullptr;
                }

                else
                {
                    setCumulativeCost(*child, getCumulativeCost(*parent) + 1);
                    determineHeuristic(*child, n, h);
                    setTotalCost(*child);
                    setParent(*child, parent);
                    setLeftChild(*parent, child);
                    treeNodes.push_back(child);
                }
                break;

            case 1:     // To generate the middle child
                setIndex(*child, nodeIndexCounter);
                while (!moveSuccess && j < 6)
                {
                    switch (j)
                    {
                        case 0:     // Try move from Peg A to Peg B
                            cout << "Attempt move from A to B..." << endl;
                            moveSuccess = moveDisc(child->Peg_A, child->Peg_B);
                            if (moveSuccess)
                            {
                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                child->stateA.discList = child->Peg_A.getDiscList();
                                child->stateA.discSum = child->Peg_A.getPegSum();

                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                // Check if existing child for the parent node already has this state.
                                if (child->stateA.discList != getLeftChild(*parent)->state.at(0).discList ||
                                    child->stateB.discList != getLeftChild(*parent)->state.at(1).discList ||
                                    child->stateC.discList != getLeftChild(*parent)->state.at(2).discList)
                                {
                                    child->state = {child->stateA, child->stateB, child->stateC};
                                    cout << "Middle Case 0: Child state assigned, A to B" << endl;
                                    child->diskMoved = child->Peg_B.getTopDisc();
                                    child->moveValue = 1;
                                }

                                else
                                {
                                    cout << "Another existing child has this state." << endl;
                                    moveSuccess = moveDisc(child->Peg_B, child->Peg_A); // move disc back.
                                    moveSuccess = false;    // set to false to continue checking for a state that is
                                                            // unique from existing children for the parent node.
                                }
                            }
                            break;

                        case 1:     // Try move from Peg A to Peg C
                            cout << "Attempt move from A to C..." << endl;
                            moveSuccess = moveDisc(child->Peg_A, child->Peg_C);
                            if (moveSuccess)
                            {
                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                child->stateA.discList = child->Peg_A.getDiscList();
                                child->stateA.discSum = child->Peg_A.getPegSum();

                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                // Check if existing child for the parent node already has this state.
                                if (child->stateA.discList != getLeftChild(*parent)->state.at(0).discList ||
                                    child->stateB.discList != getLeftChild(*parent)->state.at(1).discList ||
                                    child->stateC.discList != getLeftChild(*parent)->state.at(2).discList)
                                {
                                    child->state = {child->stateA, child->stateB, child->stateC};
                                    cout << "Middle Case 1: Child state assigned, A to C" << endl;
                                    child->diskMoved = child->Peg_C.getTopDisc();
                                    child->moveValue = -1;
                                }

                                else
                                {
                                    cout << "Another existing child has this state." << endl;
                                    moveSuccess = moveDisc(child->Peg_C, child->Peg_A); // move disc back.
                                    moveSuccess = false;    // set to false to continue checking for a state that is
                                                            // unique from existing children for the parent node.
                                }
                            }
                            break;

                        case 2:     // Try move from Peg B to Peg C
                            cout << "Attempt move from B to C..." << endl;
                            moveSuccess = moveDisc(child->Peg_B, child->Peg_C);
                            if (moveSuccess)
                            {
                                child->stateA.discList = parent->stateA.discList;   // No change of stateA for this move
                                child->stateA.discSum = parent->stateA.discSum;

                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                // Check if existing child for the parent node already has this state.
                                if (child->stateA.discList != getLeftChild(*parent)->state.at(0).discList ||
                                    child->stateB.discList != getLeftChild(*parent)->state.at(1).discList ||
                                    child->stateC.discList != getLeftChild(*parent)->state.at(2).discList)
                                {
                                    child->state = {child->stateA, child->stateB, child->stateC};
                                    cout << "Middle Case 2: Child state assigned, B to C" << endl;
                                    child->diskMoved = child->Peg_C.getTopDisc();
                                    child->moveValue = 1;
                                }

                                else
                                {
                                    cout << "Another existing child for this node has this state." << endl;
                                    moveSuccess = moveDisc(child->Peg_C, child->Peg_B); // move disc back.
                                    moveSuccess = false;    // set to false to continue checking for a state that is
                                                            // unique from existing children for the parent node.
                                }
                            }
                            break;

                        case 3:     // Try move from Peg B to Peg A
                            cout << "Attempt move from B to A..." << endl;
                            moveSuccess = moveDisc(child->Peg_B, child->Peg_A);
                            if (moveSuccess)
                            {
                                child->stateC.discList = parent->stateC.discList;   // No change of stateC for this move
                                child->stateC.discSum = parent->stateC.discSum;

                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                child->stateA.discList = child->Peg_A.getDiscList();
                                child->stateA.discSum = child->Peg_A.getPegSum();

                                // Check if existing child for the parent node already has this state.
                                if (child->stateA.discList != getLeftChild(*parent)->state.at(0).discList ||
                                    child->stateB.discList != getLeftChild(*parent)->state.at(1).discList ||
                                    child->stateC.discList != getLeftChild(*parent)->state.at(2).discList)
                                {
                                    child->state = {child->stateA, child->stateB, child->stateC};
                                    cout << "Middle Case 3: Child state assigned, B to A" << endl;
                                    child->diskMoved = child->Peg_A.getTopDisc();
                                    child->moveValue = -1;
                                }

                                else
                                {
                                    cout << "Another existing child for this node has this state." << endl;
                                    moveSuccess = moveDisc(child->Peg_A, child->Peg_B); // move disc back.
                                    moveSuccess = false;    // set to false to continue checking for a state that is
                                                            // unique from existing children for the parent node.
                                }
                            }
                            break;

                        case 4:     // Try move from Peg C to Peg B
                            cout << "Attempt move from C to B..." << endl;
                            moveSuccess = moveDisc(child->Peg_C, child->Peg_B);
                            if (moveSuccess)
                            {
                                child->stateA.discList = parent->stateA.discList;   // No change of stateA for this move
                                child->stateA.discSum = parent->stateA.discSum;

                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                // Check if existing child for the parent node already has this state.
                                if (child->stateA.discList != getLeftChild(*parent)->state.at(0).discList ||
                                    child->stateB.discList != getLeftChild(*parent)->state.at(1).discList ||
                                    child->stateC.discList != getLeftChild(*parent)->state.at(2).discList)
                                {
                                    child->state = {child->stateA, child->stateB, child->stateC};
                                    cout << "Middle Case 4: Child state assigned, C to B" << endl;
                                    child->diskMoved = child->Peg_B.getTopDisc();
                                    child->moveValue = -1;
                                }

                                else
                                {
                                    cout << "Another existing child for this node has this state." << endl;
                                    moveSuccess = moveDisc(child->Peg_B, child->Peg_C); // move disc back.
                                    moveSuccess = false;    // set to false to continue checking for a state that is
                                                            // unique from existing children for the parent node.
                                }
                            }
                            break;

                        case 5:     // Try move from Peg C to Peg A
                            cout << "Attempt move from C to A..." << endl;
                            moveSuccess = moveDisc(child->Peg_C, child->Peg_A);
                            if (moveSuccess)
                            {
                                child->stateB.discList = parent->stateB.discList;   // No change of stateB for this move
                                child->stateB.discSum = parent->stateB.discSum;

                                child->stateA.discList = child->Peg_A.getDiscList();
                                child->stateA.discSum = child->Peg_A.getPegSum();

                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                // Check if existing child for the parent node already has this state.
                                if (child->stateA.discList != getLeftChild(*parent)->state.at(0).discList ||
                                    child->stateB.discList != getLeftChild(*parent)->state.at(1).discList ||
                                    child->stateC.discList != getLeftChild(*parent)->state.at(2).discList) {
                                    child->state = {child->stateA, child->stateB, child->stateC};
                                    cout << "Middle Case 5: Child state assigned, C to A" << endl;
                                    child->diskMoved = child->Peg_A.getTopDisc();
                                    child->moveValue = 1;
                                }

                                else
                                {
                                    cout << "Another existing child for this node has this state." << endl;
                                    moveSuccess = moveDisc(child->Peg_A, child->Peg_C); // move disc back.
                                    moveSuccess = false;    // set to false to continue checking for a state that is
                                                            // unique from existing children for the parent node.
                                }
                            }
                            break;
                    }
                    j++;
                }

                if (!moveSuccess)
                {
                    parent->middle = nullptr;
                }
                else
                {
                    setCumulativeCost(*child, getCumulativeCost(*parent) + 1);
                    determineHeuristic(*child, n, h);
                    setTotalCost(*child);
                    setParent(*child, parent);
                    setMiddleChild(*parent, child);
                    treeNodes.push_back(child);
                }
                break;

            case 2:     // To generate the right child
                setIndex(*child, nodeIndexCounter);
                while (!moveSuccess && j < 6)
                {
                    switch (j)
                    {
                        case 0:     // Try move from Peg A to Peg B
                            cout << "Attempt move from A to B..." << endl;
                            moveSuccess = moveDisc(child->Peg_A, child->Peg_B);
                            if (moveSuccess) {
                                child->stateC.discList = parent->stateC.discList;   // No change of stateC for this move
                                child->stateC.discSum = parent->stateC.discSum;

                                child->stateA.discList = child->Peg_A.getDiscList();
                                child->stateA.discSum = child->Peg_A.getPegSum();

                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                // Check if existing child for the parent node already has this state.
                                if ((child->stateA.discList != getLeftChild(*parent)->state.at(0).discList ||
                                     child->stateB.discList != getLeftChild(*parent)->state.at(1).discList ||
                                     child->stateC.discList != getLeftChild(*parent)->state.at(2).discList)
                                     &&
                                    (child->stateA.discList != getMiddleChild(*parent)->state.at(0).discList ||
                                     child->stateB.discList != getMiddleChild(*parent)->state.at(1).discList ||
                                     child->stateC.discList != getMiddleChild(*parent)->state.at(2).discList))
                                {
                                    child->state = {child->stateA, child->stateB, child->stateC};
                                    cout << "Right Case 0: Child state assigned, A to B" << endl;
                                    child->diskMoved = child->Peg_B.getTopDisc();
                                    child->moveValue = 1;
                                }
                                else{
                                    cout << "Another existing child for this node has this state." << endl;
                                    moveSuccess = moveDisc(child->Peg_B, child->Peg_A); // move disc back.
                                    moveSuccess = false;    // set to false to continue checking for a state that is
                                                            // unique from existing children for the parent node.
                                }
                            }
                            break;

                        case 1:         // Try move from Peg A to Peg C
                            cout << "Attempt move from A to C..." << endl;
                            moveSuccess = moveDisc(child->Peg_A, child->Peg_C);
                            if (moveSuccess) {
                                child->stateB.discList = parent->stateB.discList;   // No change of stateB for this move
                                child->stateB.discSum = parent->stateB.discSum;

                                child->stateA.discList = child->Peg_A.getDiscList();
                                child->stateA.discSum = child->Peg_A.getPegSum();

                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                // Check if existing child for the parent node already has this state.
                                if ((child->stateA.discList != getLeftChild(*parent)->state.at(0).discList ||
                                     child->stateB.discList != getLeftChild(*parent)->state.at(1).discList ||
                                     child->stateC.discList != getLeftChild(*parent)->state.at(2).discList)
                                     &&
                                    (child->stateA.discList != getMiddleChild(*parent)->state.at(0).discList ||
                                     child->stateB.discList != getMiddleChild(*parent)->state.at(1).discList ||
                                     child->stateC.discList != getMiddleChild(*parent)->state.at(2).discList))
                                {
                                    child->state = {child->stateA, child->stateB, child->stateC};
                                    cout << "Right Case 1: Child state assigned, A to C" << endl;
                                    child->diskMoved = child->Peg_C.getTopDisc();
                                    child->moveValue = -1;
                                }
                                else {
                                    cout << "Another existing child for this node has this state." << endl;
                                    moveSuccess = moveDisc(child->Peg_C, child->Peg_A); // move disc back.
                                    moveSuccess = false;    // set to false to continue checking for a state that is
                                                            // unique from existing children for the parent node.
                                }

                            }
                            break;

                        case 2:     // Try move from Peg B to Peg C
                            cout << "Attempt move from B to C..." << endl;
                            moveSuccess = moveDisc(child->Peg_B, child->Peg_C);
                            if (moveSuccess)
                            {
                                child->stateA.discList = parent->stateA.discList;   // No change of stateA for this move
                                child->stateA.discSum = parent->stateA.discSum;

                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                // Check if existing child for the parent node already has this state.
                                if ((child->stateA.discList != getLeftChild(*parent)->state.at(0).discList ||
                                     child->stateB.discList != getLeftChild(*parent)->state.at(1).discList ||
                                     child->stateC.discList != getLeftChild(*parent)->state.at(2).discList)
                                     &&
                                    (child->stateA.discList != getMiddleChild(*parent)->state.at(0).discList ||
                                     child->stateB.discList != getMiddleChild(*parent)->state.at(1).discList ||
                                     child->stateC.discList != getMiddleChild(*parent)->state.at(2).discList))
                                {
                                    child->state = {child->stateA, child->stateB, child->stateC};
                                    cout << "Right Case 2: Child state assigned, B to C" << endl;
                                    child->diskMoved = child->Peg_C.getTopDisc();
                                    child->moveValue = 1;
                                }

                                else
                                {
                                    cout << "Another existing child for this node has this state." << endl;
                                    moveSuccess = moveDisc(child->Peg_C, child->Peg_B); // move disc back.
                                    moveSuccess = false;    // set to false to continue checking for a state that is
                                                            // unique from existing children for the parent node.
                                }
                            }
                            break;

                        case 3:     // Try move from Peg B to Peg A
                            cout << "Attempt move from B to A..." << endl;
                            moveSuccess = moveDisc(child->Peg_B, child->Peg_A);
                            if (moveSuccess)
                            {
                                child->stateC.discList = parent->stateC.discList;   // No change of stateC for this move
                                child->stateC.discSum = parent->stateC.discSum;

                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                child->stateA.discList = child->Peg_A.getDiscList();
                                child->stateA.discSum = child->Peg_A.getPegSum();

                                // Check if existing child for the parent node already has this state.
                                if ((child->stateA.discList != getLeftChild(*parent)->state.at(0).discList ||
                                     child->stateB.discList != getLeftChild(*parent)->state.at(1).discList ||
                                     child->stateC.discList != getLeftChild(*parent)->state.at(2).discList)
                                     &&
                                    (child->stateA.discList != getMiddleChild(*parent)->state.at(0).discList ||
                                     child->stateB.discList != getMiddleChild(*parent)->state.at(1).discList ||
                                     child->stateC.discList != getMiddleChild(*parent)->state.at(2).discList))
                                {
                                    child->state = {child->stateA, child->stateB, child->stateC};
                                    cout << "Right Case 3: Child state assigned, B to A" << endl;
                                    child->diskMoved = child->Peg_A.getTopDisc();
                                    child->moveValue = -1;
                                }

                                else
                                {
                                    cout << "Another existing child for this node has this state." << endl;
                                    moveSuccess = moveDisc(child->Peg_A, child->Peg_B); // move disc back.
                                    moveSuccess = false;    // set to false to continue checking for a state that is
                                                            // unique from existing children for the parent node.
                                }
                            }
                            break;

                        case 4:     // Try move from Peg C to Peg B
                            cout << "Attempt move from C to B..." << endl;
                            moveSuccess = moveDisc(child->Peg_C, child->Peg_B);
                            if (moveSuccess) {
                                child->stateA.discList = parent->stateA.discList;   // No change of stateA for this move
                                child->stateA.discSum = parent->stateA.discSum;

                                child->stateB.discList = child->Peg_B.getDiscList();
                                child->stateB.discSum = child->Peg_B.getPegSum();

                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();
                                ;
                                // Check if existing child for the parent node already has this state.
                                if ((child->stateA.discList != getLeftChild(*parent)->state.at(0).discList ||
                                     child->stateB.discList != getLeftChild(*parent)->state.at(1).discList ||
                                     child->stateC.discList != getLeftChild(*parent)->state.at(2).discList)
                                     &&
                                    (child->stateA.discList != getMiddleChild(*parent)->state.at(0).discList ||
                                     child->stateB.discList != getMiddleChild(*parent)->state.at(1).discList ||
                                     child->stateC.discList != getMiddleChild(*parent)->state.at(2).discList))
                                {
                                    child->state = {child->stateA, child->stateB, child->stateC};
                                    cout << "Right Case 4: Child state assigned, C to B" << endl;
                                    child->diskMoved = child->Peg_B.getTopDisc();
                                    child->moveValue = -1;
                                }

                                else
                                {
                                    cout << "Another existing child for this node has this state." << endl;
                                    moveSuccess = moveDisc(child->Peg_B, child->Peg_C); // move disc back.
                                    moveSuccess = false;    // set to false to continue checking for a state that is
                                                            // unique from existing children for the parent node.
                                }
                            }
                            break;

                        case 5:         // Try move from Peg C to Peg A
                            cout << "Attempt move from C to A..." << endl;
                            moveSuccess = moveDisc(child->Peg_C, child->Peg_A);
                            if (moveSuccess)
                            {
                                child->stateB.discList = parent->stateB.discList;   // No change of stateB for this move
                                child->stateB.discSum = parent->stateB.discSum;

                                child->stateA.discList = child->Peg_A.getDiscList();
                                child->stateA.discSum = child->Peg_A.getPegSum();

                                child->stateC.discList = child->Peg_C.getDiscList();
                                child->stateC.discSum = child->Peg_C.getPegSum();

                                // Check if existing child for the parent node already has this state.
                                if ((child->stateA.discList != getLeftChild(*parent)->state.at(0).discList ||
                                     child->stateB.discList != getLeftChild(*parent)->state.at(1).discList ||
                                     child->stateC.discList != getLeftChild(*parent)->state.at(2).discList)
                                     &&
                                    (child->stateA.discList != getMiddleChild(*parent)->state.at(0).discList ||
                                     child->stateB.discList != getMiddleChild(*parent)->state.at(1).discList ||
                                     child->stateC.discList != getMiddleChild(*parent)->state.at(2).discList))
                                {
                                    child->state = {child->stateA, child->stateB, child->stateC};
                                    cout << "Right Case 5: Child state assigned, C to A" << endl;
                                    child->diskMoved = child->Peg_A.getTopDisc();
                                    child->moveValue = 1;
                                }

                                else
                                {
                                    cout << "Another existing child for this node has this state." << endl;
                                    moveSuccess = moveDisc(child->Peg_A, child->Peg_C); // move disc back.
                                    moveSuccess = false;    // set to false to continue checking for a state that is
                                                            // unique from existing children for the parent node.
                                }
                            }
                            break;
                    }
                    j++;
                }

                if (!moveSuccess)
                {
                    parent->right = nullptr;
                }

                else
                {
                    setCumulativeCost(*child, getCumulativeCost(*parent) + 1);
                    determineHeuristic(*child, n, h);
                    setTotalCost(*child);
                    setParent(*child, parent);
                    setRightChild(*parent, child);
                    treeNodes.push_back(child);
                }
                break;
        }
    }
}
//**********************************************************************************************************************
// checkOpenList() checks if the current best node's successors are in the Open list and processes them accordingly
//**********************************************************************************************************************
bool Project2_TreeGraph::checkOpenList(GraphNode *best, int i)
{
    bool inOpen = false;
    int j = 0;

    switch(i)
    {
        case 0:
            if(getLeftChild(*best) != nullptr)
            {
                Successor = getLeftChild(*best);

                while (!inOpen  && j < open.size())
                {

                    if (Successor->state.at(0).discList == open[j]->state.at(0).discList &&
                        Successor->state.at(1).discList == open[j]->state.at(1).discList &&
                        Successor->state.at(2).discList == open[j]->state.at(2).discList)
                    {
                        inOpen = true;
                        Old = open[j];
                        best->Successors.push_back(Old);
                    }
                    j++;
                }

                if (inOpen)
                {
                    if (getCumulativeCost(*Old) > getCumulativeCost(*Successor))
                    {
                        setParent(*Old, best);
                        setCumulativeCost(*Old, getCumulativeCost(*Successor)); // g(Old)
                        setTotalCost(*Old);    // F'(Old)
                    }
                }
            }
            break;

        case 1:
            if(getMiddleChild(*best) != nullptr)
            {
                Successor = getMiddleChild(*best);

                while (!inOpen && j < open.size())
                {

                    if (Successor->state.at(0).discList == open[j]->state.at(0).discList &&
                        Successor->state.at(1).discList == open[j]->state.at(1).discList &&
                        Successor->state.at(2).discList == open[j]->state.at(2).discList)
                    {
                        inOpen = true;
                        Old = open[j];

                        best->Successors.push_back(Old);
                    }
                    j++;
                }

                if (inOpen)
                {
                    if (getCumulativeCost(*Old) > getCumulativeCost(*Successor))
                    {
                        setParent(*Old, best);
                        setCumulativeCost(*Old, getCumulativeCost(*Successor)); // g(Old)
                        setTotalCost(*Old);    // F'(Old)
                    }
                }
            }
            break;

        case 2:
            if(getRightChild(*best) != nullptr)
            {
                Successor = getRightChild(*best);

                while (!inOpen && j < open.size())
                {

                    if (Successor->state.at(0).discList == open[j]->state.at(0).discList &&
                        Successor->state.at(1).discList == open[j]->state.at(1).discList &&
                        Successor->state.at(2).discList == open[j]->state.at(2).discList)
                    {
                        inOpen = true;
                        Old = open[j];

                        best->Successors.push_back(Old);
                    }
                    j++;
                }

                if (inOpen)
                {
                    if (getCumulativeCost(*Old) > getCumulativeCost(*Successor))
                    {
                        setParent(*Old, best);
                        setCumulativeCost(*Old, getCumulativeCost(*Successor)); // g(Old)
                        setTotalCost(*Old);    // F'(Old)
                    }
                }
            }
            break;
    }
    return inOpen;
}
//**********************************************************************************************************************
// checkClosedList()
//**********************************************************************************************************************
bool Project2_TreeGraph::checkClosedList(GraphNode *best, int i)
{
    bool inClosed = false;
    bool betterPathFound = false;
    GraphNode * startNodePtr;
    int k = 0;

    switch (i)
    {
        case 0:
            if(getLeftChild(*best) != nullptr)
            {
                Successor = getLeftChild(*best);
                if (!closed.empty())
                {
                    while (!inClosed && k < closed.size())
                    {

                        if (Successor->state.at(0).discList == closed[k]->state.at(0).discList &&
                            Successor->state.at(1).discList == closed[k]->state.at(1).discList &&
                            Successor->state.at(2).discList == closed[k]->state.at(2).discList)
                        {
                            inClosed = true;
                            Old = closed[k];
                            best->Successors.push_back(Old);
                        }
                        k++;
                    }
                    if (inClosed)
                    {
                        if (getCumulativeCost(*Old) > getCumulativeCost(*Successor))
                        {
                            betterPathFound = true;
                            setParent(*Old, best);
                            setCumulativeCost(*Old, getCumulativeCost(*Successor)); // g(Old)
                            setTotalCost(*Old);    // F'(Old)
                        }
                    }
                    if (betterPathFound)
                    {
                        depthFirstSearch(Old);
                    }
                }
            }
            break;

        case 1:
            if(getMiddleChild(*best) != nullptr)
            {
                Successor = getMiddleChild(*best);
                if (!closed.empty())
                {
                    while (!inClosed && k < closed.size())
                    {
                        if (Successor->state.at(0).discList == closed[k]->state.at(0).discList &&
                            Successor->state.at(1).discList == closed[k]->state.at(1).discList &&
                            Successor->state.at(2).discList == closed[k]->state.at(2).discList)
                        {
                            inClosed = true;
                            Old = closed[k];
                            best->Successors.push_back(Old);
                        }
                        k++;
                    }

                    if (inClosed)
                    {
                        if (getCumulativeCost(*Old) > getCumulativeCost(*Successor))
                        {
                            betterPathFound = true;
                            setParent(*Old, best);
                            setCumulativeCost(*Old, getCumulativeCost(*Successor)); // g(Old)
                            setTotalCost(*Old);    // F'(Old)
                        }
                    }
                    if (betterPathFound)
                    {
                        depthFirstSearch(Old);
                    }
                }
            }
            break;

        case 2:
            if(getRightChild(*best) != nullptr)
            {
                Successor = getRightChild(*best);
                if (!closed.empty()) {
                    while (!inClosed && k < closed.size())
                    {
                        if (Successor->state.at(0).discList == closed[k]->state.at(0).discList &&
                            Successor->state.at(1).discList == closed[k]->state.at(1).discList &&
                            Successor->state.at(2).discList == closed[k]->state.at(2).discList)
                        {
                            inClosed = true;
                            Old = closed[k];
                            best->Successors.push_back(Old);
                        }
                        k++;
                    }

                    if (inClosed)
                    {
                        if (getCumulativeCost(*Old) > getCumulativeCost(*Successor))
                        {
                            betterPathFound = true;
                            setParent(*Old, best);
                            setCumulativeCost(*Old, getCumulativeCost(*Successor)); // g(Old)
                            setTotalCost(*Old);    // F'(Old)
                        }
                    }
                    if (betterPathFound)
                    {
                        depthFirstSearch(Old);
                    }
                }
            }
            break;
    }
    return inClosed;
}
//**********************************************************************************************************************
// placeOnOpen() places the successor of best node on the Open List
//**********************************************************************************************************************
void Project2_TreeGraph::placeOnOpen(GraphNode* best, int i)
{

    switch(i)
    {
        case 0:
            if(getLeftChild(*best) != nullptr)
            {
                Successor = getLeftChild(*best);
                setTotalCost(*Successor);
                if (getTotalCost(*Successor) <= getTotalCost(*open.front()))
                    open.insert(open.begin(), Successor);
                else
                    open.push_back(Successor);
            }
            break;

        case 1:
            if(getMiddleChild(*best) != nullptr)
            {
                Successor = getMiddleChild(*best);
                setTotalCost(*Successor);
                if (getTotalCost(*Successor) <= getTotalCost(*open.front()))
                    open.insert(open.begin(), Successor);
                else
                    open.push_back(Successor);
            }
            break;

        case 2:
            if(getRightChild(*best) != nullptr)
            {
                Successor = getRightChild(*best);
                setTotalCost(*Successor);
                if (getTotalCost(*Successor) <= getTotalCost(*open.front()))
                    open.insert(open.begin(), Successor);
                else
                    open.push_back(Successor);
            }
            break;
    }
}
//**********************************************************************************************************************
// depthFirstSearch() starts at old and propagates the new cost to OLD's successors.
//**********************************************************************************************************************
void Project2_TreeGraph::depthFirstSearch(GraphNode * old)
{
    vector<GraphNode *> discovered;        // already searched nodes
    GraphNode * start = old;
    discovered.push_back(start);
    for(int i=0; i < 3; i++)
    {
        switch (i)
        {
            case 0:
                if(getLeftChild(*start) != nullptr && getParent(*getLeftChild(*start)) == start)
                {
                    setCumulativeCost(*getLeftChild(*start), getCumulativeCost(*start) + 1);
                    // update g(successor)
                    setTotalCost(*getLeftChild(*start));  // update F(successor)
                    depthFirstSearch(getLeftChild(*start));
                }

                else if(getLeftChild(*start) != nullptr &&
                getCumulativeCost(*getParent(*getLeftChild(*start))) > getCumulativeCost(*start))
                {
                    setParent(*getLeftChild(*start), start);
                    setCumulativeCost(*getLeftChild(*start), getCumulativeCost(*start) + 1);
                    // update g(successor)
                    setTotalCost(*getLeftChild(*start));  // update F(successor)
                    depthFirstSearch(getLeftChild(*start));
                }
                break;

            case 1:
                if(getMiddleChild(*start) != nullptr && getParent(*getMiddleChild(*start)) == start)
                {
                    setCumulativeCost(*getMiddleChild(*start), getCumulativeCost(*start) + 1);
                    // update g(successor)
                    setTotalCost(*getMiddleChild(*start));  // update F(successor)
                    depthFirstSearch(getMiddleChild(*start));
                }

                else if(getMiddleChild(*start) != nullptr &&
                getCumulativeCost(*getParent(*getMiddleChild(*start))) > getCumulativeCost(*start))
                {
                    setParent(*getMiddleChild(*start), start);
                    setCumulativeCost(*getMiddleChild(*start), getCumulativeCost(*start) + 1);
                    // update g(successor)
                    setTotalCost(*getMiddleChild(*start));  // update F(successor)
                    depthFirstSearch(getMiddleChild(*start));
                }
                break;

            case 2:
                if(getRightChild(*start) != nullptr && getParent(*getRightChild(*start)) == start)
                {
                    setCumulativeCost(*getRightChild(*start), getCumulativeCost(*start) + 1);
                    // update g(successor)
                    setTotalCost(*getRightChild(*start));  // update F(successor)
                    depthFirstSearch(getRightChild(*start));
                }

                else if(getRightChild(*start) != nullptr &&
                getCumulativeCost(*getParent(*getRightChild(*start))) > getCumulativeCost(*start))
                {
                    setParent(*getRightChild(*start), start);
                    setCumulativeCost(*getRightChild(*start), getCumulativeCost(*start) + 1);
                    // update g(successor)
                    setTotalCost(*getRightChild(*start));  // update F(successor)
                    depthFirstSearch(getRightChild(*start));
                }
                break;
        }
    }
}
//**********************************************************************************************************************
// A_StarSearch() conducts an A* Search on this tree
//**********************************************************************************************************************
Project2_TreeGraph::RunData Project2_TreeGraph::A_StarSearch()
{
    auto start = high_resolution_clock::now();
    RunData runData;
    runData.numNodesGen = 0;
    runData.numNodesExp = 0;
    bool goalFound = false;
    bool onOpen = false;
    bool onClosed = false;

    GraphNode* BestNode = nullptr;
    // The A* Algorithm
    setCumulativeCost(*StartNode, 0);
    determineHeuristic(*StartNode, n,h);
    setTotalCost(*StartNode);

    open.push_back(StartNode);
    // Until a goal node is found, repeat the following procedure:
    while(!goalFound) {
    // If there are no nodes on OPEN, report failure.
        if (open.empty()) {
            //cout << "Failure to find nodes on OPEN list." << endl;
            break;
        }
    // Otherwise, pick the node on OPEN with the lowest f’ value. Call it BESTNODE.
        BestNode = open.front();
    // Remove it from OPEN. Place it on CLOSED.
        closed.push_back(BestNode);
        open.erase(open.begin());
    // See if BESTNODE is a goal node. If so, exit and report a solution (either BESTNODE if all we want is the
    // node or the path that has been created between the initial state and BESTNODE if we are interested in the path).
        if (BestNode->state.at(0).discList == goalState[0].discList &&
            BestNode->state.at(1).discList == goalState[1].discList &&
            BestNode->state.at(2).discList == goalState[2].discList)
        {
            cout << "Solution found!" << endl;
            cout << "Best State Peg A Sum: " << BestNode->state.at(0).discSum << endl;
            cout << "Best State Peg B Sum: " << BestNode->state.at(1).discSum << endl;
            cout << "Best State Peg C Sum: " << BestNode->state.at(2).discSum << endl;

            goalFound = true;
            // Set run data here
            break;
        }

        else
        {
        // Otherwise, generate the successors of BESTNODE but do not set BESTNODE to point to them yet.
        // First we need to see if any of them have already been generated.
        // For each such SUCCESSOR, do the following:

            for (int i = 0; i < 3; i++)
            {
            // Set SUCCESSOR to point back to BESTNODE.
            // These backwards links will make it possible to recover the path once a solution is found.
            // Compute g(SUCCESSOR) = g(BESTNODE) + the cost of getting from BESTNODE to SUCCESSOR.

                generateChildren(BestNode, i);

            // See if SUCCESSOR is the same as any node on OPEN (i.e., It has already been generated but not processed).
            // If so, call that node OLD. Since this node already exists in the graph, we can throw SUCCESSOR away and
            // add OLD to the list of BESTNODE's successors.
            // Now we must decide whether OLD's parent link should be reset to point to BESTNODE.
            // It should be if the path we have just found to SUCCESSOR is cheaper than the current best path to OLD
            // (since SUCCESSOR and OLD are really the same node).
            // So see whether it is cheaper to get to OLD via its current parent or to SUCCESSOR via BESTNODE by
            // comparing their g values. If OLD is cheaper (or just as cheap), then we need do nothing.
            // If SUCCESSOR is cheaper, then reset OLD's parent link to point to BESTNODE,record the new cheaper path
            // in g(OLD), and update f’(OLD).

                onOpen = checkOpenList(BestNode, i);

            // If SUCCESSOR was not on OPEN, see if it is on CLOSED. If so, call the node on CLOSED OLD and add
            // OLD to the list of BESTNODE's successors. Check to see if the new path or the old path is better
            // just as in step 2(c), and set the parent link-and g and f’ values appropriately. If we have just found
            // a better path to OLD, we must propagate the improvement to OLD's successors. This is a bit
            // tricky. OLD points to its successors. Each successor in turn points to its successors, and so forth,
            // until each branch terminates with a node that either is still on OPEN or has no successors. So to
            // propagate the new cost downward, do a depth-first traversal of the tree starting at OLD, changing
            // each node's g value (and thus also its f' value), terminating each branch when you reach either a
            // node with no successors or a node to which an equivalent or better path has already been found.
            // This condition is easy to check for. Each node's parent link points back to its best known parent. As
            // we propagate down to a node, see if its parent points to the node we are corning from. If so,
            // continue the propagation. If not, then its g value already reflects the better path of which it is
            // part. So the propagation may stop here. But it is possible that with the new value of g being
            // propagated downward, the path we are following may become better than the path through the
            // current parent. So compare the two. If the path through the current parent is still better, stop the
            // propagation. If the path we are propagating through is now better, reset the parent and continue
            // propagation.

            if (!onOpen)
                    onClosed = checkClosedList(BestNode, i);

            // If SUCCESSOR was not already on either OPEN or CLOSED, then put it on OPEN, and add it to the
            // list of BESTNODE's successors. Compute f'(SUCCESSOR) = g(SUCCESSOR) + h'(SUCCESSOR).

            if (!onOpen && !onClosed)
                    placeOnOpen(BestNode, i);
            }
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = (duration_cast<milliseconds>(stop - start).count()) * 0.001;
    delete(BestNode);
    runData.numNodesExp = closed.size();
    runData.numNodesGen = treeNodes.size();
    runData.execTime = duration;
    return runData;
}

void Project2_TreeGraph::setSuccessor(GraphNode *best, int i)
{
    cout << "Entering setSuccessor() function for RBFS..." << endl;

    switch(i)
    {
        case 0:
            if(!best->succ.empty())
            { // Clear the successor list for this node if it has been processed before.
                best->succ.erase(best->succ.begin(), best->succ.end());
            }

            cout << "Placing left child on current node's successors list..." << endl;
            if(getLeftChild(*best) != nullptr)
            {
                cout << "Current node has a left child..." << endl;
                Successor = getLeftChild(*best);
                cout << "Left child's index = " << Successor->nodeIndex << endl;
                setTotalCost(*Successor);
                cout << "Left child's f_value = " << Successor->totalCost << endl;
                cout << "Left child placed at the end of the successor queue." << endl;
                best->succ.push_back(Successor);

            }
            break;

        case 1:
            cout << "Placing middle child on current node's successors list..." << endl;
            if(getMiddleChild(*best) != nullptr)
            {
                cout << "Current node has a middle child..." << endl;
                Successor = getMiddleChild(*best);
                cout << "Middle child's index = " << Successor->nodeIndex << endl;
                setTotalCost(*Successor);
                cout << "Middle child's f_value = " << Successor->totalCost << endl;
                if (!best->succ.empty() && getTotalCost(*Successor) < getTotalCost(*best->succ.front()))
                {
                    cout << "The middle child's F_value is a priority." << endl;
                    best->succ.insert(best->succ.begin(), Successor);
                }
                else{
                    cout << "Middle child placed at the end of the successor queue." << endl;
                    best->succ.push_back(Successor);
                }
            }
            break;

        case 2:
            cout << "Placing right child on current node's successors list..." << endl;
            if(getRightChild(*best) != nullptr) {
                cout << "Current node has a right child..." << endl;
                Successor = getRightChild(*best);
                cout << "Right child's index = " << Successor->nodeIndex << endl;
                setTotalCost(*Successor);
                cout << "Right child's f_value = " << Successor->totalCost << endl;
                if (!best->succ.empty() && getTotalCost(*Successor) < getTotalCost(*best->succ.front()))
                {
                    cout << "The Right child's F_value is a priority." << endl;
                    best->succ.insert(best->succ.begin(), Successor);
                }
                else {
                    cout << "Right child placed at the end of the successor queue." << endl;
                    best->succ.push_back(Successor);
                }
            }
            break;
    }
    cout << "Entering setSuccessor() function for RBFS..." << endl;
}
//**********************************************************************************************************************
// startRBFS() Begins the Recursive Best First Search function
//**********************************************************************************************************************
Project2_TreeGraph::RunData Project2_TreeGraph::startRBFS()
{
    GraphNode solution;
    RunData runData;
    runData.numNodesGen = 0;
    runData.numNodesExp = 0;
    auto start = high_resolution_clock::now();

    cout << "In startRBFS() function..." << endl;

    solution = RBFS(StartNode, n * 100000);
    cout << "Solution Node: " << endl;
    cout << "   Index: " << solution.nodeIndex << endl;
    cout << "   F'Cost: " << solution.totalCost << endl;
    cout << "   Peg A Sum: " << solution.state[0].discSum << endl;
    cout << "   Peg B Sum: " << solution.state[1].discSum << endl;
    cout << "   Peg C Sum: " << solution.state[2].discSum << endl;
    cout << "Exiting startRBFS() function..." << endl;
    auto stop = high_resolution_clock::now();
    auto duration = (duration_cast<milliseconds>(stop - start).count()) * 0.001;
    runData.numNodesGen = treeNodes.size();
    runData.numNodesExp = expanded;
    runData.execTime = duration;
    return runData;
}
   /* Original Code by Goldstein that was edited to create above version.
   // Pick child f-value as best node value unless value is greater if greater go back up and
   // find a lower value for the node
   // If goal found FINISH
   if (BestNode->state.at(0).discList == goalState[0].discList &&
          BestNode->state.at(1).discList == goalState[1].discList &&
          BestNode->state.at(2).discList == goalState[2].discList)
   {
       cout << "Solution found!" << endl;
       cout << "Best State Peg A Sum: " << BestNode->state.at(0).discSum << endl;
       cout << "Best State Peg B Sum: " << BestNode->state.at(1).discSum << endl;
       cout << "Best State Peg C Sum: " << BestNode->state.at(2).discSum << endl;
   }
   else
   // Choose new child as parent
   // Generate the children
   // Generate their f value

   RBFS();
   GraphNode *BestNode = nullptr;

   if(succ.empty())
   {
       setCumulativeCost(*StartNode, 0);
       cout << "Setting the start node h value..." << endl;
       determineHeuristic(*StartNode, n,h);
       cout << "Setting the start node f value..." << endl;
       setTotalCost(*StartNode);
       cout << "start node f value = " << getTotalCost(*StartNode) << endl;
       succ.push_back(StartNode);
   }

   BestNode = succ.front();
   succ.erase(succ.begin());
   expanded++;

   // function RECURSIVE-BEST-FIRST-SEARCH(problem)
   // returns a solution, or failure return RBFS(problem,MAKE-NODE(problem.INITIAL-STATE),∞)
   // function RBFS(problem,node,f limit)returns a solution,or failure and a new f-cost limit
   // if problem.GOAL-TEST(node.STATE) then return SOLUTION(node)

   if (BestNode->state.at(0).discList == goalState[0].discList &&
       BestNode->state.at(1).discList == goalState[1].discList &&
       BestNode->state.at(2).discList == goalState[2].discList)
   {
       cout << "Solution found!" << endl;
       cout << "Best State Peg A Sum: " << BestNode->state.at(0).discSum << endl;
       cout << "Best State Peg B Sum: " << BestNode->state.at(1).discSum << endl;
       cout << "Best State Peg C Sum: " << BestNode->state.at(2).discSum << endl;
       generated = succ.size() + expanded;
       cout << "    Data: " << endl;
       cout << "    Expanded Nodes: " << expanded << endl;
       cout << "    Generated Nodes: " << generated << endl;
       cout << "    Execution Time: " << data.execTime << " microseconds" << endl;
   }

   else
   {
       cout << "Solution not found yet." << endl;

   // Otherwise, generate the successors of BESTNODE but do not set BESTNODE to point to them yet.
   // First we need to see if any of them have already been generated.
   // For each such SUCCESSOR, do the following:

       for (int i = 0; i < 3; i++)
       {
           cout <<"In for loop..." << endl;
           generateChildren(BestNode, i);
           setSuccessor(BestNode, i);
           moveCounter++;
       }

   // successors ←[]

   // For each action in problem.ACTIONS(node.STATE) do
   // Add CHILD-NODE(problem,node,action) into successors
   // If successors is empty then return failure, ∞

   else
   {
   //    for each s in successors do update f with value from previous search, if any
   //        s.f ← max(s.g + s.h, node.f ))
   //    loop do
   //        best ← the lowest f-value node in successors
   //        f limit represent the parent's value
   //        if best.f > f limit then return failure, best.f
   //        alternative ← the second-lowest f-value among successors
   //        result,best.f ← RBFS(problem,best,min(f limit,alternative))
   //        if result != failure then return result
     */

//**********************************************************************************************************************
// RBFS() starts the Recursive Best First Search algorithm. Returns the new f_limit (best alternative path available
// from an ancestor node).
//**********************************************************************************************************************
Project2_TreeGraph::GraphNode Project2_TreeGraph::RBFS(GraphNode * node, int f_limit)
{
    cout << "Starting f_limit = " << f_limit << endl;
    int alternative;
    GraphNode* BestSucc = nullptr;
    GraphNode result;
    cout << "Now in RBFS() function..." << endl;

    if (node->state.at(0).discList == goalState[0].discList &&
        node->state.at(1).discList == goalState[1].discList &&
        node->state.at(2).discList == goalState[2].discList)
    {
        cout << "Solution found!" << endl;
        solutionFound = true;
        cout << "Best State Peg A Sum: " << node->state.at(0).discSum << endl;
        cout << "Best State Peg B Sum: " << node->state.at(1).discSum << endl;
        cout << "Best State Peg C Sum: " << node->state.at(2).discSum << endl;
        cout << "Data: " << endl;
        cout << "    Expanded Nodes: " << expanded << endl;
        cout << "    Generated Nodes: " << generated << endl;
        return *node;
    }

    else
    {
        cout << "Goal state not found, continuing RBFS search..." << endl;
        for (int i = 0; i < 3; i++)
        {
            // generate children and add to successors list for this node.
            cout << "Generating child node #" << i;
            cout << " for node with F value = " << node->totalCost << endl;
            generateChildren(node, i);
            cout << "Setting child node #" << i << " in successor list." << endl;
            setSuccessor(node, i);
            moveCounter++;  // for h2
        }

        if (node->succ.empty())
        {
            // return failure
            cout << "Failure to find solution." << endl;
            failureRBFS = true;
            result.totalCost = n * 100000;
            // Very large number that corresponds with f_limit = infinity in textbook's pseudocode.
            return result;
        }

        cout << "The size of this node's successor list is: " << node->succ.size() << endl;
        // for each element in the successors list, do:
        for (int j = 0; j < node->succ.size(); j++)
        {   // There should be no more than 3 successors per node.
            // update f value from a previous search, if any. (s.f <- max(s.g +s.h, node.f)
            cout << " j = " << j << endl;
            cout << "Updating f_value for Node #" << node->succ[j]->nodeIndex << " to ";
            //" with f_limit = " << f_limit << endl;
            if (h != '2')
            node->succ.at(j)->totalCost =
                    max((node->succ.at(j)->cumulativeCost + node->succ.at(j)->heuristicValue),node->totalCost);
        }

        sort(node->succ.begin(), node->succ.end(), sortByF_Value);
        // loop do
        while(!solutionFound)
        {
            // Sort the list of successors
            sort(node->succ.begin(), node->succ.end(), sortByF_Value);
            ///// TRACE
            cout << "Successors: {";
            for(int i = 0; i < node->succ.size(); i++)
            {
                cout << " "<< node->succ[i]->totalCost << ", ";
            }

            cout << "}" << endl;
            //////

            // best node = the lowest f value node in successors list for the current node
            BestSucc = node->succ.front();
            expanded++;
            cout << "Current BestSucc f_value = " << BestSucc->totalCost << endl;

            // if best node's f value > f_limit
            if (BestSucc->totalCost > f_limit)
            {
                // return failure, and best node (for f value)
                cout << "Failure: Current BestSucc f_value > current f_limit of " << f_limit << ". Returning BestNode.";
                cout << endl;
                //f_limit = BestSucc->totalCost;
                failureRBFS = true;
                return *BestSucc;
            }

            // alternative = the second-lowest f value node in the successors list for the current node
            alternative = node->succ[1]->totalCost;
            cout << "BestSucc f_value = " << BestSucc->totalCost << " , f_limit = " << f_limit << endl;

            // result (with best node f value) = RBFS(BestSucc, min(f_limit, alternative)
            result = RBFS(BestSucc, min(f_limit, alternative));

            BestSucc->totalCost = result.totalCost;
            if (!failureRBFS)
            {
                // If 0, then recursive RBFS failed to find a solution node
                // return result (as node)
                cout << "result != failure, result F_value = " << result.totalCost << endl;
                return result;
            }
        }
    }
    cout << "Now exiting RBFS() function..." << endl;
    return result;
}
//**********************************************************************************************************************
// sortByF_Value() will sort the successors of a node in a vector with the lowest f_value first
//**********************************************************************************************************************
bool Project2_TreeGraph::sortByF_Value(const Project2_TreeGraph::GraphNode*lhs, const Project2_TreeGraph::GraphNode*rhs)
{
    return lhs->totalCost < rhs->totalCost;
}
