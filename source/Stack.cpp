/*
Author: Jii Yoo
Date: April 29, 2016
Implementation file for the Stack (LIFO) class
*/

#include <iostream>
#include "../header/stack/Stack.h"

/**********************************************************************
*                           Constructor
**********************************************************************/
Stack::Stack( void ) 
{
    front = new Node;
    front->from[0]='0';
    front->from[1]='0';
    front->to[0]='0';
    front->to[1]='0';
    lastNode = front;
    items=0;
}

/**********************************************************************
*                           Destructor
**********************************************************************/
Stack::~Stack( void ) 
{
    lastNode = NULL;
    while (front->ptr!=NULL) 
    {
        Node *temp = front;
        front = front->ptr;
        delete temp;
    }

    delete front;
    delete lastNode;
}

/**********************************************************************
* RETURN        : void
* PRE-CONDITION : -
* POST-CONDITION: add data at the end of the list
* PURPOSE       : push Node at the end of the list
**********************************************************************/
void Stack::push_back( string move ) 
{
    Node *temp = new Node;
    temp->from[0] = move[0];
    temp->from[1] = move[1];
    temp->to[0] = move[4];
    temp->to[1] = move[5];
    temp->ptr = NULL;

    // Since I remembered the lastNode let lastNode points to the temp
    // and now temp becomes the lastNode
    lastNode->ptr=temp;
    lastNode=temp;
    ++items;
}

/**********************************************************************
* RETURN        : string
* PRE-CONDITION : list size > 0
* POST-CONDITION: last data is removed
* PURPOSE       : pop data from the end of the list
**********************************************************************/
string Stack::pop_back( void ) 
{
    --items;
    string mv;
    mv+= lastNode->from[0];
    mv+=lastNode->from[1];
    mv+=" to ";
    mv+=lastNode->to[0];
    mv+=lastNode->to[1];

    if (front->ptr==NULL) 
    {
        return mv;
    }

    // locate the one previous to the last Node
    Node *end = new Node;
    end = front;
    while (end->ptr->ptr!=NULL)
    {
        end=end->ptr;
    }

    lastNode = end;
    end = end->ptr;
    delete end;
    lastNode->ptr = NULL;

    return mv;
}

/**********************************************************************
* RETURN        : string
* PURPOSE       : access the last data w/o modifying the list
**********************************************************************/
string Stack::peek( void ) 
{
    string mv = "";
    mv+=lastNode->from[0];
    mv+=lastNode->from[1];
    mv+=" to ";
    mv+=lastNode->to[0];
    mv+=lastNode->to[1];

    return mv;
}

/**********************************************************************
* RETURN        : int
* PURPOSE       : return the number of items in a stack
**********************************************************************/
int Stack::getItems( void ) 
{
    return items;
}
