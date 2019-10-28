/* 
Author: Jii Yoo
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
#define FROM cp->move_from
#define TO   cp->move_to
#define isLower cp->board[TO[0]][TO[1]]>=97 &&\
                   cp->board[TO[0]][TO[1]]<=122 
#define isUpper cp->board[TO[0]][TO[1]]>=65 &&\
                   cp->board[TO[0]][TO[1]]<=90 

/***********************************************************
* RETURN    : bool
* PARAMETER : Chess
* PURPOSE   : Determine which piece is moving and 
*	       return true if the move is valid else false
***********************************************************/
bool PieceMove::validMove(Chess *chess) 
{
    piece = chess->board[chess->move_from[0]][chess->move_from[1]];
    promote = 0;

    switch (piece) 
    {
        case 'k': 
        case 'K':
            return kingMove(chess);
        case 'q':
        case 'Q':
            return queenMove(chess);
        case 'b': 
        case 'B':
            return bshpMove(chess);
        case 'n': 
        case 'N':
            return knghtMove(chess);
        case 'r': 
        case 'R':
            return rookMove(chess);
        case 'p': 
        case 'P':
            return pawnMove(chess);
        default:
            return false;
    } 

    return true;
}


/***********************************************************
* RETURN    : bool
* PARAMETER : Chess
* PURPOSE   : Determine whether pawn's move is valid
***********************************************************/
bool PieceMove::pawnMove(Chess *cp) 
{
    switch (piece) 
    {
        case 'p':	// player 2 || computer
            // starter-> may move 2 places
            if (FROM[0]==4 && FROM[1]==TO[1]               &&
                   (FROM[0]+2==TO[0] || FROM[0]+4==TO[0]) &&
                   (cp->board[TO[0]][TO[1]]==' '))
            {
                return true;
            }
            // return false if pawn moved more than 1 place
            else if (FROM[0]+2!=TO[0]) 
            {
                cout << "\tPAWN: ILLEGAL MOVE\n";
                return false;
            }
            // cannot move diagonal: same team
            else if (isLower && (FROM[1]!=TO[1])) 
            {
                cout << "\tPAWN: WRONG TARGET\n";
                return false;
            }
            break;
        case 'P':	// player 1 || you
            // starter-> may move 2 places
            if (FROM[0]==14 && FROM[1]==TO[1]               &&
                    (FROM[0]-2==TO[0] || FROM[0]-4==TO[0]) &&
                    (cp->board[TO[0]][TO[1]]==' '))
            {
                return true;
            }
            // return false if pawn moved more than 1 place
            else if (FROM[0]-2!=TO[0]) 
            {
                cout << "\tPAWN: ILLEGAL MOVE\n";
                return false;
            }
            // cannot move diagonal: same team
            else if (isUpper && (FROM[1]!=TO[1])) 
            {
                cout << "\tPAWN: WRONG TARGET\n";
                return false;
            }
        default:
            break;
    }

    // cannot move front: there is something in front
    if (FROM[1]==TO[1] && cp->board[TO[0]][TO[1]]!=' ') 
    {
        cout << "\tPAWN: CANNOT CAPTURE IN THE SAME DIRECTION ?\n";
        return false;
    }
    // cannot move diagonal: there is no piece to capture
    else if (cp->board[TO[0]][TO[1]]==' ' && (FROM[1]!=TO[1])) 
    {
        cout << "\tPAWN: TRYING TO CAPTURE SOMETHING?\n";
        return false;
    }

    // Promotion -> pawn reached the otherside
    if (TO[0]==2 || TO[0]==16) 
    {
        cout << "\tPromote to:\n";
        cout << "\t1. Queen\n";
        cout << "\t2. Rook\n";
        cout << "\t3. Bishop\n";
        cout << "\t4. Knight\n\t> ";
        cin >> promote;
        cin.ignore();
    }

    // Captured something?
    if (cp->board[TO[0]][TO[1]] != ' ')
    { 
        kill(cp, cp->board[TO[0]][TO[1]]);
    }

    return true;
}

/***********************************************************
* RETURN    : bool
* PARAMETER : Chess
* PURPOSE   : Determine whether king's move is valid
***********************************************************/
bool PieceMove::kingMove(Chess *cp) {
    short sh_x = abs(TO[0] - FROM[0]);
    short sh_y = abs(TO[1] - FROM[1]);
    //	absolute valued (x,y) pairs of distance from A to B.
    //	There are 4 pairs (2,2)  (2,0)  (2,4)  (0,2)  
    if (!((sh_x==2&&sh_y==2) || (sh_x==2&&sh_y==0) ||
          (sh_x==2&&sh_y==4) || (sh_x==0&&sh_y==2))) 
    {
        cout << "\tKING: INVALID MOVE\n";
        return false;
    }

    switch (piece) 
    {
        case 'k':
            if (isLower) 
            {
                cout << "\tKING: WRONG TARGET\n";
                return false;
            }		
            break;
        case 'K':
            if (isUpper) 
            {
                cout << "\tKING: WRONG TARGET\n";
                return false;
            }
        default:
            break;
    }

    // Captured something?
    if (cp->board[TO[0]][TO[1]] != ' ')
    {
        kill(cp, cp->board[TO[0]][TO[1]]);
    }

    // Valid move
    return true;
}

