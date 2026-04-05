#include <vector>
#include <iostream> 
#include <chrono>



struct Tensor {
    // your member variables go here
    int size;
    std::vector<float> data;
    
    // your constructor goes here
    Tensor(int size) {
        // set things up
        data = std::vector<float>(size, 0.0);
        this->size = size;
    }
    
    // your operator[] goes here
    float& operator[](int i) {
        // return something
        return data[i];
    }
};

Tensor add(Tensor a, Tensor b) {

    if (a.size != b.size) {
    std::cerr << "Error: tensor sizes don't match\n";
    return Tensor(0);
    }
    
    Tensor result(a.size);

    for (int i = 0; i < a.size; i++){
        result[i] = a[i] + b[i];
    }

    return result;
} 

double time_ms(int size) {
    Tensor a(size);
    Tensor b(size);

    for (int i = 0; i < size; i++) {
        a[i] = 1.0;
        b[i] = 2.0;
    }

    auto start = std::chrono::high_resolution_clock::now();
    Tensor c = add(a, b);
    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> diff = end - start;
    return diff.count();
}

int main() {
    std::cout << "size 1K:  " << time_ms(1000)      << " ms\n";
    std::cout << "size 1M:  " << time_ms(1000000)   << " ms\n";
    std::cout << "size 10M: " << time_ms(10000000)  << " ms\n";
    return 0;
}