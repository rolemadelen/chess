/* 
	Author: Jii Myoi
    Date: April 29, 2016
	Implementation file for the PieceMove Class 
*/

// System Libraries
#include <iostream>
#include <cmath>
using namespace std;

// User Libraries
#include "../header/game/PieceMove.h"
#include "../header/game/Chess.h"

// Symbolic Constants
#define _FROM cp->crg_from
#define _TO   cp->crg_to
#define _LOWS cp->cdp_board[_TO[0]][_TO[1]]>=97 &&\
		   	  cp->cdp_board[_TO[0]][_TO[1]]<=122 
#define _CAPS cp->cdp_board[_TO[0]][_TO[1]]>=65 &&\
			  cp->cdp_board[_TO[0]][_TO[1]]<=90 

///////////////////////////////////////////////////////////
// RETURN    : bool
// PARAMETER : Chess
// PURPOSE   : Determine which piece is moving and 
//	       return true if the move is valid else false
///////////////////////////////////////////////////////////
bool PieceMove::fn_validMv(Chess *chess) {
	ch_piece = chess->cdp_board[chess->crg_from[0]][chess->crg_from[1]];
	sh_promote = 0;
	switch( ch_piece ) {
		case 'k': case 'K':
			return fn_kingMv(chess);
		case 'q': case 'Q':
			return fn_queenMv(chess);
		case 'b': case 'B':
			return fn_bshpMv(chess);
		case 'n': case 'N':
			return fn_knghtMv(chess);
		case 'r': case 'R':
			return fn_rookMv(chess);
		case 'p': case 'P':
			return fn_pawnMv(chess);
		default:
			return false;
	} // end switch
	return true;
}


///////////////////////////////////////////////////////////
// RETURN    : bool
// PARAMETER : Chess
// PURPOSE   : Determine whether pawn's move is valid
///////////////////////////////////////////////////////////
bool PieceMove::fn_pawnMv(Chess *cp) {
	switch( ch_piece ) {
		case 'p':	// player 2 || computer
			// starter-> may move 2 places
			if(_FROM[0]==4 && _FROM[1]==_TO[1] &&
				(_FROM[0]+2==_TO[0] || _FROM[0]+4==_TO[0]) &&
				(cp->cdp_board[_TO[0]][_TO[1]]==' ') )
				return true;
			// return false if pawn moved more than 1 place
			else if(_FROM[0]+2!=_TO[0]) {
				cout << "\tPAWN: ILLEGAL MOVE\n";
				return false;
			}
            // cannot move diagonal: same team
			else if(_LOWS && (_FROM[1]!=_TO[1])) {
				cout << "\tPAWN: WRONG TARGET\n";
				return false;
			}
			break;
		case 'P':	// player 1 || you
			// starter-> may move 2 places
			if(_FROM[0]==14 && _FROM[1]==_TO[1] &&
				(_FROM[0]-2==_TO[0] || _FROM[0]-4==_TO[0]) &&
				(cp->cdp_board[_TO[0]][_TO[1]]==' ') )
				return true;
			// return false if pawn moved more than 1 place
			else if(_FROM[0]-2!=_TO[0]) {
				cout << "\tPAWN: ILLEGAL MOVE\n";
				return false;
			}
            // cannot move diagonal: same team
			else if(_CAPS && (_FROM[1]!=_TO[1])) {
				cout << "\tPAWN: WRONG TARGET\n";
				return false;
			}
	}

	// cannot move front: there is something in front
	if(_FROM[1]==_TO[1] && cp->cdp_board[_TO[0]][_TO[1]]!=' ') {
		cout << "\tPAWN: CANNOT CAPTURE IN THE SAME DIRECTION ?\n";
		return false;
	}
	// cannot move diagonal: there is no piece to capture
	else if(cp->cdp_board[_TO[0]][_TO[1]]==' ' && (_FROM[1]!=_TO[1])) {
		cout << "\tPAWN: TRYING TO CAPTURE SOMETHING?\n";
		return false;
	}

	// Promotion -> pawn reached the otherside
	if(_TO[0]==2 || _TO[0]==16) {
		cout << "\tPromote to:\n";
		cout << "\t1. Queen\n";
		cout << "\t2. Rook\n";
		cout << "\t3. Bishop\n";
		cout << "\t4. Knight\n\t> ";
		cin >> sh_promote;
		cin.ignore();
	}
	
	// Captured something?
	if( cp->cdp_board[_TO[0]][_TO[1]] != ' ') 
		fn_kill(cp, cp->cdp_board[_TO[0]][_TO[1]]);

	// Valid move
	return true;
}

