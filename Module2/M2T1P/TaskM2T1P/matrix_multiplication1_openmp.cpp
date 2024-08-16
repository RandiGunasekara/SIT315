#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <omp.h>
#include <chrono> 


using namespace std;

//initializing the matrix
void initialize_matrix(vector<vector<int>>& matrix, int N) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = rand() % 100;  // Random values between 0 and 99
        }
    }
}

//matrix multiplication
void multiply_matrices(const vector<vector<int>>& A, const vector<vector<int>>& B, vector<vector<int>>& C, int N) {
    #pragma omp parallel for  // Parallelize the outer loop
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

//write the result to an output file
void write_matrix_to_file(const vector<vector<int>>& matrix, const string& filename) {
    ofstream file(filename);
    for (const auto& row : matrix) {
        for (const auto& element : row) {
            file << element << " ";
        }
        file << endl;
    }
    file.close();
}

int main() {
    int N = 500;  // Matrix size
    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    vector<vector<int>> C(N, vector<int>(N));

    srand(time(0)); 

    //Initialize matrices A and B with random values
    initialize_matrix(A, N);
    initialize_matrix(B, N);

    // Start time
    auto start = chrono::high_resolution_clock::now();
    // Perform matrix multiplication
    multiply_matrices(A, B, C, N);

    //End timing
    auto end = chrono::high_resolution_clock::now();

    //Calculate execution time
    chrono::duration<double, milli> execution_time = end - start;
    cout << "Execution time: " << execution_time.count() << " milliseconds" << endl;

    // Write the resulting matrix C to a file
    write_matrix_to_file(C, "output_matrix_omp.txt");

    cout << "Its me Randi" << endl;

    return 0;
}
