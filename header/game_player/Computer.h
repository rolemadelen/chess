/*
Author: Jii Yoo
Date: 21 April 2016
Chess piece structure file for the Computer (or player 2)
*/

#ifndef COMPUTER_H
#define COMPUTER_H

// System Libraries
#include <list>
#include <vector>

// User defined types
typedef pair<int,int> pII;
typedef pair<char, pair<int,int> > pCII;

struct Computer {
    list<pCII > pcs;
    vector<char> vc_capture;
    bool b_alive;
    std::string str_name;
    float f_ttlTm;

    // initialize computer side pcs starting location
    Computer() {
        for(int i=0;i<8;++i)
            pcs.push_back( pCII('p', pII(4,(2*i)+2)) );
        pcs.push_back( pCII('r', pII(2,2)) );
        pcs.push_back( pCII('r', pII(2,16) ));
        pcs.push_back( pCII('n', pII(2,4)) );
        pcs.push_back( pCII('n', pII(2,14) ));
        pcs.push_back( pCII('b', pII(2,6)) );
        pcs.push_back( pCII('b', pII(2,12) ));
        pcs.push_back( pCII('q', pII(2,8)) );
        pcs.push_back( pCII('k', pII(2,10) ));
        b_alive = true;
        f_ttlTm = 0.0f;
    }
};

#endif
