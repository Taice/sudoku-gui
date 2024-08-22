#pragma once

#include <stdbool.h>

#define SIZE 9
#define SUBGRID_SIZE 3

bool isValid(char sudoku[SIZE][SIZE]);
bool whereIsValid(char sudoku[SIZE][SIZE], int *row, int *col);
int backtrack(char sudoku[SIZE][SIZE], bool *foundSolution);
short isWinnable(char sudoku[SIZE][SIZE]);
int getBoxIndex(int row, int col);
void getMissingBox(int row, int col, bool missing[SIZE],
                   char sudoku[SIZE][SIZE]);
void getMissingCol(int col, bool missing[SIZE], char sudoku[SIZE][SIZE]);
void getMissingRow(int row, bool missing[SIZE], char sudoku[SIZE][SIZE]);
void calcCheats(bool cheats[SIZE][SIZE][SIZE], char sudoku[SIZE][SIZE]);
void cacheNotes(bool notes[SIZE][SIZE][SIZE], char *notefile);
void cacheSudoku(char sudoku[SIZE][SIZE], char *notefile);
void getNotesFromCache(bool notes[SIZE][SIZE][SIZE], char *filename);
int getSudokuFromCache(char sudoku[SIZE][SIZE], char *filename);
