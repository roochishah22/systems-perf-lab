#include <vector>
#include <iostream>
#include <chrono>
#include <functional>
#include <thread>
#include <algorithm> 
#include <numeric> 

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


void worker(Matrix& A, Matrix& B, Matrix& C, int start, int end) {
    // reordered matmul but only for rows start to end
    // instead of i going 0 to N, i goes start to end
    int N = A.N;
    for (int i = start; i < end; i++)
        for (int k = 0; k < N; k++)
            for (int j = 0; j < N; j++)
                C.at(i,j) += A.at(i,k) * B.at(k,j);
}

void matmul_parallel(Matrix& A, Matrix& B, Matrix& C, int num_threads) {
    int N = A.N;
    int chunk = N / num_threads;
    
    std::vector<std::thread> threads;
    
    for (int t = 0; t < num_threads; t++) {
        int start = t * chunk;
        int end = (t == num_threads - 1) ? N : start + chunk;
        
        threads.emplace_back(worker, std::ref(A), std::ref(B), std::ref(C), start, end);
    }
    
    // join all threads
    for (auto& t : threads) {
        t.join();
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

void print_stats(const std::string& name, int N, std::vector<double> times) {
    // sort for median
    std::sort(times.begin(), times.end());
    // compute min, max, mean, median
    double min = times.front();
    double max = times.back();
    double mean = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
    double median = (times.size() % 2 == 0) ? (times[times.size()/2 - 1] + times[times.size()/2]) / 2 : times[times.size()/2];
    // print all four
    std::cout << "N=" << N << " " << name << ": " << min << " ms (min), " << max << " ms (max), " << mean << " ms (mean), " << median << " ms (median)\n";
}

std::vector<double> run_trials(std::function<void()> fn, 
                                int warmup, 
                                int trials) {
    // warmup runs — discard these
    for (int i = 0; i < warmup; i++) fn();
    
    // real trials
    std::vector<double> times;
    for (int i = 0; i < trials; i++) {
        auto start = std::chrono::high_resolution_clock::now();
        fn();
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diff = end - start;
        times.push_back(diff.count());
    }
    return times;
}

int main() {
    int N = 1024;
    Matrix A(N), B(N);
    
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            A.at(i,j) = 1.0;
            B.at(i,j) = 1.0;
        }

    auto run = [&](const std::string& name, auto fn) {
        Matrix C(N);
        auto times = run_trials([&]{ fn(C); }, 3, 20);
        print_stats(name, N, times);
    };

    run("naive",      [&](Matrix& C){ matmul_naive(A, B, C); });
    run("reordered",  [&](Matrix& C){ matmul_reordered(A, B, C); });
    run("inlined",    [&](Matrix& C){ matmul_inlined(A, B, C); });
    run("1 thread",   [&](Matrix& C){ matmul_parallel(A, B, C, 1); });
    run("2 threads",  [&](Matrix& C){ matmul_parallel(A, B, C, 2); });
    run("4 threads",  [&](Matrix& C){ matmul_parallel(A, B, C, 4); });
    run("8 threads",  [&](Matrix& C){ matmul_parallel(A, B, C, 8); });

    return 0;
}