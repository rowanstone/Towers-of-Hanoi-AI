#ifndef PROJECT2_PROJECT2_PEGSTACK_H
#define PROJECT2_PROJECT2_PEGSTACK_H
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;


class Project2_PegStack {

private:
    struct DiscNode{
        int disc;           // To hold the disc number/size
        DiscNode *next;     // Pointer to the next disc
    };
    char pegLabel;          // Hold the label of the peg.
    DiscNode *top;          // Pointer to the top disc in the stack.

public:
    // Constructor
    Project2_PegStack(char);
    // Copy Constructor
    Project2_PegStack(const Project2_PegStack &);
    Project2_PegStack& operator=(const Project2_PegStack &);
    // Destructor
    ~Project2_PegStack();
    // PegStack operations
    void push(int );
    int pop ();
    bool isEmpty();
    int getTopDisc();
    int getPegSum();
    vector<int> getDiscList();
};

#endif //PROJECT2_PROJECT2_PEGSTACK_H