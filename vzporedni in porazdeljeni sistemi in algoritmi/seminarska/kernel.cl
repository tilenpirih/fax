// kernel
#define SIGMA_S 4
#define SIGMA_V 16
#define w SIGMA_S * 3
__kernel void bilateral(
	__global unsigned char *image_in,
	__global unsigned char *image_out,
	int width,
	int height)
{
	int gx = get_global_id(0); // 0-width
	// int lx = get_local_id(0); 1-32

	if (gx < width * height)
	{
		int x = gx % width;
		int y = gx / width;

		// printf("%d %d \n", x, y);

		image_out[4 * y * width + 4 * x + 0] = image_in[4 * y * width + 4 * x + 0];
		image_out[4 * y * width + 4 * x + 1] = image_in[4 * y * width + 4 * x + 1];
		image_out[4 * y * width + 4 * x + 2] = image_in[4 * y * width + 4 * x + 2];
		image_out[4 * y * width + 4 * x + 3] = image_in[4 * y * width + 4 * x + 3];

		if (gx < 100)
		{
			//printf("%d \n",image_out[gx*4+0]);
			printf("gx: %d, x: %d y: %d R: %d \n",gx,x,y,image_out[gx*4+0]);

		}
	}
}
/*
double gauss(int x, int y)
{
    double s = SIGMA_S;
    double rezultat = exp(-((pow(x, 2) + pow(y, 2)) / (2 * pow(s, 2))));

    return rezultat;
}

double gauss2(int razlika)
{
    double v = SIGMA_V;
    double rezultat = exp(-((pow(razlika, 2)) / (2 * pow(v, 2))));

    return rezultat;
}*/