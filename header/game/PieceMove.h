/*
Author: Jii Yoo
Date: April 29, 2016
Secification file for the PieceMove class 
*/

#ifndef PIECEMOVE_H
#define PIECEMOVE_H
#include "../game/Chess.h"

class PieceMove{
    private:
        char ch_piece;
        short sh_promote;

        // helper functions
        bool fn_kingMv(Chess *);
        bool fn_queenMv(Chess *);
        bool fn_bshpMv(Chess *);
        bool fn_knghtMv(Chess *); 
        bool fn_rookMv(Chess *);
        bool fn_pawnMv(Chess *);

        void fn_kill(Chess *, char);
    public:
        // Accessor
        short fn_getPrmt()
        { return sh_promote; }

        // Member functions
        bool fn_validMv(Chess *); // check for valid move
};

#endif