/***********************************************************
* RETURN    : bool
* PARAMETER : Chess
* PURPOSE   : Determine whether queen's move is valid
***********************************************************/
bool PieceMove::queenMove(Chess *cp) 
{
    short sh_x = TO[0] - FROM[0];
    short sh_y = TO[1] - FROM[1];
    bool b_swap, b_swap2;
    b_swap = b_swap2 = false;

    // make x always greater than y, x > y
    if (abs(sh_x) < abs(sh_y)) 
    {
        swap(sh_x, sh_y);
        b_swap = true;
    }

    // (vertical and horizontal move) || (diagonal moves)
    if (!((sh_y==0 && sh_x%2==0) || (abs(sh_x)==abs(sh_y))))
    {
        cout << "\tQUEEN: INVALID MOVE\n";
        return false;
    }

    // if swapped, change it back	
    if (b_swap) 
    {
        swap( sh_x, sh_y );
        b_swap = false;
    }

    // Invalid move: capturing the same team
    switch (piece)
    {
        case 'q':
            if (isLower) 
            {
                cout << "\tQUEEN: WRONG TARGET\n";
                return false;
            }
            break;
        case 'Q':
            if (isUpper) 
            {
                cout << "\tQUEEN: WRONG TARGET\n";
                return false;
            }
            break;
        default:
            break;
    }

    // Check vertial moving
    if (FROM[1]==TO[1]) 
    {
        // swap values when (going up)
        if(FROM[0] > TO[0]) 
        {
            swap(FROM[0],TO[0]);
            b_swap = true;
        }

        // check if player's piece is jumping over any pieces
        for (int i=FROM[0]+2; i<=TO[0]-2; i+=2)
        {
            if (cp->board[i][FROM[1]]!=' ') 
            {
                cout << "\tQUEEN: CANNOT GO THROUGH\n";
                return false;
            } 
        }
    }
    // Check horizontal moving
    else if (FROM[0]==TO[0])
    {
        // swap values when (going left)
        if (FROM[1] > TO[1]) 
        {
            swap(FROM[1],TO[1]);
            b_swap2 = true;
        }
        // check if player's piece is jumping over any pieces
        for(int i=FROM[1]+2; i<=TO[1]-2; i+=2)
        {
            if(cp->board[FROM[0]][i]!=' ') 
            {
                cout << "\tQUEEN: CANNOT GO THROUGH\n";
                return false;
            }
        }
    }
    // check diagonals
    else 
    {
        // negative or positive
        int temp = abs(sh_x)/sh_x;
        int temp2 = abs(sh_y)/sh_y;
        for (int i=2; i<=abs(sh_x)-2; i+=2) 
        {
            if (cp->board[FROM[0]+(i*temp)][FROM[1]+(i*temp2)]!=' ') 
            {
                cout << "\tQUEEN: CANNOT GO THROUGH\n";
                return false;
            }
        }
    } 

    // if swapped, swap it back to original state	
    if(b_swap)
    {
        swap(FROM[0],TO[0]);
    }
    else if(b_swap2)
    {
        swap(FROM[1],TO[1]);
    }

    // Captured something?
    if (cp->board[TO[0]][TO[1]] != ' ')
    {
        kill(cp, cp->board[TO[0]][TO[1]]);
    }

    return true;
}

/***********************************************************
* RETURN    : bool
* PARAMETER : Chess
* PURPOSE   : Determine whether bishop's move is valid
***********************************************************/
bool PieceMove::bshpMove(Chess *cp) 
{
    short sh_x = TO[0] - FROM[0];
    short sh_y = TO[1] - FROM[1];
    if (abs(sh_x) != abs(sh_y)) 
    {
        cout << "\tBISHOP: INVALID MOVE\n";
        return false;
    }

    int temp = abs(sh_x)/sh_x;
    int temp2 = abs(sh_y)/sh_y;
    switch (piece) 
    {
        case 'b':
            // check if player is trying to capture their own piece
            if (isLower) 
            {
                cout << "\tBISHOP: WRONG TARGET\n";
                return false;
            }
            break;
        case 'B':
            // check if player is tring to capture their own piece
            if (isUpper) 
            {
                cout << "\tBISHOP: WRONG TARGET\n";
                return false;
            }
            break;
        default:
            break;
    }

    // check if player's piece is jumping over the pieces
    for (int i=2; i<=abs(sh_x)-2; i+=2) 
    {
        if (cp->board[FROM[0]+(i*temp)][FROM[1]+(i*temp2)]!=' ') 
        {
            cout << "\tBISHOP: CANNOT GO THROUGH\n";
            return false;
        } 
    }

    // Captured something?
    if (cp->board[TO[0]][TO[1]] != ' ')
    {
        kill(cp, cp->board[TO[0]][TO[1]]);
    }

    return true;
}

