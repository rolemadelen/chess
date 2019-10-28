/* 
Author: Jii Yoo
Date: April 29, 2016
Specification file for the Node structure 
*/

#ifndef NODE_H
#define NODE_H

struct Node {
    Node *ptr;
    char from[2]; // chess moves [from->to]
    char to[2];
};

#endif