///////////////////////////////////////////////////////////
// RETURN    : bool
// PARAMETER : Chess
// PURPOSE   : Determine whether king's move is valid
///////////////////////////////////////////////////////////
bool PieceMove::fn_kingMv(Chess *cp) {
	short sh_x = abs(_TO[0] - _FROM[0]);
	short sh_y = abs(_TO[1] - _FROM[1]);
	//	absolute valued (x,y) pairs of distance from A to B.
	//	There are 4 pairs (2,2)  (2,0)  (2,4)  (0,2)  
	if( !((sh_x==2&&sh_y==2) || (sh_x==2&&sh_y==0) ||
		(sh_x==2&&sh_y==4) || (sh_x==0&&sh_y==2)) ) {
		cout << "\tKING: INVALID MOVE\n";
		return false;
	}
		
	switch(ch_piece) {
		case 'k':
			if(_LOWS) {
        		cout << "\tKING: WRONG TARGET\n";
				return false;
			}		
			break;
		case 'K':
			if(_CAPS) {
        		cout << "\tKING: WRONG TARGET\n";
				return false;
			}
	}

	// Captured something?
	if( cp->cdp_board[_TO[0]][_TO[1]] != ' ')
		fn_kill(cp, cp->cdp_board[_TO[0]][_TO[1]]);

	// Valid move
	return true;
}

///////////////////////////////////////////////////////////
// RETURN    : bool
// PARAMETER : Chess
// PURPOSE   : Determine whether queen's move is valid
///////////////////////////////////////////////////////////
bool PieceMove::fn_queenMv(Chess *cp) {
	short sh_x = _TO[0] - _FROM[0];
	short sh_y = _TO[1] - _FROM[1];
	bool b_swap, b_swap2;
	b_swap = b_swap2 = false;

	// make x always greater than y, x > y
	if( abs(sh_x) < abs(sh_y) ) {
		swap( sh_x, sh_y );
		b_swap = true;
	}
	
	// (vertical and horizontal move) || (diagonal moves)
	if( !((sh_y==0 && sh_x%2==0) || (abs(sh_x)==abs(sh_y))) ) {
		cout << "\tQUEEN: INVALID MOVE\n";
		return false;
	}

	// if swapped, change it back	
	if( b_swap ) {
		swap( sh_x, sh_y );
		b_swap = false;
	}

	// Invalid move: capturing the same team
	switch( ch_piece ) {
		case 'q':
			if(_LOWS) {
        		cout << "\tQUEEN: WRONG TARGET\n";
				return false;
			}
			break;
		case 'Q':
			if(_CAPS) {
        		cout << "\tQUEEN: WRONG TARGET\n";
				return false;
			}
	}

	// Check vertial moving
	if(_FROM[1]==_TO[1]) {
		// swap values when (going up)
		if(_FROM[0] > _TO[0]) {
			swap(_FROM[0],_TO[0]);
			b_swap = true;
		}

		// check if player's piece is jumping over any pieces
		for(int i=_FROM[0]+2; i<=_TO[0]-2; i+=2)
			if(cp->cdp_board[i][_FROM[1]]!=' ') {
				cout << "\tQUEEN: CANNOT GO THROUGH\n";
				return false;
			} 
	 }
	// Check horizontal moving
	else if(_FROM[0]==_TO[0]){
		// swap values when (going left)
		if(_FROM[1] > _TO[1]) {
			swap(_FROM[1],_TO[1]);
			b_swap2 = true;
		}
		// check if player's piece is jumping over any pieces
		for(int i=_FROM[1]+2; i<=_TO[1]-2; i+=2)
			if(cp->cdp_board[_FROM[0]][i]!=' ') {
				cout << "\tQUEEN: CANNOT GO THROUGH\n";
				return false;
			}
	}
	// check diagonals
	else {
		// negative or positive
		int temp = abs(sh_x)/sh_x;
		int temp2 = abs(sh_y)/sh_y;
		for(int i=2; i<=abs(sh_x)-2; i+=2) {
			if( cp->cdp_board[_FROM[0]+(i*temp)][_FROM[1]+(i*temp2)]!=' ') {
				cout << "\tQUEEN: CANNOT GO THROUGH\n";
				return false;
			}
		}
	} 

	// if swapped, swap it back to original state	
	if(b_swap)
		swap(_FROM[0],_TO[0]);
	else if(b_swap2)
		swap(_FROM[1],_TO[1]);

	// Captured something?
	if( cp->cdp_board[_TO[0]][_TO[1]] != ' ')
		fn_kill(cp, cp->cdp_board[_TO[0]][_TO[1]]);

	// Valid move
	return true;
}

