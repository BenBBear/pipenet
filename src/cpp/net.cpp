#include "../include/pipenet.h"

typedef unsigned char wT;
typedef unsigned char vT;
#define K_1_NUM 2
#define K_1_SIZE 3
#define K_2_NUM 1
#define K_2_SIZE 3

#define CONV_2_H (IMG_H - K_1_SIZE + 1)
#define CONV_2_W (IMG_W - K_1_SIZE + 1)

/*
28->24 This is a very good thing. Just Skip those computation.
                                  Next thing is to identify the skipping time.
                                  Skip inside conv and save the data.
*/

inline bool needToCompute(int row, int col, int H, int W, int K) {
  return (row > K - 1) && (row < H - K) && (col > K - 1) && (col < W - K);
}

wT filters_1[K_1_NUM][K_1_SIZE][K_1_SIZE][1] = {
    1,  2, 1,  0, 1, 0, -1, -2, -1, //
    -1, 0, -1, 0, 2, 0, -1, 0,  -1,
};
wT biases_1[K_1_NUM] = {10, -5};

wT filters_2[K_2_NUM][K_2_SIZE][K_2_SIZE][2] = {1, 0, 1, -1, 1, -1, 0, 1, 0,
                                                1, 0, 1, -1, 1, -1, 0, 1, 0};
wT biases_2[K_2_NUM] = {20};

void net(AXI_STREAM &INPUT_STREAM, AXI_STREAM &OUTPUT_STREAM) {

/****PORT Configuration****/
#pragma HLS INTERFACE axis port = INPUT_STREAM
#pragma HLS INTERFACE axis port = OUTPUT_STREAM

//[ASK] Row and Col and return value, are in the Control BUS
// #pragma HLS RESOURCE core = AXI_SLAVE variable = rows metadata =               \
//     "-bus_bundle CONTROL_BUS"
// #pragma HLS RESOURCE core = AXI_SLAVE variable = cols metadata =               \
//     "-bus_bundle CONTROL_BUS"
#pragma HLS RESOURCE core = AXI_SLAVE variable = return metadata =             \
    "-bus_bundle CONTROL_BUS"
  //[ASK] stable port
  // #pragma HLS INTERFACE ap_stable port = rows
  // #pragma HLS INTERFACE ap_stable port = cols

  vT output_conv_1[K_1_NUM];
  vT output_conv_2[K_2_NUM];

  IMAGE img_input(IMG_H, IMG_W);
  hls::AXIvideo2Mat(INPUT_STREAM, img_input);
  bool compute_1 = false, compute_2 = false;
  int row_c2 = 0, col_c2 = 0;

  for (unsigned int row = 0; row < IMG_H; row++)
    for (unsigned int col = 0; col < IMG_W; col++) {
#pragma HLS PIPELINE
      vT *pixel = img_input.read().val;

      // compute_1 get
      compute_1 = needToCompute(row, col, IMG_H, IMG_W, K_1_SIZE);
      conv<K_1_NUM, K_1_SIZE, 1, IMG_W, vT, wT>(pixel, output_conv_1, filters_1,
                                                biases_1, true, compute_1, col);

      if (compute_1) {
        col_c2 = (col_c2 + 1) % (CONV_2_W);
        if (col_c2 == 0)
          row_c2 = (row_c2 + 1) % (CONV_2_H);
      }
      compute_2 = needToCompute(row_c2, col_c2, CONV_2_H, CONV_2_W, K_2_SIZE);
      // get compute_2
      conv<K_2_NUM, K_2_SIZE, K_1_NUM, CONV_2_W, vT, wT>(
          output_conv_1, output_conv_2, filters_2, biases_2, compute_1,
          compute_2, col_c2);
      // at the end, OUTPUT_STREAM << data
      if (compute_2) {
        STREAM_ITEM item;
        item.last = (row == IMG_H && col == IMG_W);
        vT v = output_conv_2[0];
        item.data = (v << 16) | (v << 8) | v; // not clear
        OUTPUT_STREAM.write(item);
      }
    }
}
