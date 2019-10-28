/*
Author: Jii Yoo
Date: 21 April 2016
Implementation file for the Chess class
*/

// System Libraries
#include <algorithm> // sort
#include <iostream>
#include <fstream> // file IO
#include <cstdlib> // NULL
#include <iomanip> // setw
#include <string.h>
#include <map>
using namespace std;

// User Libraries
#include "../header/game/Chess.h"
#include "../header/game/PieceMove.h"
#include "../header/etc/PreciseClock.h"

// Symbolic Constants
#define ROW 19
#define COL 19
#define HEADER BGRED << setw(37) << ' ' << BGDEF

// Global Constants
#ifdef _WIN32 // use GREEN in windows else BLUE
const Modifier GBLU(FG_GREEN);
#else
const Modifier GBLU(FG_BLUE);
#endif
const Modifier RED(FG_RED); // change font color
const Modifier DEF(FG_DEFAULT);

const Modifier BGRED(BG_RED); // change background color
const Modifier BGDEF(BG_DEFAULT);

// User defined libraries
typedef list <pCII >::iterator t_it;

/*********************************************************************
 *                         Destructor
 *********************************************************************/
Chess::~Chess( void )
{
    for (int i=0; i<10; ++i)
    {
        delete cdp_board[i];
    }
    delete [] cdp_board;
    cdp_board = NULL;

    delete user->st_p1;
    delete user->st_p2;
    delete user;
}

/*********************************************************************
 * RETURN        : void
 * PURPOSE       : Initialize/set all required game data
**********************************************************************/
void Chess::init( void ) 
{
    // allocate memory to structures
    user = new User();
    user->st_p1 = new Player();
    user->st_p2 = new Computer();

    // Allocate memmory to 19x19 charcter array
    cdp_board = new char*[ROW];
    for (int i=0; i<ROW; ++i)
    {
        cdp_board[i] = new char[COL];
    }

    // Initialize the chess board with blank spaces
    for (int i=0; i<ROW; ++i)
    {
        for (int j=0; j<COL; ++j) 
        {
            cdp_board[i][j] = ' ';
        }
    }

    // Label ranks(1-8) and files(a-h)
    for (int i=0; i<ROW; ++i) 
    {
        for (int j=0; j<COL; ++j) 
        {
            // if odd # row, draw the horizontal grid
            if ((i&1)==1)  
            {
                if (j!=0 && j!=COL-1)
                    cdp_board[i][j] = '-';
            }
            // if even # row
            else if ((i&0)==0) 
            {
                // in first and lost column, label ranks(1-8)
                if (i>1 && i<ROW-1 && (j==0 || j==COL-1))
                {
                    cdp_board[i][j] = '9'-(i/2);
                }
                // if odd # col, draw the vertical grid
                if ((j&1)==1)
                {
                    if (i!=0 && i!=ROW-1)
                    {
                        cdp_board[i][j] = '|';
                    }
                }
                // in first and last row, label flies(a-h)
                if ((j&1)!=1 && (i==0 || i==ROW-1) && j>1 && j<COL-1)
                {
                    cdp_board[i][j] = 'a'+(j/2)-1;
                }
            } 
        }
    }
}

/**********************************************************************
 * RETURN        : void
 * PARAMETER     : bool player1
 * PURPOSE       : validate the player's move and move a piece
**********************************************************************/
void Chess::makeMove(bool p1) 
{
    clear();
    display();
    // Display player's name
    cout << endl << ((p1)?GBLU:RED)
        << ((p1)?(user->st_p1->str_name):(user->st_p2->str_name)) 
        << ": "  << DEF;
    // time how long it takes for a user to make a move
    uint64 begin = GetTimeMs64();
    getline(cin, user->str_move);
    uint64 end = GetTimeMs64();

    // while user input is invalid
    while (!isValid(user->str_move))
    {
        cout << endl << ((p1)?GBLU:RED)
            << ((p1)?(user->st_p1->str_name):(user->st_p2->str_name)) 
            << ": "  << DEF;
        begin += GetTimeMs64();
        getline(cin, user->str_move);
        end += GetTimeMs64();
    }

    // player resigns
    if ((user->str_move).compare("resign")==0) 
    {
        ((p1)?(user->st_p1->b_alive):(user->st_p2->b_alive)) = false;
        return;
    }
    // record moves
    moves.push_back(user->str_move);
    // record times
    user->vf_tmPerMv.push_back((end-begin)/1000.0f);

    //  move selected piece
    t_it it = ((p1)?(user->st_p1->pcs.begin()):(user->st_p2->pcs.begin()));
    while (it!=((p1)?(user->st_p1->pcs.end()):(user->st_p2->pcs.end()))) 
    {
        if (it->second.first==crg_from[0] && it->second.second==crg_from[1]) 
        {
            it->second.first = crg_to[0];
            it->second.second = crg_to[1];
            cdp_board[crg_from[0]][crg_from[1]] = ' ';
            it = ((p1)?(user->st_p1->pcs.end()):(user->st_p2->pcs.end())); 
            return;
        }
        ++it;
    }
}

