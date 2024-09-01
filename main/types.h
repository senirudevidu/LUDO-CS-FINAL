#ifndef TYPES_H
#define TYPES_H

#define BASE -1
#define START_X 0

#define TOTAL_PLAYERS 4
#define TOTAL_PIECES 4

#define TOTAL_CELLS 52
#define HOME_STRAIGHT_SIZE 5
#define TOTAL_RUNNING_CELLS 57
#define LAST_ELEMENT_INDEX 56

// Structure to represent each square on the board
typedef struct Square
{
    int square_number;
    int pieceCount;
    struct Square *prev;
    struct Square *next;
} Square;

// Structure to represent a piece (pawn) on the board
typedef struct
{
    int isInPlay;  // 0: In base, 1: Active on the board, 2: In home straight, 3: Finished
    int direction; // 0: Clockwise, 1: Anticlockwise
    Square *current_square;     // Current location
    int isCapture; // Pointer to the current square the piece is on
    int approachCellPassed;
    int homeStraight;
    int isSick; // if sick = 1 if Energized 2 else 0
    int isSickNumber;
    int mysteryCell;
    int block;
    int brief;              // Cannot move = 0
} Piece;

// Structure to represent a player and their pieces
typedef struct Player
{
    char colour;
    Piece pieces[TOTAL_PIECES]; // Array of pieces (pawns) for the player
    int approachCell;           // The approach cell number for the player
    int hasWon;                 // hasWon = 1 if the player wins              
} Player;

// Structure to represent a Block on the board
typedef struct Block
{
    char colour;
    int blockNumber;
    int pieceCount;
    int pieces[4];
} Block;

#endif // TYPES_H
