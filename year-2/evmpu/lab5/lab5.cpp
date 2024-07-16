#include <iostream>
#include <opencv2/highgui.hpp>
#include <time.h>

using namespace cv;

const double alpha = 0.5; /* Simple contrast control, [1.0-3.0] */
const int beta = 50; /* Simple brightness control, [0-100] */

int main(int argc, char **argv)
{
  int frameCounter = 0;
  double totalRead = 0;
  double totalProcess = 0;
  double totalWrite = 0;

  time_t tick = 1; // counts how many seconds last from the last call
  int totalFPS = 0;

  // struct timespec tmpReadTime, tmpProcessTime, tmpWriteTime;

  struct timespec timeBegin;
  clock_gettime(CLOCK_MONOTONIC_RAW, &timeBegin);

  VideoCapture cap(0); // the default camera
  if (!cap.isOpened()) return 0;

  while (1)
  {
    Mat image;
    struct timespec start, end;
    // reading
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    cap.read(image);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    totalRead += end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec);

    if(image.empty()) break;

    /*
      create an image of the same size and type with zero pixel values
    */
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    Mat newImage = Mat::zeros(image.size(), image.type());
    for( int y = 0; y < image.rows; y++ )
    {
        for( int x = 0; x < image.cols; x++ )
        {
            for( int c = 0; c < image.channels(); c++ )
            {
                newImage.at<Vec3b>(y,x)[c] = saturate_cast<uchar>
                (
                  alpha*image.at<Vec3b>(y,x)[c] + beta
                );
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    totalProcess += end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec);

    // imshow("Original Image", image);

    // writing
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    imshow("New Image", newImage);
    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    totalWrite += end.tv_sec-start.tv_sec + 0.000000001*(end.tv_nsec-start.tv_nsec);

    // FPS counter
    frameCounter++;

    clock_gettime(CLOCK_MONOTONIC_RAW, &start);
    int timeNow = start.tv_sec-timeBegin.tv_sec;

    if (timeNow - tick >= 1)
    {
      tick++;
      totalFPS += frameCounter;
      std::cout << "fps: " << frameCounter << std::endl;
      frameCounter = 0;
    }

    char c = waitKey(1);
    if (c == 9) break; // tab
  }

  struct timespec tmp;
  clock_gettime(CLOCK_MONOTONIC_RAW, &tmp);

  double totalTimeClock = tmp.tv_sec-timeBegin.tv_sec + 0.000000001*(tmp.tv_nsec-timeBegin.tv_nsec);
  std::cout << "Average fps: " << totalFPS / (tick-1) << std::endl;
  std::cout << "Time for reading: " << totalRead * 100 / totalTimeClock << "%" << std::endl;
  std::cout << "Time for processing: " << totalProcess * 100 / totalTimeClock << "%" << std::endl;
  std::cout << "Time for writing: " << totalWrite * 100 / totalTimeClock << "%" << std::endl;

  return 0;
}
