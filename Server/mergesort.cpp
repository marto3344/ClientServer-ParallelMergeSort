#include "merge-sort.h"
#include<thread>
#include <cmath>

#define THRESHHOLD 10000

void Merge(std::vector<double> &arr, std::vector<double> &copy, int l, int m, int r)
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

void MergeSortRec(std::vector<double> &arr, std::vector<double> &copy, int l, int r)
{
    if (r - l <= 1)
        return;

    int m = (r + l)/2;
    MergeSortRec(arr, copy,l,m);
    MergeSortRec(arr, copy, m, r);
    Merge(arr,copy,l,m,r);
}

void MergeSort(std::vector<double> &arr)
{
    size_t size = arr.size();
    std::vector<double> copy (size);
    MergeSortRec(arr, copy, 0 , size);
}

void ParallelMergeSortRec(std::vector<double> &arr, std::vector<double> &copy, int l, int r, int depth)
{
    if(r - l <= THRESHHOLD || depth  == 0)
    {
        MergeSortRec(arr, copy, l, r);
        return;
    }
    int m = (l + r)/2;
    std::thread leftPart([l, m, depth, &arr, &copy]{ParallelMergeSortRec(arr, copy, l, m, depth - 1);});
    ParallelMergeSortRec(arr,copy,m,r, depth-1);
    leftPart.join();
    Merge(arr,copy,l,m,r);
}

void ParallelMergeSort(std::vector<double>& arr, size_t n_threads)
{
    int depth = std::floor(std::log2((double)n_threads)) + 1;
    size_t size = arr.size();
    
    std::vector<double> copy (size);
    ParallelMergeSortRec(arr, copy, 0 , size, depth);
}
