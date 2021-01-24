__kernel void neki(__global unsigned char *image_in, __global unsigned char *image_out, int width, int height, int sigma_s, int sigma_v, __global double *tabB, __global double *tabR)
{
    int gid = get_global_id(0);
    int lid = get_local_id(0);

    int x = gid % width;
    int y = gid / width;

    int w = sigma_s * 3;


    if( gid < height * width ){

        //Sprehod
        
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

                //double gs = gauss(tmpr - y, tmps - x, sigma_s);

                
                int i = (tmps - x) * (w + 1) + (tmpr - y);
                double gs = tabR[i];
                

                double gvR = tabB[abs(intenzitetaRyx - intenzitetaRrs)];
                double gvG = tabB[abs(intenzitetaGyx - intenzitetaGrs)];
                double gvB = tabB[abs(intenzitetaByx - intenzitetaBrs)];

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
        
}