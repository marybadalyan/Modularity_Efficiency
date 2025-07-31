#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random>

std::vector<int> multiply(std::vector<int> numbers1,std::vector<int> numbers2) {
    std::vector<int> result(numbers1.size(), 0);
    for(int j = 0; j < 100; ++j){
        for (size_t i = 0; i < numbers1.size(); ++i) {
            result[i] = numbers1[i] * numbers2[i];
        }
    }
    return result;
}


std::vector<int> add(std::vector<int> numbers1, std::vector<int> numbers2) {
    std::vector<int> result(numbers1.size(), 0); // Initialize result with the same size as numbers1 and numbers2
    for(int j = 0; j < 100; ++j){
        for (size_t i = 0; i < numbers1.size(); ++i) {
            result[i] = numbers1[i] + numbers2[i];
        }
    }
    
    return result;
}

std::vector<int> fusedMultAdd(std::vector<int> numbers1, std::vector<int> numbers2) {
    std::vector<int> result(numbers1.size(), 0); // Initialize result with the same size as numbers1 and numbers2
    for(int j = 0; j < 100; ++j){
        for (size_t i = 0; i < numbers1.size(); ++i) {
            result[i] = (numbers1[i] + numbers2[i]) * (numbers1[i] + numbers2[i]);
        }
    }
    return result;
}



int main() {
    std::vector<int> numbers1(1000000);
    std::vector<int> numbers2(1000000);

    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dist(1, 100000);

    for (auto& num : numbers1) {
        num = dist(rng);
    }
    for (auto& num : numbers2) {
        num = dist(rng);
    }

    std::chrono::high_resolution_clock::time_point modular_start = std::chrono::high_resolution_clock::now();
    std::vector<int> summed = add(numbers1, numbers2);

    std::vector<int> squared = multiply(summed, summed);

    std::chrono::high_resolution_clock::time_point modular_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = modular_end - modular_start;
    std::cout << "Duration: " << duration.count() << " ms" << std::endl;

    std::chrono::high_resolution_clock::time_point fused_start = std::chrono::high_resolution_clock::now();
    fusedMultAdd(numbers1, numbers2);
    std::chrono::high_resolution_clock::time_point fused_end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> fused_duration = fused_end - fused_start;
    std::cout << "Duration: " << fused_duration.count() << " ms" << std::endl;

    return 0;
}
