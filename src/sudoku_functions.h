#pragma once

#include <stdbool.h>

#define SIZE 9
#define SUBGRID_SIZE 3

bool isValid(int sudoku[SIZE][SIZE]);
bool backtrack(int sudoku[SIZE][SIZE]);
short isWinnable(int sudoku[SIZE][SIZE]);
int getBoxIndex(int row, int col);
void getMissingBox(int row, int col, bool missing[SIZE], int sudoku[SIZE][SIZE]);
void getMissingCol(int col, bool missing[SIZE], int sudoku[SIZE][SIZE]);
void getMissingRow(int row, bool missing[SIZE], int sudoku[SIZE][SIZE]);
