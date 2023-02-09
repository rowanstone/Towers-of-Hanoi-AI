#include "Project2-kms240-PegStack.h"
using namespace std;
//**********************************************************************************************************************
// Constructor
//**********************************************************************************************************************
Project2_PegStack::Project2_PegStack(char c){
    pegLabel = c;
    top = nullptr;
}
//**********************************************************************************************************************
// Copy Constructor ()
//**********************************************************************************************************************
Project2_PegStack::Project2_PegStack(const Project2_PegStack &rhs_stack)
{
    top = nullptr;
    DiscNode *currentDiscPtr = nullptr;   // Current disc copy
    DiscNode *nextNode = nullptr;
    int discCopy;
    vector<int> discList;
    pegLabel = rhs_stack.pegLabel;
    currentDiscPtr = rhs_stack.top;
    // Traverse through the peg stack to copy all discs into a vector in inverse order.
    while(currentDiscPtr != nullptr){
        nextNode = currentDiscPtr->next;
        discCopy = currentDiscPtr->disc;
        discList.insert(discList.begin(), discCopy);
        currentDiscPtr = nextNode;
    }
    // Push the disc copies onto the new PegStack object in the correct order.
    for(int i = 0; i < discList.size();i++){
        this->push(discList[i]);
    }

}

Project2_PegStack& Project2_PegStack::operator = (const Project2_PegStack &right)
{
    top = nullptr;
    DiscNode *currentDiscPtr;   // Current disc copy
    DiscNode *nextNode = nullptr;
    int discCopy;
    vector<int> discList;
    pegLabel = right.pegLabel;
    currentDiscPtr = right.top;
    // Traverse through the peg stack to copy all discs into a vector in inverse order.

    while(currentDiscPtr != nullptr)
    {
        nextNode = currentDiscPtr->next;
        discCopy = currentDiscPtr->disc;
        discList.insert(discList.begin(), discCopy);
        currentDiscPtr = nextNode;
    }
    // Push the disc copies onto the new PegStack object in the correct order.
    for(int i = 0; i < discList.size();i++){
        this->push(discList[i]);
    }
    return *this;
}
//**********************************************************************************************************************
// Destructor function deletes all the nodes from the stack.
//**********************************************************************************************************************
Project2_PegStack::~Project2_PegStack()
{
    DiscNode *workingPtr = nullptr, *nextNode = nullptr;
    // Position workingPtr at top of the stack
    workingPtr = top;

    // Traverse the stack and delete each node
    while (workingPtr != nullptr)
    {
        nextNode = workingPtr->next;
        delete workingPtr;
        workingPtr = nextNode;
    }
    delete(workingPtr);
    delete(nextNode);
}
//**********************************************************************************************************************
// push() places a disc at the top of the stack
//**********************************************************************************************************************
void Project2_PegStack::push(int d)
{
    DiscNode *newDisc = nullptr; // Pointer to a new disk

    newDisc = new DiscNode;      // Allocate a new disk and store num there.

    // If there are no discs in the stack, then make newDisc the first disc
    if (isEmpty())
    {
        top = newDisc;
        newDisc->next = nullptr;
        newDisc->disc = d;
    }

    else if(d < top->disc)
    {
        newDisc->next = top;
        top = newDisc;
        newDisc->disc = d;
    }
}
//**********************************************************************************************************************
// pop() pops the value off the top of a stack and copies it into the variable passed as an argument.
//**********************************************************************************************************************
int Project2_PegStack::pop()
{
    int num;
    DiscNode *temp = nullptr; // Temporary pointer

    // First make sure the stack isn't empty
    if (isEmpty())
    {
        cout << "\nThe peg is empty.\n";
    }
    else // Pop value off the top of the stack
    {
        num = top->disc;
        temp = top->next;
        delete top;
        top = temp;
    }
    return num;
}
//**********************************************************************************************************************
// isEmpty() checks if the stack is empty
//**********************************************************************************************************************
bool Project2_PegStack::isEmpty()
{
    bool status;

    if (!top)
        status = true;
    else
        status = false;

    return status;
}
//**********************************************************************************************************************
// getTopDisc() returns the value of the top disc on a peg.
//**********************************************************************************************************************
int Project2_PegStack::getTopDisc()
{
    if(isEmpty())
    {
        return 0;
    }
    else
    {
        return top->disc;
    }
}
//**********************************************************************************************************************
// getPegSum() returns the sum of discs on a peg.
//**********************************************************************************************************************
int Project2_PegStack::getPegSum()
{
    DiscNode *currentDiscPtr;   // Current disc to be counted for sum
    int sum = 0;                // Sum = 0 when the stack is empty
    currentDiscPtr = top;

    if(!isEmpty())
    {
        // Traverse through the peg stack to sum the values
        while (currentDiscPtr != nullptr)
        {
            sum += currentDiscPtr->disc;
            currentDiscPtr = currentDiscPtr->next;
        }
    }
    return sum;
}
//**********************************************************************************************************************
// getDiscList() returns a list of all the current disc values on a peg in order
//**********************************************************************************************************************
vector<int> Project2_PegStack::getDiscList()
{
    DiscNode *currentDiscPtr;   // Current disc to be counted for sum
    vector<int> discList ={0};  // Empty list
    currentDiscPtr = top;

    if(!isEmpty()) {
        // Traverse through the peg stack to add values to list in order
        while (currentDiscPtr != nullptr)
        {
            discList.insert(discList.begin() + 1, currentDiscPtr->disc);
            currentDiscPtr = currentDiscPtr->next;
        }
    }
    return discList;
}