/**********************************************************************
 * RETURN        : void
 * PARAMETER     : bool p1
 * PURPOSE       : validate the piece's movement && perform promotion
 **********************************************************************/
bool Chess::checkMove( bool p1 ) 
{
    PieceMove chkMv;
    // check if p2 selected p1's piece or vice versa
    char ch = cdp_board[crg_from[0]][crg_from[1]];
    if (((p1)?(ch>='a'):(ch<'a'))) 
    {
        cout << "\tERROR: NOT YOUR PIECE.\n";
        return false;
    }
    // check whether piece's move is valid
    if (!chkMv.fn_validMv(this)) 
    {
        return false;
    }
    // check for pawn's promotion
    if (chkMv.fn_getPrmt()!=0) 
    {
        // Pawn Promotion
        char promo = (p1)?'P':'p';
        switch (chkMv.fn_getPrmt()) 
        {
            case 1: 
                promo = ((p1)?'Q':'q'); 
                break;
            case 2: 
                promo = ((p1)?'R':'r'); 
                break;
            case 3: 
                promo = ((p1)?'B':'b'); 
                break;
            case 4: 
                promo = ((p1)?'N':'n'); 
                break;
            default:
                break;
        } 

        // Promote the pawn to a desired piece
        t_it it = (p1)?(user->st_p1->pcs.begin()):(user->st_p2->pcs.begin());
        while (it!=((p1)?(user->st_p1->pcs.end()):(user->st_p2->pcs.end()))) 
        {
            if (it->first==((p1)?'P':'p')&&(it->second.first==crg_from[0]&&
                        it->second.second==crg_from[1])) 
            {
                it->first = promo;
                it = ((p1)?(user->st_p1->pcs.end()):(user->st_p2->pcs.end()));
            }
            ++it;
        } 
    }

    return true;
}

/**********************************************************************
 * RETURN    : bool
 * PARAMETER : string
 * PURPOSE   : deteremine whether player's move is valid or not;
 *				return true if valid else false
 *********************************************************************/
bool Chess::isValid( string &str ) 
{
    // remove all spaces from the user input
    for (int i=0; i<str.size(); ++i) 
    {
        if (str[i]==' ') 
        {
            str.erase(i--,1);
        }
        str[i] = tolower(str[i]);
    }

    // player resigns the game;
    if (str.compare("resign")==0)
    {
        return true;
    }

    // player views the previous movement
    if (str.compare("prev")==0) 
    {
        cout << " Previous move: " << moves.peek() << endl;
        return false;
    }
    // a2toa3 -> size should be 6
    if (str.size()!=6) 
    {
        cout << "\tSYNTAX ERROR: 6 CHARS PLEASE...\n";
        return false;
    }
    // a2toa3 -> check for valid files[a-h]
    if (!(str[0]>=97&&str[0]<=104) || !(str[4]>=97&&str[4]<=104)) 
    {
        cout << "\tSYNTAX ERROR: FILES [a-h]\n";
        return false;
    }
    // a2toa3 -> check for valid ranks[1-8]
    if (!(str[1]>='1'&&str[1]<='8') || !(str[5]>='1'&&str[5]<='8')) 
    {
        cout << "\tSYNTAX ERROR: 6 RANKS [1-8]\n";
        return false;
    }
    // a2toa3 -> check for "to"
    if (str[2]!='t' || str[3]!='o')
    {
        cout << "\tSYNTAX ERROR: 'to/TO'? \n";
        return false;
    }
    // a2toa2 -> check for stupidity
    if (str[1]==str[5] && str[0]==str[4]) 
    {
        cout << "\tERROR: SUICIDE?\n";
        return false;
    }

    // extract files and ranks from the string
    crg_from[1] = (tolower(str[0])-'a'+1)*2;
    crg_from[0] = 18-((str[1]-'0')*2);
    crg_to[1] = (tolower(str[4])-'a'+1)*2;
    crg_to[0] = 18-((str[5]-'0')*2);

    // check if blank piece was selected
    if (cdp_board[crg_from[0]][crg_from[1]] == ' ') 
    {
        cout << "\tERROR: YOU GOTTA MOVE SOMETHING?\n";
        return false;	
    }

    //PieceMove chkMv;
    switch ((moves.getItems()%2!=0) ? 0 : 1) 
    {
        case 1: 
            return checkMove(true); // player 1
        case 0: 
            return checkMove(false); // player 2
    } // end switch 

    return true;
}

