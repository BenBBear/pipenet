template <unsigned int Knum, unsigned int Ksize, unsigned int InputChannelNum,
          unsigned int InputWidth, typename V, typename W>
void conv(V input_pixel[InputChannelNum], V output_pixel[Knum],
          W filters[Knum][Ksize][Ksize][InputChannelNum], W biases[Knum],
          bool save, bool compute, unsigned int colId) {
  // input_pixel, output_pixel, filters, biases
  static V line_buffer[Ksize][InputWidth][InputChannelNum];
  static V window_buffer[Ksize][Ksize][InputChannelNum];
  int offset = int(Ksize / 2);
  if (!save)
    return;

  // SLIDE_WINDOW
  for (int c = 0; c < InputChannelNum; c++) {
    for (int k = 0; k < Ksize; k++) {
      for (int r = 0; r < Ksize - 1; r++)
        window_buffer[k][r][c] = window_buffer[k][r + 1][c];
      if (k < Ksize - 1)
        line_buffer[k][colId][c] = line_buffer[k + 1][colId][c];
    }
    line_buffer[Ksize - 1][colId][c] = input_pixel[c];
  }

  for (int c = 0; c < InputChannelNum; c++) {
    for (int k = 0; k < Ksize; k++)
      window_buffer[k][Ksize - 1][c] = line_buffer[k][colId][c];
  }

  if (!compute)
    return;

  /**** Do Convolution Here ****/
  for (int c = 0; c < InputChannelNum; c++)
    for (int fn = 0; fn < Knum; fn++) {
      for (int i = -offset; i < offset + 1; i++)
        for (int j = -offset; j < offset + 1; j++)
          output_pixel[fn] +=
              input_pixel[c] * filters[fn][i + offset][j + offset][c];
      output_pixel[fn] += biases[fn];
    }
}
