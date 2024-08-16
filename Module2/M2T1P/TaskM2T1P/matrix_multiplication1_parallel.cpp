#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <pthread.h>
#include <chrono> 

using namespace std;

//Structure to hold data for each thread
struct ThreadData {
    const vector<vector<int>>* A;
    const vector<vector<int>>* B;
    vector<vector<int>>* C;
    int start_row;
    int end_row;
    int N;
};

// initializing the matrix
void initialize_matrix(vector<vector<int>> &matrix, int N)
{
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            matrix[i][j] = rand() % 100; // Random values between 0 and 99
        }
    }
}

//matrix multiplication
void* multiply_matrices(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    const vector<vector<int>>& A = *(data->A);
    const vector<vector<int>>& B = *(data->B);
    vector<vector<int>>& C = *(data->C);
    int N = data->N;

    for (int i = data->start_row; i < data->end_row; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    return nullptr;
}

int main() {
    int N = 500;  //Matrix size 
    int num_threads = 4;  //Number of threads
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<int>> C(N, vector<int>(N));

    srand(time(0));

    // Initialize matrices A and B
    initialize_matrix(A, N);
    initialize_matrix(B, N);
    
    //array to hold thread identifiers
    pthread_t threads[num_threads];

    //array to hold thread data
    ThreadData thread_data[num_threads];

    //Calculate the number of rows each thread should process
    int rows_per_thread = N / num_threads;

    // Start time
    auto start = chrono::high_resolution_clock::now();

    //Create threads
    for (int t = 0; t < num_threads; ++t) {
        thread_data[t] = {&A, &B, &C, t * rows_per_thread, (t == num_threads - 1) ? N : (t + 1) * rows_per_thread, N};
        pthread_create(&threads[t], nullptr, multiply_matrices, (void*)&thread_data[t]);
    }

    // Join threads
    for (int t = 0; t < num_threads; ++t) {
        pthread_join(threads[t], nullptr);
    }

    //End timing
    auto end = chrono::high_resolution_clock::now();

    //Calculate execution time
    chrono::duration<double, milli> execution_time = end - start;
    cout << "Execution time: " << execution_time.count() << " milliseconds" << endl;

    //Write the resulting matrix C to a file
    ofstream file("output_matrix_parallel.txt");
    for (const auto& row : C) {
        for (const auto& element : row) {
            file << element << " ";
        }
        file << endl;
    }
    file.close();

    return 0;
}
