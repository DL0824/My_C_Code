#ifndef _SORT_H_
#define _SORT_H_

#include<stdio.h>
#include<stdlib.h>
#define INSERT_SORT_LENTH 8

void MergeSort(int *arr, int n);
void Qsort(int *arr, int n);//ǰ��ָ�뷨
void Qsort_w(int *arr, int n);//�ڿӷ�
void HQsort(int *arr, int n);// hoare��
void InsertSort(int *arr, int n);//ֱ�Ӳ���
void QuikSortNonR(int* a, int left, int right);//���ŵķǵݹ�ʵ��
#endif/*_SORT_H_*/