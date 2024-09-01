#ifndef LOGIC_H
#define LOGIC_H

void intro();
void result();
int diceValue();
int firstPlayer();
void determineOrder(int startIndex);
void diceRollMessage(int playerIndex, int roll);
int countPiecesOnBase(Piece pieces[TOTAL_PIECES]);
void movePieceOut(Piece pieces[TOTAL_PIECES], int roll, int currentPlayerIndex);
void movePiece(Piece pieces[TOTAL_PIECES], int roll, int currentPlayerIndex, Piece allPieces[TOTAL_PIECES]);

void movePlayer(Piece *pieces, Player *players, int diceValue, int path[], int totalRunningCells, int homeStraightIndex);

#endif // LOGIC_H
