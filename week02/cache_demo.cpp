#include <vector>
#include <iostream>
#include <chrono>

float sequential(std::vector<float>& data, int N) {
    float sum = 0;
    for (int i = 0; i < N; i++) {
        sum += data[i];
    }
    return sum;
}


float strided(std::vector<float>& data, int N) {
    float sum = 0;
    for (int i = 0; i < N; i += 16) {
        sum += data[i];
    }
    return sum;
}

int main(){
    std::vector<float> data;
    int size = 10000000;
    data = std::vector<float>(size, 1.0);

    auto seq_start = std::chrono::high_resolution_clock::now();
    float sequential_sum = sequential(data, size); 
    auto seq_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> seq_diff = seq_end - seq_start;
    std::cout << "sequential sum duration: " << seq_diff.count() << "\n";
    
    auto stri_start = std::chrono::high_resolution_clock::now();
    float strided_sum = strided(data,size);
    auto stri_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> stri_diff = stri_end - stri_start;
    std::cout << "strided sum duration: " << stri_diff.count() << "\n";
   

    return 0;

}