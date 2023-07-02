#ifndef _CAMERA_V2_H_
#define _CAMERA_V2_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// 导入bool
#include <stdbool.h>

#include "zf_common_headfile.h"
#include "zf_device_tft180.h"
#include "define.h"


// 定义图片的宽度和高度
#define WIDTH MT9V03X_W
#define HEIGHT MT9V03X_H

// 定义边缘检测的阈值
#define THRESHOLD 200

// 定义角点检测的窗口大小
#define WINDOW_SIZE 5

// 定义角点检测的阈值
#define CORNER_THRESHOLD 2.0

// 定义道路中央与图片中央的差值的放大系数
#define SCALE_FACTOR 10

// 定义边缘检测后的像素值
#define EDGE 255
#define NON_EDGE 0

// 定义角点检测后的像素值
#define CORNER 111


// 定义图片的二维数组
extern uint8 camera_image[HEIGHT][WIDTH];

// 定义边缘检测后的二维数组
extern uint8 edge[HEIGHT][WIDTH];

// 定义道路边缘检测后的二维数组
extern uint8 road_edge[HEIGHT][WIDTH];

void copy_image();
void edge_detection();
void corner_detection();
void road_edge_detection();
int16 calculate_deviation();
int16 process_image();

#endif // _CAMERA_V2_H_
