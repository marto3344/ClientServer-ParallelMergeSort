#include "merge-sort.h"

void Merge(std::vector<double> &arr, std::vector<double> &copy, int l, int m, int r)
{
    int j, k;
    j = l;
    k = m;
    
    for (int i = l; i < r; i++)
    {
        if(k >= r  || (k < r &&  l < m && arr[j] < arr[k]))
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
    std::vector<double> copy (size, 0);
    MergeSortRec(arr, copy, 0 , size);
}
