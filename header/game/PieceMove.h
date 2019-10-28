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
        char piece;
        short promote;

        // helper functions
        bool kingMove(Chess *);
        bool queenMove(Chess *);
        bool bshpMove(Chess *);
        bool knghtMove(Chess *); 
        bool rookMove(Chess *);
        bool pawnMove(Chess *);

        void kill(Chess *, char);
    public:
        // Accessor
        short getPrmt() { return promote; }

        // Member functions
        bool validMove(Chess *); // check for valid move
};

#endif
