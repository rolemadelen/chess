/*
Author: Jii Yoo
Date: 2 May 2016
Specification file for the User structure
*/

#ifndef USER_H
#define USER_H

// System Libraries
#include <list>
#include <vector>

// User Libraries
#include "Player.h"
#include "Computer.h"

struct User {
    Player *st_p1;
    Computer *st_p2;

    vector<float> vf_tmPerMv;
    string str_move;
};

#endif