///////////////////////////////////////////////////////////
// RETURN    : bool
// PARAMETER : Chess
// PURPOSE   : Determine whether bishop's move is valid
///////////////////////////////////////////////////////////
bool PieceMove::fn_bshpMv(Chess *cp) {
	short sh_x = _TO[0] - _FROM[0];
	short sh_y = _TO[1] - _FROM[1];
	if( abs(sh_x) != abs(sh_y) ) {
		cout << "\tBISHOP: INVALID MOVE\n";
		return false;
	}

	int temp = abs(sh_x)/sh_x;
	int temp2 = abs(sh_y)/sh_y;
	switch(ch_piece) {
		case 'b':
			// check if player is trying to capture their own piece
			if(_LOWS) {
        		cout << "\tBISHOP: WRONG TARGET\n";
				return false;
			}
			break;
		case 'B':
			// check if player is tring to capture their own piece
			if(_CAPS) {
        		cout << "\tBISHOP: WRONG TARGET\n";
				return false;
			}
	}

	// check if player's piece is jumping over the pieces
	for(int i=2; i<=abs(sh_x)-2; i+=2) {
		if( cp->cdp_board[_FROM[0]+(i*temp)][_FROM[1]+(i*temp2)]!=' ') {
			cout << "\tBISHOP: CANNOT GO THROUGH\n";
			return false;
		} 
	}

	// Captured something?
	if( cp->cdp_board[_TO[0]][_TO[1]] != ' ')
		fn_kill(cp, cp->cdp_board[_TO[0]][_TO[1]]);

	// Valid move
	return true;
}

///////////////////////////////////////////////////////////
// RETURN    : bool
// PARAMETER : Chess
// PURPOSE   : Determine whether knight's move is valid
///////////////////////////////////////////////////////////
bool PieceMove::fn_knghtMv(Chess *cp) {
	short sh_x = abs(_TO[0] - _FROM[0]);
	short sh_y = abs(_TO[1] - _FROM[1]);

	// regardless of the direction,
	// night's abs() distance from A to B is (4,2) or (2,4)
	if( !((sh_x==4 && sh_y==2)||(sh_x==2 && sh_y==4)) )	{
        cout << "\tKNIGHT: ILLEGAL MOVE\n";
		return false;
	}
	
	switch(ch_piece) {
		case 'n':
			// check if player is tring to capture their own piece
			if(_LOWS) {
        		cout << "\tKNIGHT: WRONG TARGET\n";
				return false;
			}
			break;
		case 'N':
			// check if player is tring to capture their own piece
			if(_CAPS) {
        		cout << "\tKNIGHT: WRONG TARGET\n";
				return false;
			}
	}
	// Captured something?
	if( cp->cdp_board[_TO[0]][_TO[1]] != ' ')
		fn_kill(cp, cp->cdp_board[_TO[0]][_TO[1]]);

	// Valid move
	return true;
}

