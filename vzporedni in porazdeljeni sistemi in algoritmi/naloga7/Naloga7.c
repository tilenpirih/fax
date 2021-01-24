#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "FreeImage.h"
#include <CL/cl.h>

struct histogram {
	unsigned int *R;
	unsigned int *G;
	unsigned int *B;
};


#define WORKGROUP_SIZE 256
#define MAX_SOURCE_SIZE 16384

// gcc Naloga7.c -O2 -lm -lOpenCL -Wl,-rpath,./ -L./ -l:"libfreeimage.so.3" -o Naloga7
// srun -n1 -G1 --reservation=fri Naloga7


void printHistogram(histogram H) {
	printf("Colour\tNo. Pixels\n");
	for (int i = 0; i < BINS; i++) {
		if (H.B[i]>0)
			printf("%dB\t%d\n", i, H.B[i]);
		if (H.G[i]>0)
			printf("%dG\t%d\n", i, H.G[i]);
		if (H.R[i]>0)
			printf("%dR\t%d\n", i, H.R[i]);
	}
}

int main(void)
{

	//Velikost slike

	//Load image from file
	FIBITMAP *imageBitmap = FreeImage_Load(FIF_PNG, "input.png", 0);
	//Convert it to a 32-bit image
    FIBITMAP *imageBitmap32 = FreeImage_ConvertTo32Bits(imageBitmap);
	
    //Get image dimensions
    int width = FreeImage_GetWidth(imageBitmap32);
	int height = FreeImage_GetHeight(imageBitmap32);
	int pitch = FreeImage_GetPitch(imageBitmap32);
	//Preapare room for a raw data copy of the image
    unsigned char *imageIn = (unsigned char *)malloc(height*pitch * sizeof(unsigned char));
	
    //Initalize the histogram
    histogram H;
	H.B = (unsigned int*)calloc(256, sizeof(unsigned int));
	H.G = (unsigned int*)calloc(256, sizeof(unsigned int));
	H.R = (unsigned int*)calloc(256, sizeof(unsigned int));
	
    //Extract raw data from the image
	FreeImage_ConvertToRawBits(imageIn, imageBitmap32, pitch, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

    //Free source image data
	FreeImage_Unload(imageBitmap32);
	FreeImage_Unload(imageBitmap);

	char ch;
	int i;
	cl_int ret;

	// Branje datoteke
	FILE *fp;
	char *source_str;
	size_t source_size;

	fp = fopen("kernel.cl", "r");
	if (!fp)
	{
		fprintf(stderr, ":-(#\n");
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	source_str[source_size] = '\0';

	fclose(fp);

	// Podatki o platformi
	cl_platform_id platform_id[10];
	cl_uint ret_num_platforms;
	char *buf;
	size_t buf_len;
	ret = clGetPlatformIDs(10, platform_id, &ret_num_platforms);
	// max. "stevilo platform, kazalec na platforme, dejansko "stevilo platform

	// Podatki o napravi
	cl_device_id device_id[10];
	cl_uint ret_num_devices;
	// Delali bomo s platform_id[0] na GPU
	ret = clGetDeviceIDs(platform_id[0], CL_DEVICE_TYPE_GPU, 10,
						 device_id, &ret_num_devices);
	// izbrana platforma, tip naprave, koliko naprav nas zanima
	// kazalec na naprave, dejansko "stevilo naprav

	// Kontekst
	cl_context context = clCreateContext(NULL, 1, &device_id[0], NULL, NULL, &ret);
	// kontekst: vklju"cene platforme - NULL je privzeta, "stevilo naprav,
	// kazalci na naprave, kazalec na call-back funkcijo v primeru napake
	// dodatni parametri funkcije, "stevilka napake

	// Ukazna vrsta
	cl_command_queue command_queue = clCreateCommandQueue(context, device_id[0], 0, &ret);
	// kontekst, naprava, INORDER/OUTOFORDER, napake

	// Delitev dela
	size_t local_item_size =  WORKGROUP_SIZE ; // 32x32
	size_t num_groups = (((width * height) - 1) / local_item_size + 1);
	size_t global_item_size = num_groups * local_item_size; 

	// Priprava programa
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
												   NULL, &ret);
	// kontekst, "stevilo kazalcev na kodo, kazalci na kodo,
	// stringi so NULL terminated, napaka

	// Prevajanje
	ret = clBuildProgram(program, 1, &device_id[0], NULL, NULL, NULL);
	// program, "stevilo naprav, lista naprav, opcije pri prevajanju,
	// kazalec na funkcijo, uporabni"ski argumenti

	// Log
	size_t build_log_len;
	char *build_log;
	ret = clGetProgramBuildInfo(program, device_id[0], CL_PROGRAM_BUILD_LOG,
								0, NULL, &build_log_len);
	// program, "naprava, tip izpisa,
	// maksimalna dol"zina niza, kazalec na niz, dejanska dol"zina niza
	build_log = (char *)malloc(sizeof(char) * (build_log_len + 1));
	ret = clGetProgramBuildInfo(program, device_id[0], CL_PROGRAM_BUILD_LOG,
								build_log_len, build_log, NULL);
	printf("%s\n", build_log);
	free(build_log);

	// "s"cepec: priprava objekta
	cl_kernel kernel = clCreateKernel(program, "sobel", &ret);
	// program, ime "s"cepca, napaka

	// Alokacija pomnilnika na napravi
	// CL_MEM_READ_ONLY = 01
	// CL_MEM_COPY_HOST_PTR = 10
	// 01 | 10 = 11
	cl_mem image_mem_obj_in = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, height * width * sizeof(unsigned char)*4, imageIn, &ret);
	cl_mem image_mem_obj_R = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 256 * sizeof(unsigned int), NULL, &ret);
	cl_mem image_mem_obj_G = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 256 * sizeof(unsigned int), NULL, &ret);
	cl_mem image_mem_obj_B = clCreateBuffer(context, CL_MEM_WRITE_ONLY, 256 * sizeof(unsigned int), NULL, &ret);

	// "s"cepec: argumenti
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&image_mem_obj_in);
	ret |= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&image_mem_obj_R);
	ret |= clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&image_mem_obj_G);
	ret |= clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&image_mem_obj_B);
	ret |= clSetKernelArg(kernel, 4, sizeof(cl_int), (void *)&width);
	ret |= clSetKernelArg(kernel, 5, sizeof(cl_int), (void *)&height);
	
	// "s"cepec, "stevilka argumenta, velikost podatkov, kazalec na podatke

	// "s"cepec: zagon
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
								 &global_item_size, &local_item_size, 0, NULL, NULL);
	// vrsta, "s"cepec, dimenzionalnost, mora biti NULL,
	// kazalec na "stevilo vseh niti, kazalec na lokalno "stevilo niti,
	// dogodki, ki se morajo zgoditi pred klicem

	// Kopiranje rezultatov
	ret = clEnqueueReadBuffer(command_queue, image_mem_obj_R, CL_TRUE, 0, 256 * sizeof(unsigned int), H.R, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, image_mem_obj_G, CL_TRUE, 0, 256 * sizeof(unsigned int), H.G, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, image_mem_obj_B, CL_TRUE, 0, 256 * sizeof(unsigned int), H.B, 0, NULL, NULL);
	// branje v pomnilnik iz naparave, 0 = offset
	// zadnji trije - dogodki, ki se morajo zgoditi prej

	//Rezultati
	

	// "ci"s"cenje
	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(image_mem_obj_in);
	ret = clReleaseMemObject(image_mem_obj_R);
	ret = clReleaseMemObject(image_mem_obj_G);
	ret = clReleaseMemObject(image_mem_obj_B);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	free(imageIn);
	printHistogram(H);
	return 0;
}

/*

Resolucija 640x480,   čas: 4.47s
Resolucija 800x600,   čas: 4.514s
Resolucija 1600x900,  čas: 4.582s
Resolucija 1920x1080, čas: 4.684s
Resolucija 3840x2160, čas: 4.919s

*/