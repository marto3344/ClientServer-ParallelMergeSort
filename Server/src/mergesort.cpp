#include<thread>
#include <cmath>
#include "mergesort.h"

#define THRESHHOLD 10000

void Merge(std::vector<int> &arr, std::vector<int> &copy, int l, int m, int r)
{
    int j, k;
    j = l;
    k = m;
    
    for (int i = l; i < r; i++)
    {
        if(k >= r  || (k < r &&  j < m && arr[j] < arr[k]))
        {
            copy[i] = arr[j];
            j++;
        }
        else
        {
            copy[i] = arr[k];
            k++;
        }
    }

    for (size_t i = l; i < r; i++)
    {
        arr[i] = copy[i];
    }
}

void MergeSortRec(std::vector<int> &arr, std::vector<int> &copy, int l, int r)
{
    if (r - l <= 1)
        return;

    int m = (r + l)/2;
    MergeSortRec(arr, copy,l,m);
    MergeSortRec(arr, copy, m, r);
    Merge(arr,copy,l,m,r);
}

void MergeSort(std::vector<int> &arr)
{
    size_t size = arr.size();
    std::vector<int> copy (size);
    MergeSortRec(arr, copy, 0 , size);
}

void ParallelMergeSortRec(std::vector<int> &arr, std::vector<int> &copy, int l, int r, int depth)
{
    if(r - l <= THRESHHOLD || depth  <= 0)
    {
        MergeSortRec(arr, copy, l, r);
        return;
    }
    int m = (l + r)/2;
    std::thread leftPart([&arr, &copy, l, m, depth]() {
        ParallelMergeSortRec(arr, copy, l, m, depth - 1);
    });
    ParallelMergeSortRec(arr,copy,m,r, depth-1);
    if(leftPart.joinable())
        leftPart.join();
    Merge(arr,copy,l,m,r);
}

void ParallelMergeSort(std::vector<int>& arr, size_t n_threads)
{
    if (n_threads == 1)
    {
        MergeSort(arr);
        return;    
    }

    size_t size = arr.size();
    std::vector<int> copy (size);
    int depth = static_cast<int>(std::log2(static_cast<double>(n_threads)));
    ParallelMergeSortRec(arr, copy, 0 , size, depth);
}
