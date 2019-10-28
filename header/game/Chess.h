/*
Author: Jii Yoo
Date: 21 April 2016
Specification file for the Chess class
*/

#ifndef CHESS_H
#define CHESS_H

// User Libraries
#include "../stack/Stack.h"
#include "../game_player/User.h"
#include "../etc/Colormod.h"
using namespace Color;

class Chess {
    // Let PieceMove class access Chess' private variables
    friend class PieceMove;

    private:
        User *user;
        Stack moves;	

        char **cdp_board;  // chess board
        char crg_from[2];  // move piece location from->to
        char crg_to[2];

        // Helper functions;
        void init( void ); // game - body/logic
        void makeMove( bool );
        bool checkMove( bool );
        bool isValid( string & );
        void gameover( void );
        void playGame( void );

        void display( void ); // game - graphic(?) part
        void drawPieces( void );
        void clear( void );
        void status( void );

        void scoreboard( bool );  // file IO
    public:
        Chess() { init(); } // Constructor
        ~Chess( void ); // Destructor
        void menu( void );
};

#endif
