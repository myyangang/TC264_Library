#include "camera.h"
int my_abs(int value) {
	if(value>=0) return value;
	else return -value;
}


/*变量声明*/
uint8 original_image[image_h][image_w];
uint8 image_thereshold;//图像分割阈值
//------------------------------------------------------------------------------------------------------------------
//  @brief      获得一副灰度图像
//  @since      v1.0 
//------------------------------------------------------------------------------------------------------------------
void copyImage(uint8(*mt9v03x_image)[image_w]){
	#define use_num		1	//1就是不压缩，2就是压缩一倍	
	uint8 i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < image_h; i += use_num){
        for (j = 0; j < image_w; j += use_num){
            original_image[row][line] = mt9v03x_image[i][j];//这里的参数填写你的摄像头采集到的图像
			line++;
        }
        line = 0;
        row++;
    }
}
//------------------------------------------------------------------------------------------------------------------
//  @brief     动态阈值
//  @since      v1.0 
//------------------------------------------------------------------------------------------------------------------
uint8 otsuThreshold(uint8 *image, uint16 col, uint16 row){
	#define GrayScale 256
    uint16 Image_Width  = col;
    uint16 Image_Height = row;
    int X; uint16 Y;
    uint8* data = image;
    int HistGram[GrayScale] = {0};
	
	uint32 Amount = 0;
    uint32 PixelBack = 0;
    uint32 PixelIntegralBack = 0;
    uint32 PixelIntegral = 0;
    int32 PixelIntegralFore = 0;
    int32 PixelFore = 0;
    double OmegaBack=0, OmegaFore=0, MicroBack=0, MicroFore=0, SigmaB=0, Sigma=0; // 类间方差;
    uint8 MinValue=0, MaxValue=0;
    uint8 Threshold = 0;
	
	
    for (Y = 0; Y <Image_Height; Y++) //Y<Image_Height改为Y =Image_Height；以便进行 行二值化
    {
        //Y=Image_Height;
        for (X = 0; X < Image_Width; X++)
        {
        HistGram[(int)data[Y*Image_Width + X]]++; //统计每个灰度值的个数信息
        }
    }




    for (MinValue = 0; MinValue < 256 && HistGram[MinValue] == 0; MinValue++) ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistGram[MinValue] == 0; MaxValue--) ; //获取最大灰度的值

    if (MaxValue == MinValue)
    {
        return MaxValue;          // 图像中只有一个颜色
    }
    if (MinValue + 1 == MaxValue)
    {
        return MinValue;      // 图像中只有二个颜色
    }

    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        Amount += HistGram[Y];        //  像素总数
    }

    PixelIntegral = 0;
    for (Y = MinValue; Y <= MaxValue; Y++)
    {
        PixelIntegral += HistGram[Y] * Y;//灰度值总数
    }
    SigmaB = -1;
    for (Y = MinValue; Y < MaxValue; Y++)
    {
          PixelBack = PixelBack + HistGram[Y];    //前景像素点数
          PixelFore = Amount - PixelBack;         //背景像素点数
          OmegaBack = (double)PixelBack / Amount;//前景像素百分比
          OmegaFore = (double)PixelFore / Amount;//背景像素百分比
          PixelIntegralBack += HistGram[Y] * Y;  //前景灰度值
          PixelIntegralFore = PixelIntegral - PixelIntegralBack;//背景灰度值
          MicroBack = (double)PixelIntegralBack / PixelBack;//前景灰度百分比
          MicroFore = (double)PixelIntegralFore / PixelFore;//背景灰度百分比
          Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore) * (MicroBack - MicroFore);//g
          if (Sigma > SigmaB)//遍历最大的类间方差g
          {
              SigmaB = Sigma;
              Threshold = (uint8)Y;
          }
    }
   return Threshold;
}
//------------------------------------------------------------------------------------------------------------------
//  @brief      图像二值化，这里用的是大津法二值化。
//  @since      v1.0 
//------------------------------------------------------------------------------------------------------------------
uint8 bin_image[image_h][image_w];//图像数组
void turn_to_bin(void){
 	image_thereshold = otsuThreshold(original_image[0], image_w, image_h);
  	for(uint8 i = 0; i<image_h ; ++i){
      	for(uint8 j = 0; j<image_w ; ++j){
			bin_image[i][j] = (original_image[i][j]>image_thereshold) ? white_pixel : black_pixel;
      	}
  	}
}


