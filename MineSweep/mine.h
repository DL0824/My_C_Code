#pragma once
#ifndef _MINE_H_
#define _MINE_H_
#include<Windows.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#define MINES 20
#define ROW 12
#define COL 12
void Menu();
void Game();
void SetMine(char mine_board[][COL],int row,int col);
void PlayGame(char show_board[][COL], char mine_board[][COL], int row, int col);
int GetMineNum(char mine_board[][COL], int i, int j);
void ShowBoard(char show_board[][COL], int row, int col);
#endif