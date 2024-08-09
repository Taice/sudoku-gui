#pragma once

#include <stdbool.h>

#define SIZE 9
#define SUBGRID_SIZE 3

bool isValid(char sudoku[SIZE][SIZE]);
bool backtrack(char sudoku[SIZE][SIZE]);
short isWinnable(char sudoku[SIZE][SIZE]);
int getBoxIndex(int row, int col);
void getMissingBox(int row, int col, bool missing[SIZE], char sudoku[SIZE][SIZE]);
void getMissingCol(int col, bool missing[SIZE], char sudoku[SIZE][SIZE]);
void getMissingRow(int row, bool missing[SIZE], char sudoku[SIZE][SIZE]);
void calcCheats(bool cheats[SIZE][SIZE][SIZE], char sudoku[SIZE][SIZE]);
