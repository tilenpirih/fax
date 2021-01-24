// kernel
__kernel void sobel(
	__global unsigned char *img_in,
	__global unsigned int *img_R,
	__global unsigned int *img_G,
	__global unsigned int *img_B,
	int width,
	int height)
{
	int gx = get_global_id(0); // 0-width
	int lx = get_local_id(0);  // 1-32

	__local unsigned int bufferR[256];
	__local unsigned int bufferG[256];
	__local unsigned int bufferB[256];

	bufferR[lx]=0;
	bufferG[lx]=0;
	bufferB[lx]=0;
	barrier(CLK_LOCAL_MEM_FENCE);
	
	atomic_add(&bufferR[img_in[gx * 4 + 2]], 1);
	atomic_add(&bufferG[img_in[gx * 4 + 1]], 1);
	atomic_add(&bufferB[img_in[gx * 4]], 1);

	barrier(CLK_LOCAL_MEM_FENCE);

	atomic_add(&img_R[lx], bufferR[lx]);
	atomic_add(&img_G[lx], bufferG[lx]);
	atomic_add(&img_B[lx], bufferB[lx]);

}