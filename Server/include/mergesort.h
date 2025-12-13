#pragma once
#include<vector>

void Merge( std::vector<int>& arr, std::vector<int>& copy, int l, int m, int r);

void MergeSortRec(std::vector<int>& arr, std::vector<int>& copy, int l, int r);

void MergeSort(std::vector<int>& arr);

void ParallelMergeSortRec(std::vector<int>& arr, std::vector<int>& copy, int l, int r, int depth);

void ParallelMergeSort(std::vector<int>& arr, size_t n_threads);