/**********************************************************************
 * RETURN        : void
 * PURPOSE       : display the final result
 *********************************************************************/
void Chess::gameover() 
{
    clear();
    display();
    cout << endl;

    // Display the winner
    if (user->st_p1->b_alive) 
    {
        cout << GBLU << user->st_p1->str_name << " WON !!" << DEF << endl;
    }
    else 
    {
        cout << RED << user->st_p2->str_name << " WON !!" << DEF << endl;
    }

    // Display all the moves if user wants to
    cout << endl;
    cout << " Display all played moves? (Y/N): ";
    char temp; cin >> temp;
    float timeP1=0.0f,timeP2=0.0f;

    if (tolower(temp)=='y') 
    {
        for (int i=moves.getItems(),j=user->vf_tmPerMv.size()-1; i>0; --i,--j) 
        {
            cout << " Move #" << i << " - " << ((i%2!=0)?GBLU:RED);
            cout << "Player " << ((i%2!=0)?1:2) 
                << ": " << moves.pop_back() << " - "
                << "took " << user->vf_tmPerMv[j] << " seconds to move "
                << DEF << endl;
        }
        cout << endl;
    } 
    else if (tolower(temp)!='n')
    {
        cout << "error..." << endl;
    }

    // sum up player's total spent time moving pieces
    for (int i=user->vf_tmPerMv.size()-1; i>0; --i) 
    {
        if (i%2==0)
        {
            user->st_p1->f_ttlTm+=user->vf_tmPerMv[i];
        }
        else 
        {
            user->st_p2->f_ttlTm+=user->vf_tmPerMv[i];
        }
    }

    // sort then display fastest and slowest time
    if (user->vf_tmPerMv.size())
    {
        sort(user->vf_tmPerMv.begin(), user->vf_tmPerMv.end());
        cout << " Fastest time took to move a piece: "
            << user->vf_tmPerMv[0] << " seconds\n";
        cout << " Slowest time took to move a piece: "
            << user->vf_tmPerMv[user->vf_tmPerMv.size()-1] << " seconds\n\n";
    }

    scoreboard( false );
}

/**********************************************************************
 * RETURN        : void
 * PURPOSE       : start the game
 *********************************************************************/
void Chess::playGame( void ) 
{
    clear();
    cout << "Name of player1: ";
    cin >> user->st_p1->str_name;
    cout << "Name of player2: ";
    cin >> user->st_p2->str_name;
    cin.ignore();

    // player turn
    bool pTurn = true; 
    while (user->st_p1->b_alive&&user->st_p2->b_alive) 
    {
        if(pTurn) 
        { 
            makeMove(true); 
            pTurn = false; 
        }
        else  
        { 
            makeMove(false); 
            pTurn = true; 
        }
    }

    gameover();
}

/*********************************************************************
 * RETURN        : void
 * PURPOSE       : dispaly the chessboard
 ********************************************************************/
void Chess::display( void ) 
{
    drawPieces();
    status();

    // display the board
    for (int i=0; i<ROW; ++i) 
    {
        for (int j=0; j<COL; ++j) 
        {
            char ch = cdp_board[i][j];
            // p1->blue, p2->red
            if (ch>='a'&&ch<='z'&&(i!=0&&i!=ROW-1))
            {
                cout << RED;
            }
            else if (ch>='A'&&ch<='Z' ) 
            {
                cout << GBLU;
            }
            cout << cdp_board[i][j] << DEF << ' ';
        } 
        cout << endl;
    }
}

/**********************************************************************
 * RETURN        : void
 * PURPOSE       : locate each chess pieces on the grid
 *********************************************************************/
void Chess::drawPieces( void ) 
{
    t_it it = user->st_p1->pcs.begin();
    while (it!=user->st_p1->pcs.end())
    {
        cdp_board[it->second.first][it->second.second] = (it++)->first;
    }

    it = user->st_p2->pcs.begin();
    while (it!=user->st_p2->pcs.end())
    {
        cdp_board[it->second.first][it->second.second] = (it++)->first;
    }
}

