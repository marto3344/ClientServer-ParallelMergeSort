#include<iostream>
#include<vector>

using namespace std;
int main()
{
    int threadsCount, size;
    cout<<"Enter number of threads:\n";
    cin>>threadsCount;
    cout<<"Enter array size:\n";
    cin>>size;
    vector<double> arr(size, 0);
    int a;
    for (size_t i = 0; i < size; i++)
    {
        cin>>a;
        arr[i]= a;
    }
    
}   
