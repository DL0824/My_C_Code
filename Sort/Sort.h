#ifndef _SORT_H_
#define _SORT_H_

#include<stdio.h>
#include<stdlib.h>
#define INSERT_SORT_LENTH 8

void MergeSort(int *arr, int n);
void Qsort(int *arr, int n);//前后指针法
void Qsort_w(int *arr, int n);//挖坑法
void HQsort(int *arr, int n);// hoare法
void InsertSort(int *arr, int n);//直接插排
void QuikSortNonR(int* a, int left, int right);//快排的非递归实现
#endif/*_SORT_H_*/