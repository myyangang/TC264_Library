//-------------------------------------------------------------------------------------------------------------------
//  简介:八邻域图像处理
//  闲鱼ID：玛卡巴卡的杂货铺
//------------------------------------------------------------------------------------------------------------------

#include "zf_common_headfile.h"

#define image_h 120
#define image_w 128

int my_abs(int value) {
  if (value >= 0)
    return value;
  else
    return -value;
}

int16_t limit_a_b(int16_t x, int a, int b) {
  if (x < a) x = a;
  if (x > b) x = b;
  return x;
}

int16_t limit1(int16_t x, int16_t y) {
  if (x > y)
    return y;
  else if (x < -y)
    return -y;
  else
    return x;
}

uint8_t original_image[image_h][image_w];
uint8_t image_thereshold;  // 图像分割阈值

void Get_image(uint8_t (*mt9v03x_image)[image_w]) {
#define use_num 1
  uint8_t i = 0, j = 0, row = 0, line = 0;
  for (i = 0; i < image_h; i += use_num)  //
  {
    for (j = 0; j < image_w; j += use_num)  //
    {
      original_image[row][line] = mt9v03x_image[i][j];
      line++;
    }
    line = 0;
    row++;
  }
}

uint8_t otsuThreshold(uint8_t *image, uint16_t col, uint16_t row) {
#define GrayScale 256
  uint16_t Image_Width = col;
  uint16_t Image_Height = row;
  int X;
  uint16_t Y;
  uint8_t *data = image;
  int HistGram[GrayScale] = {0};

  uint32_t Amount = 0;
  uint32_t PixelBack = 0;
  uint32_t PixelIntegralBack = 0;
  uint32_t PixelIntegral = 0;
  int32_t PixelIntegralFore = 0;
  int32_t PixelFore = 0;
  double OmegaBack = 0, OmegaFore = 0, MicroBack = 0, MicroFore = 0, SigmaB = 0,
         Sigma = 0;  // 类间方差;
  uint8_t MinValue = 0, MaxValue = 0;
  uint8_t Threshold = 0;

  for (Y = 0; Y < Image_Height;
       Y++)  // Y<Image_Height改为Y =Image_Height；以便进行 行二值化
  {
    // Y=Image_Height;
    for (X = 0; X < Image_Width; X++) {
      HistGram[(int)data[Y * Image_Width + X]]++;  // 统计每个灰度值的个数信息
    }
  }
  for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++)
    ;  // 获取最小灰度的值
  for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0;
       MaxValue--)
    ;  // 获取最大灰度的值

  if (MaxValue == MinValue) {
    return MaxValue;  // 图像中只有一个颜色
  }
  if (MinValue + 1 == MaxValue) {
    return MinValue;  // 图像中只有二个颜色
  }

  for (Y = MinValue; Y <= MaxValue; Y++) {
    Amount += HistGram[Y];  //  像素总数
  }

  PixelIntegral = 0;
  for (Y = MinValue; Y <= MaxValue; Y++) {
    PixelIntegral += HistGram[Y] * Y;  // 灰度值总数
  }
  SigmaB = -1;
  for (Y = MinValue; Y < MaxValue; Y++) {
    PixelBack = PixelBack + HistGram[Y];     // 前景像素点数
    PixelFore = Amount - PixelBack;          // 背景像素点数
    OmegaBack = (double)PixelBack / Amount;  // 前景像素百分比
    OmegaFore = (double)PixelFore / Amount;  // 背景像素百分比
    PixelIntegralBack += HistGram[Y] * Y;    // 前景灰度值
    PixelIntegralFore = PixelIntegral - PixelIntegralBack;  // 背景灰度值
    MicroBack = (double)PixelIntegralBack / PixelBack;  // 前景灰度百分比
    MicroFore = (double)PixelIntegralFore / PixelFore;  // 背景灰度百分比
    Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) *
            (MicroBack - MicroFore);  // g
    if (Sigma > SigmaB)               // 遍历最大的类间方差g
    {
      SigmaB = Sigma;
      Threshold = (uint8_t)Y;
    }
  }
  return Threshold;
}

