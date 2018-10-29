/*
	Author: Jii Myoi
	Date: 21 April 2016
	Chess structure file for the player 1
*/

#ifndef PLAYER_H
#define PLAYER_H

// System Libraries
#include <list>
#include <vector>

// User defined types
typedef pair<int,int> pII;
typedef pair<char, pair<int,int> > pCII;

struct Player {
	list<pCII > pcs;
	vector<char> vc_capture;
	bool b_alive;
	std::string str_name;
	float f_ttlTm;
	// initialize player side pcs starting location
	Player() {
		for(int i=0;i<8;++i)
			pcs.push_back( pCII('P', pII(14,(2*i)+2)) );
		pcs.push_back( pCII('R', pII(16,2)) );
		pcs.push_back( pCII('R', pII(16,16)) );
		pcs.push_back( pCII('N', pII(16,4)) );
		pcs.push_back( pCII('N', pII(16,14)) );
		pcs.push_back( pCII('B', pII(16,6)) );
		pcs.push_back( pCII('B', pII(16,12)) );
		pcs.push_back( pCII('Q', pII(16,8))) ;
		pcs.push_back( pCII('K', pII(16,10)) );
		b_alive = true;
		f_ttlTm = 0.0f;
	}
};

#endif
