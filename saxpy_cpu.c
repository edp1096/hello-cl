#include <stdio.h>

#define MAX_N 5

void saxpy(int n, float a, float x[MAX_N], float y[MAX_N]) {
    for (int i = 0; i < n; ++i)
        y[i] = a * x[i] + y[i];
}

int main() {
    const int n = MAX_N;
    float a = 2.0;
    float x[MAX_N] = {1.0, 2.0, 3.0, 4.0, 5.0};
    float y[MAX_N] = {6.0, 7.0, 8.0, 9.0, 10.0};

    saxpy(n, a, x, y);

    for (int i = 0; i < n; ++i)
        printf("%f ", y[i]);

    return 0;
}
