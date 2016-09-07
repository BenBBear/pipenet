#ifndef _PIPENET_HELPER_H_
#define _PIPENET_HELPER_H_

#include "hls_video.h"
#include <iostream>

using namespace std;
#define DEBUG cout << "DEBUG: "
#define INFO cout << "INFO: "
#define WARN cout << "WARNING: "
#define ERROR cout << "ERROR: "

typedef ap_axiu<32, 1, 1, 1> STREAM_ITEM;
typedef hls::stream<STREAM_ITEM> AXI_STREAM;
typedef hls::Scalar<3, char> RGB_PIXEL;
typedef hls::Scalar<1, char> GRAY_PIXEL;

#define GRAY_PIXEL_TYPE HLS_8UC1
#define RGB_PIXEL_TYPE HLS_8UC3
#endif
