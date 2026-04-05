#include <vector>
#include <iostream>
#include <chrono>
#include <functional>

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

void matmul_inlined(Matrix& A, Matrix& B, Matrix& C) {
    int N = A.N;
    for (int i = 0; i < N; i++)
        for (int k = 0; k < N; k++)
            for (int j = 0; j < N; j++)
                C.data[i*N+j] += A.data[i*N+k] * B.data[k*N+j];
}


void matmul_tiled(Matrix& A, Matrix& B, Matrix& C, int tile_size) {
    int N = A.N;
    for (int i = 0; i < N; i += tile_size)
        for (int k = 0; k < N; k += tile_size)
            for (int j = 0; j < N; j += tile_size)
                for (int ii = i; ii < i + tile_size; ii++) {
                    int row_a = ii * N;
                    int row_c = ii * N;
                    for (int kk = k; kk < k + tile_size; kk++) {
                        float a_val = A.data[row_a + kk];
                        int row_b = kk * N;
                        for (int jj = j; jj < j + tile_size; jj++)
                            C.data[row_c + jj] += a_val * B.data[row_b + jj];
                    }
                }
}

void benchmark(const std::string& name, int N, 
               std::function<void()> fn) {
    auto start = std::chrono::high_resolution_clock::now();
    fn();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = end - start;
    std::cout << "N=" << N << " " << name << ": " << diff.count() << " ms\n";
}

int main() {
    for (int N : {512, 1024, 2048}) {
        Matrix A(N), B(N), C(N), C2(N), C3(N), C4(N), C5(N);;

        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++) {
                A.at(i,j) = 1.0;
                B.at(i,j) = 1.0;
            }

        benchmark("naive",     N, [&]{ matmul_naive(A, B, C); });
        benchmark("reordered", N, [&]{ matmul_reordered(A, B, C2); });
        benchmark("inlined",   N, [&]{ matmul_inlined(A, B, C3); });
        benchmark("tiled_16",  N, [&]{ matmul_tiled(A, B, C4, 16); });
        benchmark("tiled_32",  N, [&]{ matmul_tiled(A, B, C5, 32); });
    }
    return 0;
}