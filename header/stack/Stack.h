/*
Author: Jii Yoo
Date: April 29, 2016
Specification file for the Stack (LIFO) class
*/

#ifndef STACK_H
#define STACK_H

// System Libraries
#include <cstdlib>
#include <iostream>
#include <string>
using namespace std;

// User Libraries
#include "Node.h"

class Stack {
    private:
        Node *front;
        Node *lastNode;
        int items;

    public:
        Stack( void );  // Constructor
        ~Stack( void ); // Destructor

        // Member Functions
        void push_back( string );
        string pop_back( void );
        string peek( void );
        int getItems( void );
};

#endif
