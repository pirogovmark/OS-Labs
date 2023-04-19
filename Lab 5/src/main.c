#include <stdio.h>
#include <dlfcn.h>
#include "lib.h"


void printMenu(){
    printf("\nEnter command:\n");
    printf("\n0 - to change methods of calculation\n");
    printf("\n1 - to compute the derived function f(x) = cos(x) with arguments point and delta\n");
    printf("\n2 - to calculate the area of flat figure with arguments length and width\n");
    printf("\n3 - to end program\n");
}


const char* lib1 = "./liblib1.so";
const char* lib2 = "./liblib2.so";


int main(int argc, char const *argv[]) {
    float A = 0, deltaX = 0, B = 0;
    printMenu();
    int command = 0, link = 0, flag = 1;

    void *currentLib = dlopen(lib1, RTLD_LAZY);
    printf("\nCurrent method: %d\n\n", link + 1);
    float (*Derivative)(float A, float deltaX);
    float (*Square)(float A, float B);
    Derivative = dlsym(currentLib, "Derivative");
    Square = dlsym(currentLib, "Square");

    while (flag) {
        scanf("%d", &command);
        switch (command) {
            case 0:
                dlclose(currentLib);
                if (link == 0) {
                    currentLib = dlopen(lib2, RTLD_LAZY);
                } else {
                    currentLib = dlopen(lib1, RTLD_LAZY);
                }
                link = !link;
                Derivative = dlsym(currentLib, "Derivative");
                Square = dlsym(currentLib, "Square");
                break;
        
            case 1:
                scanf("%f%f", &A, &deltaX);
                printf("\n--------------------------------------------------\n");
                printf("Answer: %f\n", Derivative(A, deltaX));
                printf("\n--------------------------------------------------\n");
                break;
        
            case 2:
                scanf("%f%f", &A, &B);
                printf("---------------------------------\n");
                printf("Answer: %f\n", Square(A, B));
                printf("---------------------------------\n");
                break;

            case 3:
                flag = 0;
                break;

            default:
                printf("Wrong command\n");
                break;
        }

        if (flag == 1) {
            printMenu();
            printf("\nCurrent method: %d\n\n", link + 1);
        } else {
            printf("Program completed!\n");
        }
    }
    
    return 0;
}