/*
函数名称：void get_start_point(uint8 start_row)
功能说明：寻找两个边界的边界点作为八邻域循环的起始点
参数说明：输入任意行数
函数返回：无
修改时间：2022年9月8日
备    注：
example：  get_start_point(image_h-2)
 */
Point startPoint_L = {0, 0};
Point startPoint_R = {0, 0};

bool get_start_point(uint8 start_row){
	uint8 l_found = 0, r_found = 0;

	//清零
	startPoint_L.x = 0;
	startPoint_L.y = 0;
	startPoint_R.x = 0;
	startPoint_R.y = 0;

	//从中间往左边，先找起点
	for (uint8 i = image_w / 2; i > border_min; i--){
		startPoint_L.x = i;//x
		startPoint_L.y = start_row;//y
		if (bin_image[start_row][i] == 255 && bin_image[start_row][i - 1] == 0){
			//printf("找到左边起点image[%d][%d]\n", start_row,i);
			l_found = 1;
			break;
		}
	}

	for (uint8 i = image_w / 2; i < border_max; i++){
		startPoint_R.x = i;//x
		startPoint_R.y = start_row;//y
		if (bin_image[start_row][i] == 255 && bin_image[start_row][i + 1] == 0){
			//printf("找到右边起点image[%d][%d]\n",start_row, i);
			r_found = 1;
			break;
		}
	}

	return (l_found && r_found);
}

/*
函数名称：void search_l_r(uint16 break_flag, uint8(*image)[image_w],uint16 *l_stastic, uint16 *r_stastic,
							uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y,uint8*hightest)

功能说明：八邻域正式开始找右边点的函数，输入参数有点多，调用的时候不要漏了，这个是左右线一次性找完。
参数说明：
break_flag_r			：最多需要循环的次数
(*image)[image_w]		：需要进行找点的图像数组，必须是二值图,填入数组名称即可
					   特别注意，不要拿宏定义名字作为输入参数，否则数据可能无法传递过来
*l_stastic				：统计左边数据，用来输入初始数组成员的序号和取出循环次数
*r_stastic				：统计右边数据，用来输入初始数组成员的序号和取出循环次数
l_start_x				：左边起点横坐标
l_start_y				：左边起点纵坐标
r_start_x				：右边起点横坐标
r_start_y				：右边起点纵坐标
hightest				：循环结束所得到的最高高度
函数返回：无
修改时间：2022年9月25日
备    注：
example：
	search_l_r((uint16)USE_num,image,&data_stastics_l, &data_stastics_r,startPoint_L.x,
				startPoint_L.y, startPoint_R.x, startPoint_R.y,&hightest);
 */
#define USE_num	image_h*3	//定义找点的数组成员个数按理说300个点能放下，但是有些特殊情况确实难顶，多定义了一点

 //存放点的x，y坐标