uint8_t bin_image[image_h][image_w];  // 图像数组
void turn_to_bin(void) {
  uint8_t i, j;
  image_thereshold = otsuThreshold(original_image[0], image_w, image_h);
  for (i = 0; i < image_h; i++) {
    for (j = 0; j < image_w; j++) {
      if (original_image[i][j] > image_thereshold)
        bin_image[i][j] = 0;
      else
        bin_image[i][j] = 255;
    }
  }
}

uint8_t start_point_l[2] = {0};  // 左边起点的x，y值
uint8_t start_point_r[2] = {0};  // 右边起点的x，y值
uint8_t get_start_point(uint8_t start_row) {
  uint8_t i = 0, l_found = 0, r_found = 0;
  // 清零
  start_point_l[0] = 0;  // x
  start_point_l[1] = 0;  // y

  start_point_r[0] = 0;  // x
  start_point_r[1] = 0;  // y

  // 从中间往左边，先找起点
  for (i = image_w / 2; i > 0; i--) {
    start_point_l[0] = i;          // x
    start_point_l[1] = start_row;  // y
    if (bin_image[start_row][i] == 255 && bin_image[start_row][i - 1] == 0) {
      // printf("找到左边起点image[%d][%d]\n", start_row,i);
      l_found = 1;
      break;
    }
  }

  for (i = image_w / 2; i < border_max; i++) {
    start_point_r[0] = i;          // x
    start_point_r[1] = start_row;  // y
    if (bin_image[start_row][i] == 255 && bin_image[start_row][i + 1] == 0) {
      // printf("找到右边起点image[%d][%d]\n",start_row, i);
      r_found = 1;
      break;
    }
  }

  if (l_found && r_found)
    return 1;
  else {
    // printf("未找到起点\n");
    return 0;
  }
}

/*
函数名称：void search_l_r(uint16_t break_flag, uint8_t(*image)[image_w],uint16_t
*l_stastic, uint16_t *r_stastic, uint8_t l_start_x, uint8_t l_start_y, uint8_t
r_start_x, uint8_t r_start_y,uint8_t*hightest)

功能说明：八邻域正式开始找右边点的函数，输入参数有点多，调用的时候不要漏了，这个是左右线一次性找完。
参数说明：
break_flag_r			：最多需要循环的次数
(*image)[image_w]：需要进行找点的图像数组，必须是二值图,填入数组名称即可
特别注意，不要拿宏定义名字作为输入参数，否则数据可能无法传递过来
*l_stastic：统计左边数据，用来输入初始数组成员的序号和取出循环次数
*r_stastic：统计右边数据，用来输入初始数组成员的序号和取出循环次数
l_start_x：左边起点横坐标
l_start_y				：左边起点纵坐标
r_start_x				：右边起点横坐标
r_start_y				：右边起点纵坐标
hightest				：循环结束所得到的最高高度
函数返回：无
修改时间：2022年9月25日
备    注：
example：
 */
#define USE_num \
  300  // 定义找点的数组成员个数按理说300个点能放下，但是有些特殊情况确实难顶，多定义了一点

// 存放点的x，y坐标
uint16_t points_l[(uint16_t)USE_num][2] = {{0}};
uint16_t points_r[(uint16_t)USE_num][2] = {{0}};
uint16_t dir_r[(uint16_t)USE_num] = {0};
uint16_t dir_l[(uint16_t)USE_num] = {0};
uint16_t data_stastics_l = 0;
uint16_t data_stastics_r = 0;
uint8_t hightest = 0;

