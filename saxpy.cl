__kernel void saxpy(__global float* a, __global float* x, __global float* y) {
    int i = get_global_id(0);
    y[i] = *a * x[i] + y[i];
}
