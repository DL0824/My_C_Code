#pragma once
#ifndef _CHESS_H_
#define _CHESS_H_
#include<stdio.h>
#include<Windows.h>
#include<time.h>
#include<string.h>
#define ROW 3
#define COL 3
void menu();
void Game();
void Computer(char board[][COL],int row,int col);
void Player(char board[][COL],int row,int col);
void Show(char board[][COL],int row,int col);
char Judge(char board[][COL], int row, int col);

#endif 
