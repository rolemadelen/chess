## Text-based Chess
This project was done as a final project for C++ Data Structure course.

## Start
Clone the repository locally: <br />
``` bash
$ git clone https://github.com/jmyoi/Chess
$ cd Chess
```

No install is required. Simply compile all `.cpp`:
```bash
$ cd source
$ g++ -o chess *.cpp
$ ./chess
```

## How to move a piece
Moving a piece is straight forward. Enter the *source* (the position of a piece to be moved) and *destination* 
(the position where you want your piece to be at) following this syntax:
```terminal
Player1: [src] to [dest]
```

If Player1 wants to move a pawn located at `b2` to `b4`, type:
```terminal
Player1: b2 to b4
```

## Commands
- `resign` : end the game
- `prev` : display previously done move (or opponents move)
