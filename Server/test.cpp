#include<iostream>
#include <vector>
#include <random>
#include<fstream>
#include "merge-sort.h"

int main(){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 100000.0);
    std::vector<double> vec(10000000);
    for (auto& val : vec) {
        val = dis(gen);
    }
    //MergeSort(vec);
    ParallelMergeSort(vec,4);
    std::cout<<"Sorting finished! Now writing results!";

    /*std::ofstream file("numbers.txt");
    if (file.is_open()) {
        for (const auto& val : vec) {
            file << val << "\n";
        }
        file.close();
        std::cout << "Записано успешно!\n";
    } else {
        std::cerr << "Грешка при отваряне на файла!\n";
    }*/
    return 0;
    
}