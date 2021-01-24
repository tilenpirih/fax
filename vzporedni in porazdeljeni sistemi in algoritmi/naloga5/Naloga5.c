#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "FreeImage.h"
#include <CL/cl.h>

#define WORKGROUP_SIZE	(512)
#define MAX_SOURCE_SIZE	16384

int main(void) 
{

    //Velikost slike
	int width = 640;
    int height = 480;
	int pitch = ((32 * width + 31) / 32) * 4;

	char ch;
    int i;
	cl_int ret;

    // Branje datoteke
    FILE *fp;
    char *source_str;
    size_t source_size;

    fp = fopen("kernel_mandelbrot.cl", "r");
    if (!fp) 
	{
		fprintf(stderr, ":-(#\n");
        exit(1);
    }
    source_str = (char*)malloc(MAX_SOURCE_SIZE);
    source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	source_str[source_size] = '\0';
    fclose( fp );

	// Rezervacija pomnilnika
    unsigned char *image = (unsigned char *)malloc(height * width * sizeof(unsigned char) * 4);
 
    // Podatki o platformi
    cl_platform_id	platform_id[10];
    cl_uint			ret_num_platforms;
	char			*buf;
	size_t			buf_len;
	ret = clGetPlatformIDs(10, platform_id, &ret_num_platforms);
			// max. "stevilo platform, kazalec na platforme, dejansko "stevilo platform
	
	// Podatki o napravi
	cl_device_id	device_id[10];
	cl_uint			ret_num_devices;
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
    size_t local_item_size = WORKGROUP_SIZE;
	size_t num_groups = (((height * width) -1)/local_item_size+1);		
    size_t global_item_size = num_groups*local_item_size;		

    // Priprava programa
    cl_program program = clCreateProgramWithSource(context,	1, (const char **)&source_str,  
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
	build_log =(char *)malloc(sizeof(char)*(build_log_len+1));
	ret = clGetProgramBuildInfo(program, device_id[0], CL_PROGRAM_BUILD_LOG, 
							    build_log_len, build_log, NULL);
	printf("%s\n", build_log);
	free(build_log);

    // "s"cepec: priprava objekta
    cl_kernel kernel = clCreateKernel(program, "izracunaj_piksle", &ret);
			// program, ime "s"cepca, napaka

	// Alokacija pomnilnika na napravi
    cl_mem image_mem_obj = clCreateBuffer(context, CL_MEM_WRITE_ONLY, height * width * sizeof(unsigned char) * 4, NULL, &ret);

    // "s"cepec: argumenti
    ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&image_mem_obj);
    ret |= clSetKernelArg(kernel, 1, sizeof(cl_int), (void *)&height);
    ret |= clSetKernelArg(kernel, 2, sizeof(cl_int), (void *)&width);
			// "s"cepec, "stevilka argumenta, velikost podatkov, kazalec na podatke

	// "s"cepec: zagon
    ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,						
								 &global_item_size, &local_item_size, 0, NULL, NULL);	
			// vrsta, "s"cepec, dimenzionalnost, mora biti NULL, 
			// kazalec na "stevilo vseh niti, kazalec na lokalno "stevilo niti, 
			// dogodki, ki se morajo zgoditi pred klicem
																						
    // Kopiranje rezultatov
    ret = clEnqueueReadBuffer(command_queue, image_mem_obj, CL_TRUE, 0,						
							  height*width*sizeof(int), image, 0, NULL, NULL);				
			// branje v pomnilnik iz naparave, 0 = offset
			// zadnji trije - dogodki, ki se morajo zgoditi prej

    //Rezultati
    FIBITMAP *dst = FreeImage_ConvertFromRawBits(image, width, height, pitch,
		32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
	FreeImage_Save(FIF_PNG, dst, "mandelbrot.png", 0);

 
    // "ci"s"cenje
    ret = clFlush(command_queue);
    ret = clFinish(command_queue);
    ret = clReleaseKernel(kernel);
    ret = clReleaseProgram(program);
    ret = clReleaseMemObject(image_mem_obj);
    ret = clReleaseCommandQueue(command_queue);
    ret = clReleaseContext(context);
	
	
    free(image);

    return 0;
}

/*

Resolucija 640x480,   čas: 4.47s
Resolucija 800x600,   čas: 4.514s
Resolucija 1600x900,  čas: 4.582s
Resolucija 1920x1080, čas: 4.684s
Resolucija 3840x2160, čas: 4.919s

*/