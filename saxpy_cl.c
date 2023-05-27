#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include <clblast_c.h>

#define MAX_N 5

int main() {
    const int n = MAX_N;
    float a = 2.0;
    float x[MAX_N] = {1.0, 2.0, 3.0, 4.0, 5.0};
    float y[MAX_N] = {6.0, 7.0, 8.0, 9.0, 10.0};

    // Get platform and device information
    cl_platform_id platform_id;
    cl_device_id device_id;
    cl_uint ret_num_devices, ret_num_platforms;
    cl_int ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);

    // Create an OpenCL context
    cl_context context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);

    // Create a command queue
    cl_command_queue command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

    // Create memory buffers on the device for the input and output
    cl_mem memobj_a = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(float), NULL, &ret);
    cl_mem memobj_x = clCreateBuffer(context, CL_MEM_READ_ONLY, n * sizeof(float), NULL, &ret);
    cl_mem memobj_y = clCreateBuffer(context, CL_MEM_READ_WRITE, n * sizeof(float), NULL, &ret);

    // Copy the input data to the memory buffers
    ret = clEnqueueWriteBuffer(command_queue, memobj_a, CL_TRUE, 0, sizeof(float), &a, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, memobj_x, CL_TRUE, 0, n * sizeof(float), x, 0, NULL, NULL);
    ret = clEnqueueWriteBuffer(command_queue, memobj_y, CL_TRUE, 0, n * sizeof(float), y, 0, NULL, NULL);

    // Load the kernel source code from a .cl file
    FILE* file = fopen("saxpy.cl", "r");
    if (!file) {
        printf("Failed to open kernel file.\n");
        return 1;
    }
    fseek(file, 0, SEEK_END);
    size_t program_size = ftell(file);
    rewind(file);
    char* source_str = (char*)malloc(program_size + 1);
    fread(source_str, sizeof(char), program_size, file);
    fclose(file);
    source_str[program_size] = '\0';

    // Create a program from the kernel source
    cl_program program = clCreateProgramWithSource(context, 1, (const char**)&source_str, (const size_t*)&program_size, &ret);

    // Build the program
    ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);

    // Create the OpenCL kernel
    cl_kernel kernel = clCreateKernel(program, "saxpy", &ret);

    // Set the arguments of the kernel
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&memobj_a);
    ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&memobj_x);
    ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&memobj_y);

    // Execute the OpenCL kernel
    size_t global_item_size = n;
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, NULL, 0, NULL, NULL);

    // Read the output data from the memory buffer
    ret = clEnqueueReadBuffer(command_queue, memobj_y, CL_TRUE, 0, n * sizeof(float), y, 0, NULL, NULL);

    // Print the result
    for (int i = 0; i < n; ++i)
        printf("%f ", y[i]);

    // Clean up
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(memobj_a);
    ret = clReleaseMemObject(memobj_x);
    ret = clReleaseMemObject(memobj_y);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);

    free(source_str);

    return 0;
}
