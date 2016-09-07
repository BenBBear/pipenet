#ifndef _PIPENET_H_
#define _PIPENET_H_
#include "./helper.h"
#include "./lib/conv.h"

#define IMG_H 28
#define IMG_W 28

typedef  RGB_PIXEL PIXEL;
typedef hls::Mat<IMG_H, IMG_W, RGB_PIXEL_TYPE> IMAGE;

void net(AXI_STREAM &src_axi, AXI_STREAM &dst_axi, int rows, int cols);



//template <unsigned int Ksize, unsigned int InputChannelNum, typename vT>
//void mpool(vT[InputChannelNum] input_pixel, vT[InputChannelNum] output_pixel);

// fc not needed, because it could be implemented by conv.

//template<typename T>
//void f(T s)
//{
//    std::cout << s << '\n';
//}

#endif
