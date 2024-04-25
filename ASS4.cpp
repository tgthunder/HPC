#include<iostream>
#include<cuda_runtime.h> 
#include<cstdlib>

#define BLOCK_SIZE 16

using namespace std;

void initialize_matrix(int *array, int rows, int cols){
    for(int i = 0 ; i < rows; i++){
        for(int j = 0; j < cols; j++){
            array[i*cols + j] = rand() % 10;
        }
    }
}

void print_matrix(int *array, int rows, int cols){
    for(int i = 0 ; i < rows; i++){
        for(int j = 0; j < cols; j++){
            cout << array[i*cols + j] << " ";
        }
        cout << endl;
    }
}

void matrix_multiplication_cpu(int *a, int *b, int *c, int common, int c_rows, int c_cols){
    for(int i = 0; i < c_rows; i++){
        for(int j = 0; j < c_cols; j++){
            int sum = 0;
            for(int k = 0; k < common; k++){
                sum += a[i*common + k] * b[k*c_cols + j];
            }
            c[i*c_cols + j] = sum;
        }
    }
}

__global__ void matrix_multiply(int *a, int *b, int *c, int c_rows, int common, int c_cols)
{
    int row = blockIdx.y*blockDim.y + threadIdx.y;
    int col = blockIdx.x*blockDim.x + threadIdx.x;
    int sum = 0;

    if(col < c_cols && row < c_rows) {
        for(int k = 0; k < common; k++) {
            sum += a[row*common + k] * b[k*c_cols + col];
        }
        c[c_cols*row + col] = sum;
    }
}

int main() {
    int a_rows = 4, a_cols = 4, b_rows = 4, b_cols = 4;
    int c_rows = a_rows, c_cols = b_cols;
    int *a, *b, *c, *d_a, *d_b, *d_c;
    int size_a = a_rows * a_cols * sizeof(int);
    int size_b = b_rows * b_cols * sizeof(int);
    int size_c = c_rows * c_cols * sizeof(int);

    // Allocate memory on host
    a = (int*)malloc(size_a);
    b = (int*)malloc(size_b);
    c = (int*)malloc(size_c);

    // Initialize matrices
    initialize_matrix(a, a_rows, a_cols);
    initialize_matrix(b, b_rows, b_cols);

    // Allocate memory on device
    cudaMalloc(&d_a, size_a);
    cudaMalloc(&d_b, size_b);
    cudaMalloc(&d_c, size_c);

    // Copy data from host to device
    cudaMemcpy(d_a, a, size_a, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, b, size_b, cudaMemcpyHostToDevice);

    // Define grid and block dimensions
    dim3 blockDim(BLOCK_SIZE, BLOCK_SIZE);
    dim3 gridDim((c_cols + BLOCK_SIZE - 1) / BLOCK_SIZE, (c_rows + BLOCK_SIZE - 1) / BLOCK_SIZE);

    // Launch kernel
    matrix_multiply<<<gridDim, blockDim>>>(d_a, d_b, d_c, c_rows, a_cols, c_cols);

    // Copy result back to host
    cudaMemcpy(c, d_c, size_c, cudaMemcpyDeviceToHost);

    // Print result
    print_matrix(c, c_rows, c_cols);

    // Free device memory
    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);

    // Free host memory
    free(a);
    free(b);
    free(c);

    return 0;
}
