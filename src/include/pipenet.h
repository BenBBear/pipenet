#ifndef _PIPENET_H_
#define _PIPENET_H_
#include "./helper.h"

#define IMG_H 28
#define IMG_W 28

typedef PIXEL RGB_PIXEL;
typedef hls::Mat<IMG_H, IMG_W, RGB_PIXEL_TYPE> IMAGE;

void net(AXI_STREAM &src_axi, AXI_STREAM &dst_axi, int rows, int cols);

template <unsigned int Ksize, unsigned int Knum, unsigned int InputChannelNum,
          typename vT, typename wT>
void conv(vT[InputChannelNum] input_pixel, vT[Knum] output_pixel,
          wT[Knum][Ksize][Ksize] filters, wT[Knum] biases);

template <unsigned int Ksize, unsigned int InputChannelNum, typename vT>
void mpool(vT[InputChannelNum] input_pixel, vT[InputChannelNum] output_pixel);

// fc not needed, because it could be implemented by conv.

#endif