void search_l_r(uint16_t break_flag, uint8_t (*image)[image_w],
                uint16_t *l_stastic, uint16_t *r_stastic, uint8_t l_start_x,
                uint8_t l_start_y, uint8_t r_start_x, uint8_t r_start_y,
                uint8_t *hightest) {
  
  uint8_t i = 0, j = 0;
  uint8_t search_filds_l[8][2] = {{0}};
  uint8_t index_l = 0;
  uint8_t temp_l[8][2] = {{0}};
  uint8_t center_point_l[2] = {0};
  uint16_t l_data_statics;

  static int8_t seeds_l[8][2] = {
      // 从-90°顺时针旋转
      {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1},
  };
  uint8_t search_filds_r[8][2] = {{0}};
  uint8_t center_point_r[2] = {0};
  uint8_t index_r = 0;
  uint8_t temp_r[8][2] = {{0}};
  uint16_t r_data_statics;
  static int8_t seeds_r[8][2] = {
      // 从+90°逆时针旋转
      {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}, {-1, 0}, {-1, 1},
  };
  l_data_statics = *l_stastic;
  r_data_statics = *r_stastic;

  // center_point_l = {l_start_x, l_start_y}
  // center_point_r = {r_start_x, r_start_y}
  center_point_l[0] = l_start_x;  // x
  center_point_l[1] = l_start_y;  // y
  center_point_r[0] = r_start_x;  // x
  center_point_r[1] = r_start_y;  // y

  // 最大循环break_flag次
  while (break_flag--) {
    // 获取center_point_l点周围八个点的坐标
    for (i = 0; i < 8; i++) {
      search_filds_l[i][0] = center_point_l[0] + seeds_l[i][0];  // x
      search_filds_l[i][1] = center_point_l[1] + seeds_l[i][1];  // y
    }

    // points_l[l_data_statics] = {左边起点坐标}
    // l_data_statics += 1
    points_l[l_data_statics][0] = center_point_l[0];  // x
    points_l[l_data_statics][1] = center_point_l[1];  // y
    l_data_statics++;

    // 获取center_point_周围八个点的坐标
    for (i = 0; i < 8; i++) {
      search_filds_r[i][0] = center_point_r[0] + seeds_r[i][0];  // x
      search_filds_r[i][1] = center_point_r[1] + seeds_r[i][1];  // y
    }

    // points_r[r_data_statics] = {右边起点坐标}
    points_r[r_data_statics][0] = center_point_r[0];  // x
    points_r[r_data_statics][1] = center_point_r[1];  // y

    // teml_l[8][2] = {0}
    for (i = 0; i < 8; i++) {
      temp_l[i][0] = 0;
      temp_l[i][1] = 0;
    }

    // 绕周围一圈，找到最先从黑突变为白的区域，此时令黑色序号为i，令temp_l[当前序号index_l]={黑点坐标}，当前序号++
    index_l = 0;
    for (i = 0; i < 8; i++) {
      if (image[search_filds_l[i][1]][search_filds_l[i][0]] == 0 &&
          image[search_filds_l[(i + 1) & 7][1]]
               [search_filds_l[(i + 1) & 7][0]] == 255) {
        temp_l[index_l][0] = search_filds_l[(i)][0];
        temp_l[index_l][1] = search_filds_l[(i)][1];
        index_l++;
        dir_l[l_data_statics - 1] =
            (i);  // TODO:dir_l的作用是什么？l_data_statics的作用是什么？
      }

      // 如果找到了突变点,使得center_point_l = argmin_{j∈[0,index_l)}
      // {temp_l[j].y}
      if (index_l) {
        center_point_l[0] = temp_l[0][0];  // x
        center_point_l[1] = temp_l[0][1];  // y
        for (j = 0; j < index_l; j++) {
          if (center_point_l[1] > temp_l[j][1]) {
            center_point_l[0] = temp_l[j][0];  // x
            center_point_l[1] = temp_l[j][1];  // y
          }
        }
      }
    }

    if ((points_r[r_data_statics][0] == points_r[r_data_statics - 1][0] &&
         points_r[r_data_statics][0] == points_r[r_data_statics - 2][0] &&
         points_r[r_data_statics][1] == points_r[r_data_statics - 1][1] &&
         points_r[r_data_statics][1] == points_r[r_data_statics - 2][1]) ||
        (points_l[l_data_statics - 1][0] == points_l[l_data_statics - 2][0] &&
         points_l[l_data_statics - 1][0] == points_l[l_data_statics - 3][0] &&
         points_l[l_data_statics - 1][1] == points_l[l_data_statics - 2][1] &&
         points_l[l_data_statics - 1][1] == points_l[l_data_statics - 3][1])) {
      break;
    }
    if (my_abs(points_r[r_data_statics][0] - points_l[l_data_statics - 1][0]) <
            2 &&
        my_abs(points_r[r_data_statics][1] - points_l[l_data_statics - 1][1] <
               2)) {
      *hightest =
          (points_r[r_data_statics][1] + points_l[l_data_statics - 1][1]) >> 1;
      break;
    }
    if ((points_r[r_data_statics][1] < points_l[l_data_statics - 1][1])) {
      continue;
    }
    if (dir_l[l_data_statics - 1] == 7 &&
        (points_r[r_data_statics][1] > points_l[l_data_statics - 1][1])) {
      center_point_l[0] = points_l[l_data_statics - 1][0];  // x
      center_point_l[1] = points_l[l_data_statics - 1][1];  // y
      l_data_statics--;
    }
    r_data_statics++;

    index_r = 0;
    for (i = 0; i < 8; i++) {
      temp_r[i][0] = 0;
      temp_r[i][1] = 0;
    }
    for (i = 0; i < 8; i++) {
      if (image[search_filds_r[i][1]][search_filds_r[i][0]] == 0 &&
          image[search_filds_r[(i + 1) & 7][1]]
               [search_filds_r[(i + 1) & 7][0]] == 255) {
        temp_r[index_r][0] = search_filds_r[(i)][0];
        temp_r[index_r][1] = search_filds_r[(i)][1];
        index_r++;
        dir_r[r_data_statics - 1] = (i);
        // printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics -
        // 1]);
      }
      if (index_r) {
        center_point_r[0] = temp_r[0][0];  // x
        center_point_r[1] = temp_r[0][1];  // y
        for (j = 0; j < index_r; j++) {
          if (center_point_r[1] > temp_r[j][1]) {
            center_point_r[0] = temp_r[j][0];  // x
            center_point_r[1] = temp_r[j][1];  // y
          }
        }
      }
    }
  }
  *l_stastic = l_data_statics;
  *r_stastic = r_data_statics;
}

