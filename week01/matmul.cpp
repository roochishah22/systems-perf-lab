#include <vector>
#include <iostream>
#include <chrono>

void matmul(const std::vector<float>& A,
            const std::vector<float>& B,
            std::vector<float>& C,
            int N) {
    // three nested loops
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            for (int k = 0; k < N; k++){

                C[i*N + j] += A[i*N + k] * B[k*N + j];
            }   
        }
    }
}


int main() {
    for (int N : {64, 256}) {
        std::vector<float> A(N*N, 1.0);
        std::vector<float> B(N*N, 1.0);
        std::vector<float> C(N*N, 0.0);

        auto start = std::chrono::high_resolution_clock::now();
        matmul(A, B, C, N);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> diff = end - start;
        std::cout << "N=" << N << ": " << diff.count() << " ms\n";

    }
    
    return 0;
    
}