/***********************************************************
* RETURN    : bool
* PARAMETER : Chess
* PURPOSE   : Determine whether knight's move is valid
***********************************************************/
bool PieceMove::knghtMove(Chess *cp) 
{
    short sh_x = abs(TO[0] - FROM[0]);
    short sh_y = abs(TO[1] - FROM[1]);

    // regardless of the direction,
    // night's abs() distance from A to B is (4,2) or (2,4)
    if (!((sh_x==4 && sh_y==2)||(sh_x==2 && sh_y==4)))	
    {
        cout << "\tKNIGHT: ILLEGAL MOVE\n";
        return false;
    }

    switch(piece) 
    {
        case 'n':
            // check if player is tring to capture their own piece
            if (isLower) 
            {
                cout << "\tKNIGHT: WRONG TARGET\n";
                return false;
            }
            break;
        case 'N':
            // check if player is tring to capture their own piece
            if (isUpper) 
            {
                cout << "\tKNIGHT: WRONG TARGET\n";
                return false;
            }
            break;
        default:
            break;
    }

    // Captured something?
    if (cp->board[TO[0]][TO[1]] != ' ')
    {
        kill(cp, cp->board[TO[0]][TO[1]]);
    }

    return true;
}

/***********************************************************
* RETURN    : bool
* PARAMETER : Chess
* PURPOSE   : Determine whether rook's move is valid
***********************************************************/
bool PieceMove::rookMove(Chess *cp) 
{
    // check if rook is moving diagonally
    if (FROM[0]!=TO[0] && FROM[1]!=TO[1]) 
    {
        cout << "\tROOK: ILLEGAL MOVE\n";
        return false;
    }

    switch (piece) 
    {
        case 'r': // player 2 
            // check if player is trying to capture their own piece
            if (isLower) 
            {
                cout << "\tROOK: WRONG TARGET\n";
                return false;
            }
            break;
        case 'R': // player 1
            // check if player is trying to capture their own piece
            if (isUpper) 
            {
                cout << "\tROOK: WRONG TARGET\n";
                return false;
            }
    }

    bool b_swap, b_swap2;
    b_swap = b_swap2 = false;

    // Vertical checking
    if (FROM[1]==TO[1]) 
    {
        // 2 to 8 (going downward), 8 to 2(going upward)
        // swap values when (going up)
        if (FROM[0] > TO[0]) 
        {
            swap(FROM[0],TO[0]);
            b_swap = true;
        }

        for (int i=FROM[0]+2; i<=TO[0]-2; i+=2)
        {
            if (cp->board[i][FROM[1]]!=' ') 
            {
                cout << "\tROOK: CANNOT GO THROUGH\n";
                return false;
            } 
        }
    }
    // Horizontal checking
    else 
    {
        // 2 to 8 (going right), 8 to 2(going left)
        // swap values when (going left)
        if (FROM[1] > TO[1]) 
        {
            swap(FROM[1],TO[1]);
            b_swap2 = true;
        }

        for (int i=FROM[1]+2; i<=TO[1]-2; i+=2)
        {
            if (cp->board[FROM[0]][i]!=' ') 
            {
                cout << "\tROOK: CANNOT GO THROUGH\n";
                return false;
            }
        }
    }

    // if swapped, swap it back to original state	
    if (b_swap)
    {
        swap(FROM[0],TO[0]);
    }
    else if(b_swap2)
    {
        swap(FROM[1],TO[1]);
    }

    // Captured somthing?
    if (cp->board[TO[0]][TO[1]] != ' ')
    {
        kill(cp, cp->board[TO[0]][TO[1]]);
    }

    return true;
}

/***********************************************************
* RETURN    : void
* PARAMETER : Chess, char
* PURPOSE   : Determine which piece was captured and
*			   decrease the remaining number of that piece
***********************************************************/
void PieceMove::kill(Chess *cp, char ch) 
{
    list<pCII >::iterator it;
    switch ((cp->moves.getItems()%2!=0)?2:1 ) 
    {
        case 1: 
        { // player 1
            it = cp->user->st_p2->pcs.begin();
            while (it!=cp->user->st_p2->pcs.end()) 
            {
                if (it->first==ch&&(it->second.first==cp->move_to[0]&&
                            it->second.second==cp->move_to[1])) 
                {
                    if (ch=='k')
                    {
                        cp->user->st_p2->b_alive = false;
                    }

                    cp->user->st_p2->pcs.erase(it);
                    it=cp->user->st_p2->pcs.end();
                    cp->user->st_p1->vc_capture.push_back(ch);
                    continue;
                }

                ++it;
            }
            break;
        }
        case 2: 
        { // player 2
            it = cp->user->st_p1->pcs.begin();
            while (it!=cp->user->st_p1->pcs.end()) 
            {
                if (it->first==ch&&(it->second.first==cp->move_to[0]&&
                            it->second.second==cp->move_to[1])) 
                {
                    if (ch=='K')
                    {
                        cp->user->st_p1->b_alive = false;
                    }

                    cp->user->st_p1->pcs.erase(it);
                    it=cp->user->st_p1->pcs.end();
                    cp->user->st_p2->vc_capture.push_back(ch);
                    continue;
                }
                ++it;
            }
        }
    }
}