///////////////////////////////////////////////////////////
// RETURN    : bool
// PARAMETER : Chess
// PURPOSE   : Determine whether rook's move is valid
///////////////////////////////////////////////////////////
bool PieceMove::fn_rookMv(Chess *cp) {
    // check if rook is moving diagonally
    if(_FROM[0]!=_TO[0] && _FROM[1]!=_TO[1]) {
        cout << "\tROOK: ILLEGAL MOVE\n";
        return false;
    }

	switch( ch_piece ) {
		case 'r': // player 2 
			// check if player is trying to capture their own piece
			if(_LOWS) {
        		cout << "\tROOK: WRONG TARGET\n";
				return false;
			}
			break;
		case 'R': // player 1
			// check if player is trying to capture their own piece
			if(_CAPS) {
        		cout << "\tROOK: WRONG TARGET\n";
				return false;
			}
	}

	bool b_swap, b_swap2;
	b_swap = b_swap2 = false;

	// Vertical checking
	if(_FROM[1]==_TO[1]) {
		// 2 to 8 (going downward), 8 to 2(going upward)
		// swap values when (going up)
		if(_FROM[0] > _TO[0]) {
			swap(_FROM[0],_TO[0]);
			b_swap = true;
		}

		for(int i=_FROM[0]+2; i<=_TO[0]-2; i+=2)
			if(cp->cdp_board[i][_FROM[1]]!=' ') {
				cout << "\tROOK: CANNOT GO THROUGH\n";
				return false;
			} 
	 }
	// Horizontal checking
	else {
		// 2 to 8 (going right), 8 to 2(going left)
		// swap values when (going left)
		if(_FROM[1] > _TO[1]) {
			swap(_FROM[1],_TO[1]);
			b_swap2 = true;
		}
		
		for(int i=_FROM[1]+2; i<=_TO[1]-2; i+=2)
			if(cp->cdp_board[_FROM[0]][i]!=' ') {
				cout << "\tROOK: CANNOT GO THROUGH\n";
				return false;
			}
	}

	// if swapped, swap it back to original state	
	if(b_swap)
		swap(_FROM[0],_TO[0]);
	else if(b_swap2)
		swap(_FROM[1],_TO[1]);

	// Captured somthing?
	if( cp->cdp_board[_TO[0]][_TO[1]] != ' ')
		fn_kill(cp, cp->cdp_board[_TO[0]][_TO[1]]);

	// Valid move
	return true;
}

///////////////////////////////////////////////////////////
// RETURN    : void
// PARAMETER : Chess, char
// PURPOSE   : Determine which piece was captured and
//			   decrease the remaining number of that piece
///////////////////////////////////////////////////////////
void PieceMove::fn_kill(Chess *cp, char ch) {
	list<pCII >::iterator it;
	switch( (cp->stck_mv.getItems()%2!=0)?2:1 ) {
		case 1: { // player 1
			it = cp->usr->st_p2->pcs.begin();
			while( it!=cp->usr->st_p2->pcs.end() ) {
				if( it->first==ch&&(it->second.first==cp->crg_to[0]&&
					it->second.second==cp->crg_to[1]) ) {
					if( ch=='k' ) cp->usr->st_p2->b_alive = false;
					cp->usr->st_p2->pcs.erase(it);
					it=cp->usr->st_p2->pcs.end();
					cp->usr->st_p1->vc_capture.push_back(ch);
					continue;
				}
				++it;
			}
			break;
		}
		case 2: { // player 2
			it = cp->usr->st_p1->pcs.begin();
			while( it!=cp->usr->st_p1->pcs.end() ) {
				if( it->first==ch&&(it->second.first==cp->crg_to[0]&&
					it->second.second==cp->crg_to[1]) ) {
					if( ch=='K' ) cp->usr->st_p1->b_alive = false;
					cp->usr->st_p1->pcs.erase(it);
					it=cp->usr->st_p1->pcs.end();
					cp->usr->st_p2->vc_capture.push_back(ch);
					continue;
				}
				++it;
			}
		}
	}
}