Point points_l[(uint16)USE_num] = {{0}};//左线
Point points_r[(uint16)USE_num] = {{0}};//右线
uint16 dir_r[(uint16)USE_num] = {0};//用来存储右边生长方向
uint16 dir_l[(uint16)USE_num] = {0};//用来存储左边生长方向
uint16 data_stastics_l = 0;//统计左边找到点的个数
uint16 data_stastics_r = 0;//统计右边找到点的个数
uint16 hightest = 0;//最高点
void search_l_r(
	uint16 break_flag, uint8(*image)[image_w], uint16 *l_stastic, uint16 *r_stastic, 
	uint8 l_start_x, uint8 l_start_y, uint8 r_start_x, uint8 r_start_y, uint8*hightest
){

	//左边变量
	Point searchFileds_L[8] = {{0}};
	uint8 index_l = 0;
	Point temp_l[8] = {{0}};
	Point centerPoint_L = {0, 0};
	uint16 l_data_statics;//统计左边
	//定义八个邻域
	static Point seeds_l[8] = {{0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1},{1,0},{1,1}};
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//这个是顺时针

	//右边变量
	Point searchFileds_R[8] = {{0}};
	Point centerPoint_R = {0, 0};//中心坐标点
	uint8 index_r = 0;//索引下标
	Point temp_r[8] = {{0}};
	uint16 r_data_statics;//统计右边
	//定义八个邻域
	static Point seeds_r[8] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};
	//{-1,-1},{0,-1},{+1,-1},
	//{-1, 0},	     {+1, 0},
	//{-1,+1},{0,+1},{+1,+1},
	//这个是逆时针

	l_data_statics = *l_stastic;//统计找到了多少个点，方便后续把点全部画出来
	r_data_statics = *r_stastic;//统计找到了多少个点，方便后续把点全部画出来

	//第一次更新坐标点  将找到的起点值传进来
	centerPoint_L.x = l_start_x;//x
	centerPoint_L.y = l_start_y;//y
	centerPoint_R.x = r_start_x;//x
	centerPoint_R.y = r_start_y;//y

		//开启邻域循环
	while(--break_flag){

		//左边
		for(uint8 i = 0 ; i < 8 ; ++i){ //传递8F坐标
			searchFileds_L[i].x = centerPoint_L.x + seeds_l[i].x;//x
			searchFileds_L[i].y = centerPoint_L.y + seeds_l[i].y;//y
		}
		//中心坐标点填充到已经找到的点内
		points_l[l_data_statics].x = centerPoint_L.x;//x
		points_l[l_data_statics].y = centerPoint_L.y;//y
		l_data_statics++;//索引加一

		//右边
		for(uint8 i = 0 ; i < 8 ; ++i){ //传递8F坐标
			searchFileds_R[i].x = centerPoint_R.x + seeds_r[i].x;//x
			searchFileds_R[i].y = centerPoint_R.y + seeds_r[i].y;//y
		}
		//中心坐标点填充到已经找到的点内
		points_r[r_data_statics].x = centerPoint_R.x;//x
		points_r[r_data_statics].y = centerPoint_R.y;//y

		index_l = 0;//先清零，后使用
		for(uint8 i = 0 ; i < 8 ; ++i){
			temp_l[i].x = 0;//先清零，后使用
			temp_l[i].y = 0;//先清零，后使用
		}

		//左边判断
		for(uint8 i = 0 ; i < 8 ; ++i){
			if(image[searchFileds_L[i].y][searchFileds_L[i].x] == 0
				&& image[searchFileds_L[(i + 1) & 7].y][searchFileds_L[(i + 1) & 7].x] == 255)
			{
				temp_l[index_l].x = searchFileds_L[(i)].x;
				temp_l[index_l].y = searchFileds_L[(i)].y;
				index_l++;
				dir_l[l_data_statics - 1] = (i);//记录生长方向
			}

			if(index_l){
				//更新坐标点
				centerPoint_L.x = temp_l[0].x;
				centerPoint_L.y = temp_l[0].y;
				for(uint8 j = 0; j < index_l; j++){
					if(centerPoint_L.y > temp_l[j].y){
						centerPoint_L.x = temp_l[j].x;
						centerPoint_L.y = temp_l[j].y;
					}
				}
			}
		}
		if((points_r[r_data_statics].x== points_r[r_data_statics-1].x&& points_r[r_data_statics].x == points_r[r_data_statics - 2].x
			&& points_r[r_data_statics].y == points_r[r_data_statics - 1].y && points_r[r_data_statics].y == points_r[r_data_statics - 2].y)
			||(points_l[l_data_statics-1].x == points_l[l_data_statics - 2].x && points_l[l_data_statics-1].x == points_l[l_data_statics - 3].x
				&& points_l[l_data_statics-1].y == points_l[l_data_statics - 2].y && points_l[l_data_statics-1].y == points_l[l_data_statics - 3].y))
		{
			//printf("三次进入同一个点，退出\n");
			break;
		}
		if(my_abs(points_r[r_data_statics].x - points_l[l_data_statics - 1].x) < 2 && my_abs(points_r[r_data_statics].y - points_l[l_data_statics - 1].y < 2)){
			//printf("\n左右相遇退出\n");	
			*hightest = (points_r[r_data_statics].y + points_l[l_data_statics - 1].y) >> 1;//取出最高点
			//printf("\n在y=%d处退出\n",*hightest);
			break;
		}
		if((points_r[r_data_statics].y < points_l[l_data_statics - 1].y)){
			printf("\n如果左边比右边高了，左边等待右边\n");	
			continue;//如果左边比右边高了，左边等待右边
		}
		if(dir_l[l_data_statics - 1] == 7 && (points_r[r_data_statics].y > points_l[l_data_statics - 1].y)){ //左边比右边高且已经向下生长了
			//printf("\n左边开始向下了，等待右边，等待中... \n");
			centerPoint_L.x = points_l[l_data_statics - 1].x;
			centerPoint_L.y = points_l[l_data_statics - 1].y;
			--l_data_statics;
		}
		r_data_statics++;//索引加一

		index_r = 0;//先清零，后使用
		for(uint8 i = 0 ; i < 8 ; i++){
			temp_r[i].x = 0;//先清零，后使用
			temp_r[i].y = 0;//先清零，后使用
		}

		//右边判断
		for(uint8 i = 0 ; i < 8 ; i++){
			if(image[searchFileds_R[i].y][searchFileds_R[i].x] == 0 && image[searchFileds_R[(i+1)&7].y][searchFileds_R[(i+1)&7].x] == 255){
				temp_r[index_r].x = searchFileds_R[i].x;
				temp_r[index_r].y = searchFileds_R[i].y;
				++index_r;//索引加一
				dir_r[r_data_statics - 1] = i;//记录生长方向
				//printf("dir[%d]:%d\n", r_data_statics - 1, dir_r[r_data_statics - 1]);
			}
			if(index_r){
				//更新坐标点
				centerPoint_R.x = temp_r[0].x;//x
				centerPoint_R.y = temp_r[0].y;//y
				for (uint8 j = 0 ; j < index_r ; ++j){
					if (centerPoint_R.y > temp_r[j].y){
						centerPoint_R.x = temp_r[j].x;
						centerPoint_R.y = temp_r[j].y;
					}
				}
			}
		}


	}


	//取出循环次数
	*l_stastic = l_data_statics;
	*r_stastic = r_data_statics;

}
/*
函数名称：void get_left(uint16 total_L)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_L	：找到的点的总数
函数返回：无
修改时间：2022年9月25日
备    注：
example： get_left(data_stastics_l );
 */
