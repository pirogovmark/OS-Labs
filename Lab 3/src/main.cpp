#include <iostream>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>


int const N = 20;
int matrix[20][20], new_matrix[20][20];
int window, frame;


typedef struct threadArguments{
    int numberOfThreads;
    int currentThread;
} threadArguments;


void quickSort(int *array, int low, int high) {
    int i = low;
    int j = high;
    int pivot = array[(i + j) / 2];
    int temp;

    while (i <= j) {
        while (array[i] < pivot) i++;
        while (array[j] > pivot) j--;
        if (i <= j) {
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;
            ++i;
            --j;
        }
    }
    if (j > low) quickSort(array, low, j);
    if (i < high) quickSort(array, i, high);
}


void printAugMatrix(int matrix[N][N]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
}


void printMatrix(int matrix[N][N]) {
    for (int i = frame; i < N - frame - 1; ++i) {
        for (int j = frame; j < N - frame - 1; ++j) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
}


void fillMatrix() {
    srand(time(NULL));
    // Заполнение матрицы внутри
    for (int i = frame; i < N - frame; ++i) {
        for (int j = frame; j < N - frame; ++j) {
            matrix[i][j] = rand() % 9 + 1;
        }
    }
    // Дополнение матрицы для крайних элементов
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 0; j < frame; ++j) {
            matrix[i][j] = matrix[i][frame];
            matrix[i][N - j - 1] = matrix[i][N - frame - 1];
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < frame; ++j) {
            matrix[j][i] = matrix[frame][i];
            matrix[N - j - 1][i] = matrix[N - frame - 1][i];  
        }
    }
}


void resultToMatrix() {
    for (int i = frame; i < N - frame; ++i) {
        for (int j = frame; j < N - frame; ++j) {
            matrix[i][j] = new_matrix[i][j];
        }
    }
    for (int i = 1; i < N - 1; ++i) {
        for (int j = 0; j < frame; ++j) {
            matrix[i][j] = matrix[i][frame];
            matrix[i][N - j - 1] = matrix[i][N - frame - 1];
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < frame; ++j) {
            matrix[j][i] = matrix[frame][i];
            matrix[N - j - 1][i] = matrix[N - frame - 1][i];  
        }
    }
}


int medianForElem(int i, int j) {
    int frame = window / 2;
    int sortSize = window * window - 1;
    int numbers[sortSize];
    int counter = 0;

    for (int l = i - frame; l <= i + frame; ++l) {
        for (int k = j - frame; k <= j + frame; ++k) {
            numbers[counter] = matrix[l][k];
            ++counter;
        }
    }
    quickSort(numbers, 0, sortSize - 1);

    return numbers[sortSize / 2];
}


void filterString(int stringNumber) {
    for (int j = frame; j < N - frame; ++j) {
        new_matrix[stringNumber][j] = medianForElem(stringNumber, j);
    }
}


void* threadFilter(void* arg) {
    threadArguments data = *((threadArguments*) arg);
    for (int i = data.currentThread; i < N; i += data.numberOfThreads) {
        filterString(i);
    }
}


int main() {
    int numberOfThreads;
    std::cout << "Enter the number of threads: ";
    std::cin >> numberOfThreads;

    std::cout << "Enter window size: ";
    std::cin >> window;
    frame = window / 2;

    int overlays;
    std::cout << "Enter the number of overlays: ";
    std::cin >> overlays;

    fillMatrix();
    printMatrix(matrix);
    std::cout << '\n';

    double start = clock();

    for (int k = 0; k < overlays; ++k) {
        pthread_t threads[numberOfThreads];
        threadArguments* data = (threadArguments*)malloc(sizeof(threadArguments) * numberOfThreads);
        for (int i = 0; i < numberOfThreads; ++i) {
            data[i].currentThread = i;
            data[i].numberOfThreads = numberOfThreads;
        }
        for (int i = 0; i < numberOfThreads; ++i) {
            if (pthread_create(&threads[i], NULL, &threadFilter, &data[i]) != 0) {
                std::cout << "Failed to create thread\n";
                return 1;
            }
        }
        for (int i = 0; i < numberOfThreads; ++i) {
            if (pthread_join(threads[i], NULL) != 0) {
                std::cout << "Failed to join thread\n";
                return 1;
            }
        }
        resultToMatrix();
    }
    
    std::cout << "\nNew matrix:\n";
    printMatrix(matrix);
    
    std::cout << "\nThe program ran for " << (clock() - start) / (CLOCKS_PER_SEC) << " seconds\n";

    return 0;
}
