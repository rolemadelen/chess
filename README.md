## Text-based Chess
My final project for C++ Data Structure course (2016).

## Installation
Clone the repository:
``` bash
$ git clone https://github.com/jmyoi/Chess
$ cd Chess
```

Compule `.cpp` files and run the executable:
```bash
$ cd source
$ g++ -o chess *.cpp
$ ./chess
```

## How to play
Use the following syntax to move a piece from one poistion to other.
```terminal
Player1: [src] to [dest]
```

For example, if *Player1* wants to move a pawn located at `b2` to `b4`, enter
```terminal
Player1: b2 to b4
```

## Commands
- `resign` : end the game
- `prev` : display previously done move (or opponents move)