/*
函数名称：void image_draw_rectan(uint8_t(*image)[image_w])
功能说明：给图像画一个黑框
参数说明：uint8_t(*image)[image_w]	图像首地址
函数返回：无
修改时间：2022年9月8日
备    注：
example： image_draw_rectan(bin_image);
 */
void image_draw_rectan(uint8t (*image)[image_w]) {
  uint8_t i = 0;
  for (i = 0; i < image_h; i++) {
    image[i][0] = 0;
    image[i][1] = 0;
    image[i][image_w - 1] = 0;
    image[i][image_w - 2] = 0;
  }
  for (i = 0; i < image_w; i++) {
    image[0][i] = 0;
    image[1][i] = 0;
    // image[image_h-1][i] = 0;
  }
}

/*
函数名称：void image_process(void)
功能说明：最终处理函数
参数说明：无
函数返回：无
修改时间：2022年9月8日
备    注：
example： image_process();
 */
void image_process(void) {
  uint16_t i;
  uint8_t hightest = 0;
  /*这是离线调试用的*/
  Get_image(mt9v03x_image);
  turn_to_bin();
  /*提取赛道边界*/
  image_draw_rectan(bin_image);
  // 清零
  data_stastics_l = 0;
  data_stastics_r = 0;
  if (get_start_point(image_h - 2)) {
    search_l_r((uint16_t)USE_num, bin_image, &data_stastics_l, &data_stastics_r,
               start_point_l[0], start_point_l[1], start_point_r[0],
               start_point_r[1], &hightest);
  }

  for (i = 0; i < data_stastics_l; i++) {
    ips154_drawpoint(points_l[i][0] + 2, points_l[i][1],
                     uesr_BLUE);  // 显示起点
  }
  for (i = 0; i < data_stastics_r; i++) {
    ips154_drawpoint(points_r[i][0] - 2, points_r[i][1], uesr_RED);  // 显示起点
  }
}
