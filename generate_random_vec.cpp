#include<iostream>
#include<fstream>
#include<vector>
#include <random>

using namespace std;
int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        cout<<"You should pass file name and vector size as command line args!\n";
        return -1;
    }
    const char* fname = argv[1];
    size_t size = atoi(argv[2]);

    ofstream outfile(fname, std::ios::binary);
    if (!outfile) {
        std::cerr << "Cannot open file for writing!\n";
        return 1;
    }
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distrib(1, 1000000);
    vector<int> vec(size);
    for(auto& val : vec)
    {
        val = distrib(gen);
    }
    outfile.write(reinterpret_cast<const char*> (&size), sizeof(size));
    outfile.write(reinterpret_cast<const char*> (vec.data()), vec.size()*sizeof(int));
    outfile.close();
}   
