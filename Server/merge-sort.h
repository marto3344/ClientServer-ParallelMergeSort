#pragma once
#include<vector>

void Merge( std::vector<double>& arr, std::vector<double>& copy, int l, int m, int r);

void MergeSortRec(std::vector<double>& arr, std::vector<double>& copy, int l, int r);

void MergeSort(std::vector<double>& arr);

void ParallelMergeSortRec(std::vector<double>& arr, std::vector<double>& copy, int l, int r, int depth);

void ParallelMergeSort(std::vector<double>& arr, size_t n_threads);