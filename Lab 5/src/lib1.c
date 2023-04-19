#include <stdio.h>
#include <math.h>


float Derivative(float A, float deltaX) {
    printf("Calculation of derivative function f(x) = cos(x)\n");
    printf("at the point %f with approximation %f\n", A, deltaX);
    printf("by formula f'(x) = (f(A + deltaX) – f(A))/deltaX\n");
    float dfdx = (cos(A + deltaX) - cos(A)) / deltaX;
    return dfdx;
}


float Square(float A, float B) {
    printf("Calculation square of rectangle\n");
    printf("with length: %f, width: %f\n", A, B);
    return A * B;
}