/**********************************************************************
 * RETURN        : void
 * PURPOSE       : clear the screen
 *********************************************************************/
void Chess::clear( void ) 
{
    for (int i=0; i<50; ++i)
    {
        cout << endl;
    }
}

/**********************************************************************
 * RETURN        : void
 * PURPOSE       : Display player's status of captured pieces;
 *********************************************************************/
void Chess::status( void ) 
{
    // sort captured pieces
    sort(user->st_p1->vc_capture.begin(), user->st_p1->vc_capture.end());
    sort(user->st_p2->vc_capture.begin(), user->st_p2->vc_capture.end());

    // Display Captured pieces
    cout << endl << HEADER << endl;
    cout << " Captured piece(s): " << endl;
    cout << GBLU << " Player 1 :" << DEF << "\n\t"; 
    for (int i=0; i<user->st_p1->vc_capture.size(); ++i) 
    {
        cout << RED << user->st_p1->vc_capture[i] << DEF << ' ';
        if ((i+1)%8==0)
        {
            cout << "\n\t";
        }
    }
    cout << endl << endl;
    cout << RED << " Player 2 :" << DEF << "\n\t"; 
    for (int i=0; i<user->st_p2->vc_capture.size(); ++i)
    {
        cout << GBLU << user->st_p2->vc_capture[i] << DEF << ' ';
        if ((i+1)%8==0) 
        {
            cout << "\n\t";
        }
    }
    cout << endl;

    // Display Syntax Information
    cout << endl << HEADER << endl;
    cout << " Syntax: " << endl;
    cout << "      a2 to a3     ->    ?? to ??\n\n";
    cout << " Commands: " << endl;
    cout << "      \"resign\" -> resign the game\n";
    cout << "      \"prev\" -> display previous move";
    cout << endl << HEADER << endl << endl;
    cout << RED << " NO" << DEF << " Castling\n\n";
}

/**********************************************************************
 * RETURN        : void
 * PARAMETER     : bool print
 * PURPOSE       : if true, only display data w/o writing to a file
 *					else only save data to the file w/o displaying
 *********************************************************************/
void Chess::scoreboard( bool print ) 
{	
    map<float, string> mymap; // <ttlTime,name>
    map<float, string>::iterator it;
    fstream file("score_board.dat",ios::in);

    // read data from a file
    if (!file.fail()) 
    {
        string str; float f;
        while (file >> str >> f)
        { 
            mymap[f] = str;
        }
    }
    file.close();

    // display the score board if print is true
    if (print) 
    {
        it = mymap.begin();
        cout << setfill('=') << setw(40) << ' ' << setfill(' ') << endl;
        cout << " Winner " << setw(10) << ' ' << "Total Time Spent\n";
        cout << setfill('=') << setw(40) << ' ' << setfill(' ') << endl;
        while (it!=mymap.end()) 
        {
            cout << left << ' '  << setw(10) << (it++)->second << setw(10) 
                << ' ' << it->first << " seconds" << endl;
        }
        cout << right << "\n Press Enter .............\n";
        cin.ignore(255,'\n');
        getchar();
        return;
    }

    // add new winner and winner's time to the map
    if( user->st_p1->b_alive ) 
    {
        mymap[user->st_p1->f_ttlTm] = user->st_p1->str_name;
    }
    else
    {
        mymap[user->st_p2->f_ttlTm] = user->st_p2->str_name;
    }

    // write data to a file	
    file.open("score_board.dat",ios::out);
    it = mymap.begin();
    while (it!=mymap.end()) 
    {
        file << (it++)->second << setw(5) << ' ' << it->first << endl;
    }

    file.close();
}

//////////////////////////////////////////////////////////////////////
// RETURN        : void
// PURPOSE       : Display the menu
//////////////////////////////////////////////////////////////////////
void Chess::menu( void ) 
{
    int n; 
    do
    {
        clear();
        cout << " 1. Play Game\n";
        cout << " 2. View Scoreboard\n";
        cout << " 0. exit\n > ";
        cin >> n;

        while (cin.fail()) 
        {
            cin.clear();
            cin.ignore(255, '\n');
            cout << " > ";
            cin >> n;
        }

        switch(n) 
        {
            case 0: 
                return;
            case 1: 
                playGame(); 
                break;
            case 2: 
                scoreboard(true); 
                break;
        }
    } while (n!=1);
}
