#include <vector>
#include <iostream>

struct Matrix {
    // two member variables: N and data
    int N;
    std::vector<float> data;
    
    // constructor: takes N, initializes data with N*N floats
    Matrix(int N) {
        this-> N = N;
        data = std::vector<float>(N*N, 0.0);
    }
    
    // at() method: returns data[i*N + j]
    float& at(int i, int j) {
        return data[i*N +j];
    }
};

void matmul_naive(Matrix& A, Matrix& B, Matrix& C) {
    // your three nested loops here
    // use A.at(i,k) and B.at(k,j) and C.at(i,j)
    int N = A.N;

    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            for (int k = 0; k < N; k++){

                C.at(i,j) += A.at(i,k) * B.at(k,j);
            }   
        }
    }
}

void matmul_reordered(Matrix& A, Matrix& B, Matrix& C) {
    int N = A.N;
    for (int i = 0; i < N; i++)
        for (int k = 0; k < N; k++)
            for (int j = 0; j < N; j++)
                C.at(i,j) += A.at(i,k) * B.at(k,j);
}

int main() {
    for (int N : {256, 512}) {
        Matrix A(N), B(N), C(N), C2(N);

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                A.at(i,j) = 1.0;
                B.at(i,j) = 1.0;
            }

        auto start = std::chrono::high_resolution_clock::now();
        matmul_naive(A, B, C);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diff = end - start;
        std::cout << "N=" << N << " naive:     " << diff.count() << " ms\n";

        auto start2 = std::chrono::high_resolution_clock::now();
        matmul_reordered(A, B, C2);
        auto end2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diff2 = end2 - start2;
        std::cout << "N=" << N << " reordered: " << diff2.count() << " ms\n\n";
    }
    return 0;
}