uint8 l_border[image_h];//左线数组
uint8 r_border[image_h];//右线数组
uint8 center_line[image_h];//中线数组
void get_left(uint16 total_L)
{
	uint8 i = 0;
	uint16 j = 0;
	uint8 h = 0;
	//初始化
	for (i = 0;i<image_h;i++)
	{
		l_border[i] = border_min;
	}
	h = image_h - 2;
	//左边
	for (j = 0; j < total_L; j++)
	{
		//printf("%d\n", j);
		if (points_l[j].y == h)
		{
			l_border[h] = points_l[j].x+1;
		}
		else continue; //每行只取一个点，没到下一行就不记录
		h--;
		if (h == 0) 
		{
			break;//到最后一行退出
		}
	}
}
/*
函数名称：void get_right(uint16 total_R)
功能说明：从八邻域边界里提取需要的边线
参数说明：
total_R  ：找到的点的总数
函数返回：无
修改时间：2022年9月25日
备    注：
example：get_right(data_stastics_r);
 */
void get_right(uint16 total_R)
{
	uint8 i = 0;
	uint16 j = 0;
	uint8 h = 0;
	for (i = 0; i < image_h; i++)
	{
		r_border[i] = border_max;//右边线初始化放到最右边，左边线放到最左边，这样八邻域闭合区域外的中线就会在中间，不会干扰得到的数据
	}
	h = image_h - 2;
	//右边
	for (j = 0; j < total_R; j++)
	{
		if (points_r[j].y == h)
		{
			r_border[h] = points_r[j].x - 1;
		}
		else continue;//每行只取一个点，没到下一行就不记录
		h--;
		if (h == 0)break;//到最后一行退出
	}
}

