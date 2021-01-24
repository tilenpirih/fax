// kernel
__kernel void sobel(
	__global unsigned char *img_in,
	__global unsigned char *img_out,
	int width,
	int height
	)
{
	int gx = get_global_id(0); // 0-width
	int gy = get_global_id(1); // 0-height
	int gtid = gx + gy * width;
	int lx = get_local_id(0) + 1; // 1-32
	int ly = get_local_id(1) + 1; // 1-32

	__local unsigned char buffer[32 + 2][32 + 2];
	buffer[lx][ly] = img_in[gtid];
	if (lx == 1)
	{
		buffer[0][ly] = img_in[gtid - 1];
	}
	else if (lx == 32)
	{
		buffer[33][ly] = img_in[gtid + 1];
	}
	if (ly == 1)
	{
		buffer[lx][0] = img_in[gtid - width];
		if (lx == 32)
			buffer[33][0] = img_in[gtid - width + 1];
		else if (lx == 1)
			buffer[0][0] = img_in[gtid - width - 1];
	}
	else if (ly == 32)
	{
		buffer[lx][33] = img_in[gtid + width];
		if (lx == 32)
			buffer[33][33] = img_in[gtid + width + 1];
		else if (lx == 1)
			buffer[0][33] = img_in[gtid + width - 1];
	}

	barrier(CLK_LOCAL_MEM_FENCE);

	int Gx = -buffer[lx - 1][ly - 1] - 2 * buffer[lx - 1][ly] -
			 buffer[lx - 1][ly + 1] + buffer[lx + 1][ly - 1] +
			 2 * buffer[lx + 1][ly] + buffer[lx + 1][ly + 1];

	int Gy = -buffer[lx - 1][ly - 1] - 2 * buffer[lx][ly - 1] -
			 buffer[lx + 1][ly - 1] + buffer[lx - 1][ly + 1] +
			 2 * buffer[lx][ly + 1] + buffer[lx + 1][ly + 1];

	int tempPixel = sqrt((float)(Gx * Gx + Gy * Gy));
	if (tempPixel > 255)
		img_out[gtid] = 255;
	else
		img_out[gtid] = tempPixel;

	/*
		 Gx = -getPixel(image_in, i - 1, j - 1, width, height) - 2 * getPixel(image_in, i - 1, j, width, height) -
                 getPixel(image_in, i - 1, j + 1, width, height) + getPixel(image_in, i + 1, j - 1, width, height) +
                 2 * getPixel(image_in, i + 1, j, width, height) + getPixel(image_in, i + 1, j + 1, width, height);

				 
            Gy = -getPixel(image_in, i - 1, j - 1, width, height) - 2 * getPixel(image_in, i, j - 1, width, height) -
                 getPixel(image_in, i + 1, j - 1, width, height) + getPixel(image_in, i - 1, j + 1, width, height) +
                 2 * getPixel(image_in, i, j + 1, width, height) + getPixel(image_in, i + 1, j + 1, width, height);
*/
	// img_out[gtid] = img_in[gtid];
}