#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include "FreeImage.h"
#include <omp.h>
//Seminarska naloga VPSA
// gcc openMp.c -Wl,-rpath,./ -L./ -l:"libfreeimage.so.3" -o seminarska -lm -fopenmp -O2
// ./seminarska
#define SIGMA_S 4
#define SIGMA_V 16
#define w SIGMA_S * 3
#define NTHREADS 16

double tabelaGaussBarve[256];
double tabelaGaussRazdalje[w + 1][w + 1];
double *tabelaGaussRazdalje1;


void sprehod(int x, int y, int width, int height, unsigned char *image_in, unsigned char *image_out)
{
    double intR = 0;
    double intG = 0;
    double intB = 0;
    double WR = 0;
    double WG = 0;
    double WB = 0;
    int tmpr = 0;
    int tmps = 0;

    for (int r = y - w; r <= y + w; r++)
    {
        for (int s = x - w; s <= x + w; s++)
        {
            tmpr = r;
            tmps = s;
            
            if (tmpr < 0)
            {
                tmpr = 0;
            }
            if (tmps < 0)
            {
                tmps = 0;
            }
            if (tmpr >= height)
            {
                tmpr = height - 1;
            }
            if (tmps >= width)
            {
                tmps = width - 1;
            }

            int intenzitetaRrs = image_in[4 * tmpr * width + 4 * tmps + 2];
            int intenzitetaGrs = image_in[4 * tmpr * width + 4 * tmps + 1];
            int intenzitetaBrs = image_in[4 * tmpr * width + 4 * tmps + 0];

            int intenzitetaRyx = image_in[4 * y * width + 4 * x + 2];
            int intenzitetaGyx = image_in[4 * y * width + 4 * x + 1];
            int intenzitetaByx = image_in[4 * y * width + 4 * x + 0];

            double gs = tabelaGaussRazdalje[abs(tmps - x)][abs(tmpr - y)];

            double gvR = tabelaGaussBarve[abs(intenzitetaRyx - intenzitetaRrs)];
            double gvG = tabelaGaussBarve[abs(intenzitetaGyx - intenzitetaGrs)];
            double gvB = tabelaGaussBarve[abs(intenzitetaByx - intenzitetaBrs)];

            intR += gs * gvR * intenzitetaRrs;
            intG += gs * gvG * intenzitetaGrs;
            intB += gs * gvB * intenzitetaBrs;

            WR += gs * gvR;
            WG += gs * gvG;
            WB += gs * gvB;
        }
    }

    int nekiR = (int)(round(intR / WR));
    int nekiG = (int)(round(intG / WG));
    int nekiB = (int)(round(intB / WB));

    if (nekiR > 255)
    {
        nekiR = 255;
    }
    if (nekiG > 255)
    {
        nekiG = 255;
    }
    if (nekiB > 255)
    {
        nekiB = 255;
    }

    image_out[4 * y * width + 4 * x + 0] = nekiB;
    image_out[4 * y * width + 4 * x + 1] = nekiG;
    image_out[4 * y * width + 4 * x + 2] = nekiR;
    image_out[4 * y * width + 4 * x + 3] = image_in[4 * y * width + 4 * x + 3];

}

int main()
{
    struct timeval time1, time2;
    gettimeofday(&time1, NULL);
    

    FIBITMAP *imageBitmap = FreeImage_Load(FIF_PNG, "ema4k.png", 0);
    
    FIBITMAP *imageBitmap32 = FreeImage_ConvertTo32Bits(imageBitmap);

    //Dimenzije slike
    int width = FreeImage_GetWidth(imageBitmap32);
    int height = FreeImage_GetHeight(imageBitmap32);
    int pitch = FreeImage_GetPitch(imageBitmap32);

    // Rezervacija pomnilnika
    unsigned char *image_in = (unsigned char *)malloc(height * pitch * sizeof(unsigned char));
    unsigned char *image_out = (unsigned char *)malloc(height * width * sizeof(unsigned char));
    FreeImage_ConvertToRawBits(image_in, imageBitmap32, pitch, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);

    for(int i=0; i<w + 1; i++){
        for(int j=0; j<w + 1; j++){
            tabelaGaussRazdalje[i][j] = exp(-((pow(i, 2) + pow(j, 2)) / (2 * pow(SIGMA_S, 2))));
        }
    }


    for(int x=0; x<w+1; x++){
        for(int y=0; y<w+1; y++){
            int i = x * (w+1) + y;
            // printf("%d %d %d alo\n",x,y,i);
        }
    }

    for(int i=0; i<256; i++){
        tabelaGaussBarve[i] = exp(-((pow(i, 2)) / (2 * pow(SIGMA_V, 2))));
    }
    omp_set_num_threads(NTHREADS);
    #pragma omp parallel for schedule(guided, 1000)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sprehod(j, i, width, height, image_in, image_out);
            // printf("care");
        }
        // printf("care %d \n",i);
    }
    FIBITMAP *out = FreeImage_ConvertFromRawBits(image_out, width, height, pitch, 32, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
    FreeImage_Save(FIF_PNG, out, "output.png", 0);
    
    gettimeofday(&time2, NULL);
    double seconds = (double)(time2.tv_usec - time1.tv_usec) / 1000000 + (double)(time2.tv_sec - time1.tv_sec);
    printf("Time: %.3f Nthreads: %d sigmaS: %d\n",seconds,NTHREADS,SIGMA_S);
    return 0;
}