//定义膨胀和腐蚀的阈值区间
#define threshold_max	255*5//此参数可根据自己的需求调节
#define threshold_min	255*2//此参数可根据自己的需求调节
//形态学滤波，简单来说就是膨胀和腐蚀的思想
void image_filter(uint8(*bin_image)[image_w]){
	uint32 num = 0;
	for(uint16 i = 1; i < image_h - 1; ++i){
		for(uint16 j = 1; j < (image_w - 1); ++j){
			//统计八个方向的像素值
			num = bin_image[i-1][j-1] + bin_image[i-1][j] + bin_image[i-1][j+1] +
				  bin_image[i][j-1]   + bin_image[i][j+1] +
				  bin_image[i+1][j-1] + bin_image[i+1][j] + bin_image[i+1][j+1];
			if (num >= threshold_max && bin_image[i][j] == 0){
				bin_image[i][j] = 255;//白  可以搞成宏定义，方便更改
			}else if (num <= threshold_min && bin_image[i][j] == 255){
				bin_image[i][j] = 0;//黑
			}
		}
	}

}

/*
函数名称：void image_draw_rectan(uint8(*image)[image_w])
功能说明：给图像画一个黑框
参数说明：uint8(*image)[image_w]	图像首地址
函数返回：无
修改时间：2022年9月8日
备    注：
example： image_draw_rectan(bin_image);
 */
void image_draw_rectan(uint8(*image)[image_w])
{
	for(uint16 i = 0; i < image_h; ++i){
		image[i][0] = 0;
		image[i][1] = 0;
		image[i][image_w - 1] = 0;
		image[i][image_w - 2] = 0;
	}
	for(uint16 i = 0; i < image_w; ++i){
		image[0][i] = 0;
		image[1][i] = 0;
		//image[image_h-1][i] = 0;
	}
}

/*最终处理函数*/
void image_process(void){
	uint8 hightest = 0;//定义一个最高行，tip：这里的最高指的是y值的最小
	
	/*这是离线调试用的*/
	copyImage(mt9v03x_image);
	turn_to_bin();

	/*提取赛道边界*/
	image_filter(bin_image);//滤波
	image_draw_rectan(bin_image);//预处理

	//清零
	data_stastics_l = 0;
	data_stastics_r = 0;
	if (get_start_point(image_h - 2))//找到起点了，再执行八领域，没找到就一直找
	{
		printf("正在开始八领域\n");
		search_l_r((uint16)USE_num, bin_image, &data_stastics_l, &data_stastics_r, startPoint_L.x, startPoint_L.y, startPoint_R.x, startPoint_R.y, &hightest);
		printf("八邻域已结束\n");
		// 从爬取的边界线内提取边线 ， 这个才是最终有用的边线
		get_left(data_stastics_l);
		get_right(data_stastics_r);
		//处理函数放这里，不要放到if外面去了，不要放到if外面去了，不要放到if外面去了，重要的事说三遍

	}


	//显示图像   改成你自己的就行 等后期足够自信了，显示关掉，显示屏挺占资源的
	tft180_displayimage03x(bin_image[0], MT9V03X_W, MT9V03X_H);
	// tft180_show_gray_image(0, 0, (p), MT9V03X_W, MT9V03X_H, (width), (height), 0)
	//根据最终循环次数画出边界点
	for(uint16 i = 0; i < data_stastics_l; i++){
		tft180_draw_point(points_l[i].x+2, points_l[i].y, uesr_BLUE);
	}
	for(uint16 i = 0; i < data_stastics_r; i++){
		tft180_draw_point(points_r[i].x-2, points_r[i].y, uesr_RED);
	}
	for(uint16 i = hightest; i < image_h-1; i++){
		center_line[i] = (l_border[i] + r_border[i]) >> 1;//求中线
		//求中线最好最后求，不管是补线还是做状态机，全程最好使用一组边线，中线最后求出，不能干扰最后的输出
		//当然也有多组边线的找法，但是个人感觉很繁琐，不建议
		tft180_draw_point(center_line[i], i, uesr_GREEN);//显示起点 显示中线
		tft180_draw_point(l_border[i], i, uesr_GREEN);//显示起点 显示左边线
		tft180_draw_point(r_border[i], i, uesr_GREEN);//显示起点 显示右边线
	}


}





/*

这里是起点（0.0）***************——>*************x值最大
************************************************************
************************************************************
************************************************************
************************************************************
******************假如这是一副图像*************************
***********************************************************
***********************************************************
***********************************************************
***********************************************************
***********************************************************
***********************************************************
y值最大*******************************************(188.120)

*/


