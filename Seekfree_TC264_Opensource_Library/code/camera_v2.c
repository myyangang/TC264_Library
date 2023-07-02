#include"camera_v2.h"




// 定义图片的二维数组
uint8 camera_image[HEIGHT][WIDTH] = {0
    // 省略图片数据，假设是二值化的图片，白色为255，黑色为0
};

// 定义边缘检测后的二维数组
uint8 edge[HEIGHT][WIDTH];

// 定义道路边缘检测后的二维数组
uint8 road_edge[HEIGHT][WIDTH];

// 定义道路左边缘和右边缘的数组
int16 left_edge[HEIGHT * 3][2];
int16 right_edge[HEIGHT * 3][2];
// 定义道路左边缘和右边缘的长度
uint16 left_edge_length;
uint16 right_edge_length;

// 定义道路左边缘和右边缘角点检测后的二维数组
int16 left_corner[HEIGHT][2];
int16 right_corner[HEIGHT][2];
// 定义道路左边缘和右边缘角点检测后的长度
uint16 left_corner_length;
uint16 right_corner_length;

// 定义8个方向的偏移量
int16 dx[8] = {-1, 0, 1,  1, 1, 0, -1, -1};
int16 dy[8] = {-1, -1, -1, 0, 1, 1, 1, 0};


// 定义道路中央和图片中央的差值
int16 deviation;

// 从mt9v03x_image复制到image
void copy_image(){
    // memcpy(camera_image, mt9v03x_image,HEIGHT * WIDTH);
    for(int16 i = 0; i < HEIGHT; i++){
        for(int16 j = 0; j < WIDTH; j++){
            camera_image[i][j] = mt9v03x_image[i][j];
        }
    }
}
// 定义边缘检测函数，使用Sobel算子进行梯度计算，然后根据阈值判断是否为边缘
void edge_detection() {
    // 初始化边缘数组为0
    for (int16 i = 0; i < HEIGHT; i++) {
        for (int16 j = 0; j < WIDTH; j++) {
            edge[i][j] = NON_EDGE;
        }
    }
    // 遍历图片数组，跳过边界像素
    for (int16 i = 1; i < HEIGHT - 1; i++) {
        for (int16 j = 1; j < WIDTH - 1; j++) {
            // 计算水平方向和垂直方向的梯度
            int16 gx = -camera_image[i-1][j-1] - 2 * camera_image[i][j-1] - camera_image[i+1][j-1]
                     + camera_image[i-1][j+1] + 2 * camera_image[i][j+1] + camera_image[i+1][j+1];
            int16 gy = -camera_image[i-1][j-1] - 2 * camera_image[i-1][j] - camera_image[i-1][j+1]
                     + camera_image[i+1][j-1] + 2 * camera_image[i+1][j] + camera_image[i+1][j+1];
            // 计算梯度幅值
            int16 g = sqrt(gx * gx + gy * gy);
            // 根据阈值判断是否为边缘，并赋值为255
            if (g > THRESHOLD) {
                edge[i][j] = EDGE;
            }
        }
    }
}

// 定义拐点检测函数，使用差分法计算每一行的道路左边缘和右边缘的斜率的变化率，然后根据阈值判断是否为拐点，并记录拐点的位置和数量
void corner_detection() {
    // 初始化拐点数组为-1
    for (int16 i = 0; i < HEIGHT; i++) {
        for (int16 j = 0; j < 2; j++) {
            left_corner[i][j] = -1;
            right_corner[i][j] = -1;
        }
    }
    left_corner_length = 0;
    right_corner_length = 0;

    if (left_edge_length < 7 || right_edge_length < 7) {
        return;
    }

    // 遍历道路边缘数组，跳过前两行和后两行
    for (int16 i = 3; i < left_edge_length - 3; i++) {
        // 计算第i-3个点和i-1个点的变换向量
        int16 vector_left_previous[2] = {left_edge[i-1][0] - left_edge[i-3][0], left_edge[i-1][1] - left_edge[i-3][1]};
        // 计算第i+1个点和i+3个点的变换向量
        int16 vector_left_next[2] = {left_edge[i+3][0] - left_edge[i+1][0], left_edge[i+3][1] - left_edge[i+1][1]};
        // 计算两个向量的欧式距离
        double distance_left = sqrt((vector_left_previous[0] - vector_left_next[0]) * (vector_left_previous[0] - vector_left_next[0])
                                 + (vector_left_previous[1] - vector_left_next[1]) * (vector_left_previous[1] - vector_left_next[1]));


        // 根据阈值判断是否为拐点，并记录拐点的位置
        if (distance_left > CORNER_THRESHOLD) {
            left_corner[left_corner_length][0] = left_edge[i][0];
            left_corner[left_corner_length][1] = left_edge[i][1];
            left_corner_length++;
        }

    }
    // 若果记录的拐点是连续的，则只保留中间的一个
    for (int16 i = 0; i < left_corner_length; i++) {
        
        if (abs(left_corner[i][0] - left_corner[i+1][0]) < 5 && abs(left_corner[i][1] - left_corner[i+1][1]) < 5) {
            // 开始记录连续的拐点
            int16 j = i;
            for (; j < left_corner_length - 1; j++) {
                if (abs(left_corner[j][0] - left_corner[j+1][0]) >= 5 || abs(left_corner[j][1] - left_corner[j+1][1]) >= 5) {
                    break;
                }
            }
            // 保留中间的拐点
            left_corner[i][0] = left_corner[(i + j) / 2][0];
            left_corner[i][1] = left_corner[(i + j) / 2][1];
            // 删除多余的拐点
            for (int16 k = i + 1; k <= j; k++) {
                left_corner[k][0] = -1;
                left_corner[k][1] = -1;
            }
            
            i = j;
        }
    }
    // 更新拐点数组和拐点数量
    int16 new_left_corner_length = 0;
    for (int16 i = 0; i < left_corner_length; i++) {
        if (left_corner[i][0] != -1) {
            left_corner[new_left_corner_length][0] = left_corner[i][0];
            left_corner[new_left_corner_length][1] = left_corner[i][1];
            if(new_left_corner_length != i) {
                left_corner[i][0] = -1;
                left_corner[i][1] = -1;
            }
            new_left_corner_length++;
        }
    }
    left_corner_length = new_left_corner_length;

    for (int16 i = 3; i < right_edge_length - 3; i++) {
        // 计算第i-3个点和i-1个点的变换向量
        int16 vector_right_previous[2] = {right_edge[i-1][0] - right_edge[i-3][0], right_edge[i-1][1] - right_edge[i-3][1]};
        // 计算第i+1个点和i+3个点的变换向量
        int16 vector_right_next[2] = {right_edge[i+3][0] - right_edge[i+1][0], right_edge[i+3][1] - right_edge[i+1][1]};
        // 计算两个向量的欧式距离
        double distance_right = sqrt((vector_right_previous[0] - vector_right_next[0]) * (vector_right_previous[0] - vector_right_next[0])
                                 + (vector_right_previous[1] - vector_right_next[1]) * (vector_right_previous[1] - vector_right_next[1]));

        // 根据阈值判断是否为拐点，并记录拐点的位置
        if (distance_right > CORNER_THRESHOLD) {
            right_corner[right_corner_length][0] = right_edge[i][0];
            right_corner[right_corner_length][1] = right_edge[i][1];
            right_corner_length++;
        }
    }
    // 若果记录的拐点是连续的，则只保留中间的一个
    for (int16 i = 0; i < right_corner_length; i++) {
        
        if (abs(right_corner[i][0] - right_corner[i+1][0]) < 5 && abs(right_corner[i][1] - right_corner[i+1][1]) < 5) {
            // 开始记录连续的拐点
            int16 j = i;
            for (; j < right_corner_length - 1; j++) {
                if (abs(right_corner[j][0] - right_corner[j+1][0]) >= 5 || abs(right_corner[j][1] - right_corner[j+1][1]) >= 5) {
                    break;
                }
            }
            // 保留中间的拐点
            right_corner[i][0] = right_corner[(i + j) / 2][0];
            right_corner[i][1] = right_corner[(i + j) / 2][1];
            // 删除多余的拐点
            for (int16 k = i + 1; k <= j; k++) {
                right_corner[k][0] = -1;
                right_corner[k][1] = -1;
            }
            
            i = j;
        }
    }
    // 更新拐点数组和拐点数量
    int16 new_right_corner_length = 0;
    for (int16 i = 0; i < right_corner_length; i++) {
        if (right_corner[i][0] != -1) {
            right_corner[new_right_corner_length][0] = right_corner[i][0];
            right_corner[new_right_corner_length][1] = right_corner[i][1];
            if(new_right_corner_length != i) {
                right_corner[i][0] = -1;
                right_corner[i][1] = -1;
            }
            new_right_corner_length++;
        }
    }
    right_corner_length = new_right_corner_length;

}


// 定义道路边缘检测函数，使用扫描线法从下往上扫描每一行的像素，找到相离最远的两个边缘像素，并记录为道路左边缘和右边缘
void road_edge_detection() {
    // 初始化道路边缘数组为-1
    for (int16 i = 0; i < HEIGHT * 3; i++) {
        for (int16 j = 0; j < 2; j++) {
            left_edge[i][j] = -1;
            right_edge[i][j] = -1;
        }
    }
    // 将图片的边沿设置成边缘,2像素宽
    for (int16 i = 0; i < HEIGHT; i++) {
        if(i==0||i==1||i==HEIGHT-1||i==HEIGHT-2){
            for (int16 j = 0; j < WIDTH; j++) {
                edge[i][j] = EDGE;
            }
        }
        else{
            edge[i][0] = EDGE;
            edge[i][1] = EDGE;
            edge[i][WIDTH - 1] = EDGE;
            edge[i][WIDTH - 2] = EDGE;
        }
    }
    // 扫描倒数第三行，找到相离最远的两个边缘像素，并记录为道路左边缘和右边缘
    int16 max_distance = 0;
    for (int16 j = 0; j < WIDTH; j++) {
        if (edge[HEIGHT - 3][j] == EDGE) {
            for (int16 k = j + 1; k < WIDTH; k++) {
                if (edge[HEIGHT - 3][k] == EDGE) {
                    int16 distance = k - j;
                    if (distance > max_distance) {
                        max_distance = distance;
                        left_edge[0][0] = j+1;
                        left_edge[0][1] = HEIGHT - 2;
                        left_edge[1][0] = j;
                        left_edge[1][1] = HEIGHT - 3;
                        right_edge[0][0] = k-1;
                        right_edge[0][1] = HEIGHT - 2;
                        right_edge[1][0] = k;
                        right_edge[1][1] = HEIGHT - 3;
                    }
                    j = k-1;
                    break;
                }
            }
        }
    }
    // 如果没有找到道路左边缘和右边缘，说明图片无效，结束函数
    if (max_distance == 0) {
        return;
    }
    // 在上一行的基础上延伸道路左边缘和右边缘，使用8邻域法找到下一个边沿点
    int16 left_x = left_edge[1][0];
    int16 left_y = left_edge[1][1];
    int16 right_x = right_edge[1][0];
    int16 right_y = right_edge[1][1];
    int16 index_left = 1;
    int16 index_right = 1;
    // 定义两个边缘是否相遇标志
    bool is_meet = false;
    while (index_left < HEIGHT * 3 && index_right < HEIGHT * 3) {

        // 初始化下一个点的坐标为-1
        int16 next_left_x = -1;
        int16 next_left_y = -1;
        int16 next_right_x = -1;
        int16 next_right_y = -1;
        // 对于道路左侧边缘的点，其右侧是道路，所以需要从上一次检测到的点开始逆时针检测找到到的第一个边缘点就是下一个道路左侧边缘点
        // 计算上一次检测到的点和当前点的方向
        
        int16 prev_left_x = left_edge[index_left - 1][0];
        int16 prev_left_y = left_edge[index_left - 1][1];


        int16 dir_left = -1;
        for (int16 d = 7; d >= 0; d--) {
            if (prev_left_x == left_x + dx[d] && prev_left_y == left_y + dy[d]) {
                dir_left = d;
                break;
            }
        }
        // 从上一次检测到的点开始逆时针检测
        for (int16 d = (dir_left + 7) % 8,i=0; i<8; d = (d + 7) % 8,i++) {
            // 计算下一个点的坐标
            int16 new_left_x = left_x + dx[d];
            int16 new_left_y = left_y + dy[d];
            // 判断是否在图片范围内，并且是边沿点
            if (new_left_x >= 0 && new_left_x < WIDTH && new_left_y >= 0 && new_left_y < HEIGHT && edge[new_left_y][new_left_x] == EDGE) {
                // 更新下一个点的坐标
                next_left_x = new_left_x;
                next_left_y = new_left_y;
                break;
            }
        }
        // 对于道路右侧边缘的点，其左侧是道路，所以需要从上一次检测到的点开始顺时针检测找到的第一个边缘点就是下一个道路右侧边缘点
        // 计算上一次检测到的点和当前点的方向
        int16 prev_right_x = right_edge[index_right - 1][0];
        int16 prev_right_y = right_edge[index_right - 1][1];

        int16 dir_right = -1;
        for (int16 d = 0; d < 8; d++) {
            if (prev_right_x == right_x + dx[d] && prev_right_y == right_y + dy[d]) {
                dir_right = d;
                break;
            }
        }
        // 从上一次检测到的点开始顺时针检测
        for (int16 d = (dir_right + 1) % 8,i=0; i<8; d = (d + 1) % 8,i++) {
            // 计算下一个点的坐标
            int16 new_right_x = right_x + dx[d];
            int16 new_right_y = right_y + dy[d];
            // 判断是否在图片范围内，并且是边沿点
            if (new_right_x >= 0 && new_right_x < WIDTH && new_right_y >= 0 && new_right_y < HEIGHT && edge[new_right_y][new_right_x] == EDGE) {
                // 更新下一个点的坐标
                next_right_x = new_right_x;
                next_right_y = new_right_y;
                break;
            }
        }
        // 如果找到了下一个点，更新道路左边缘和右边缘数组，并更新当前点的坐标
        if (next_left_x != -1 && next_left_y != -1) {
            index_left++;
            left_edge[index_left][0] = next_left_x;
            left_edge[index_left][1] = next_left_y;
            left_x = next_left_x;
            left_y = next_left_y;
        }
        if (next_right_x != -1 && next_right_y != -1) {
            index_right++;
            right_edge[index_right][0] = next_right_x;
            right_edge[index_right][1] = next_right_y;
            right_x = next_right_x;
            right_y = next_right_y;
        }
        // 两个点相遇，结束循环
        if (((left_edge[index_left][0] == right_edge[index_right][0]) && (left_edge[index_left][1] == right_edge[index_right][1])) || (left_edge[index_left-1][0] == right_edge[index_right][0] && left_edge[index_left-1][1] == right_edge[index_right][1]) || (right_edge[index_right-1][0] == left_edge[index_left][0] && right_edge[index_right-1][1] == left_edge[index_left][1])) {
            is_meet = true;
            break;
        }

        // 将right_edge和left_edge的坐标对应的road_edge的坐标赋值
        road_edge[left_edge[index_left][1]][left_edge[index_left][0]] = EDGE;
        road_edge[right_edge[index_right][1]][right_edge[index_right][0]] = EDGE;
        
    }
    if(is_meet){
        left_edge_length = index_left + 1;
        right_edge_length = index_right + 1;
    }
    else{
        left_edge_length = 0;
        right_edge_length = 0;
    }
}

// // 定义保存图片函数，将二维数组的格式保存到temp.h文件中，每个值占4个位，空格填充
// void save_image(uint8 camera_image[HEIGHT][WIDTH]) {
//     // 打开文件
//     FILE *fp = fopen("temp.h", "w");
//     if (fp == NULL) {
//         printf("Error: cannot open file.\n");
//         return;
//     }
//     // 写入文件
//     fprintf(fp, "const uint8 camera_image[%d][%d] = {\n", HEIGHT, WIDTH);
//     for (int16 i = 0; i < HEIGHT; i++) {
//         // fprintf(fp, "    {");
//         for (int16 j = 0; j < WIDTH; j++) {
//             fprintf(fp, "%3d", camera_image[i][j]);
//             if (j != WIDTH - 1) {
//                 fprintf(fp, ",");
//             }
//         }
//         // fprintf(fp, "}");
//         if (i != HEIGHT - 1) {
//             fprintf(fp, ",");
//         }
//         fprintf(fp, "\n");
//     }
//     fprintf(fp, "};\n");
//     // 关闭文件
//     fclose(fp);
// }

// 定义计算偏离程度函数，使用道路中央和图片中央的差值的绝对值乘以一个放大系数作为返回值，偏离越大，返回值越大
int16 calculate_deviation() {
    // 初始化偏离程度为0
    float deviation = 0.0;
    // 得到两侧拐点中y值较大的拐点的y值
    int16 first_y;
    // 计算两个边缘的y值大于first_y的所有边缘点的x值的平均值
    int16 left_sum = 0;
    int16 right_sum = 0;
    // 找到第一个非图像边缘的拐点的y值
    if(left_corner_length>0 && right_corner_length>0){
        for(int16 i=0;i<left_corner_length;i++){
            if(abs(left_corner[i][0]-1)>5&&abs(left_corner[i][0]-WIDTH+2)>5&&abs(left_corner[i][1]-1)>5&&abs(left_corner[i][1]-HEIGHT+1)>5){
                first_y = left_corner[i][0];
                break;
            }
            else{
                first_y = HEIGHT/2;
            }
        }
        for(int16 i=0;i<right_corner_length;i++){
            if(abs(right_corner[i][0]-1)>5&&abs(right_corner[i][0]-WIDTH+2)>5&&abs(right_corner[i][1]-1)>5&&abs(right_corner[i][1]-HEIGHT+2)>5){
                if(right_corner[i][0]<first_y){
                    first_y = right_corner[i][0];
                    break;
                }
                
            }
        }
    }
    else if(left_corner_length>0){
        for(int16 i=0;i<left_corner_length;i++){
            if(abs(left_corner[i][0]-1)>5&&abs(left_corner[i][0]-WIDTH+2)>5&&abs(left_corner[i][1]-1)>5&&abs(left_corner[i][1]-HEIGHT+1)>5){
                first_y = left_corner[i][0];
                break;
            }
            else{
                first_y = HEIGHT/2;
            }
        }
    }
    else if(right_corner_length>0){
        for(int16 i=0;i<right_corner_length;i++){
            if(abs(right_corner[i][0]-1)>5&&abs(right_corner[i][0]-WIDTH+2)>5&&abs(right_corner[i][1]-1)>5&&abs(right_corner[i][1]-HEIGHT+2)>5){
                first_y = right_corner[i][0];
                break;
            }
            else{
                first_y = HEIGHT/2;
            }
        }
    }
    else{
        first_y = HEIGHT/2;
    }
    
    if (left_edge_length == 0 || right_edge_length == 0) {
        return deviation;
    }
    // 记录有效的边缘点的个数
    int16 left_count = 0;
    for (int16 i = 0; i < left_edge_length; i++) {
        if (left_edge[i][1] >= first_y) {
            left_sum += left_edge[i][0];
            left_count++;
        }
        else{
            break;
        }
    }
    int16 right_count = 0;
    for (int16 i = 0; i < right_edge_length; i++) {
        if (right_edge[i][1] >= first_y) {
            right_sum += right_edge[i][0];
            right_count++;
        }
        else{
            break;
        }
    }
    if (left_count == 0 || right_count == 0) {
        return deviation;
    }
    // 计算x值的平均值和图片中央的差值的绝对值，作为偏离程度
    float left_average = left_sum / left_count;
    float right_average = right_sum / right_count;
    deviation = 1.0*(left_average + right_average) / 2 - 1.0*WIDTH / 2;

    // 乘以一个放大系数，返回偏离程度
    deviation *= SCALE_FACTOR;
    return (int)deviation;
}

// 定义处理图片的函数，先进行边缘检测，然后进行角点检测，然后进行道路边缘检测，然后保存图片，然后计算偏离程度
int16 process_image() {
    copy_image();
    // 边缘检测
    edge_detection();
    // // 保存图片
    // save_image(edge);
    // 道路边缘检测
    // road_edge_detection();
    // 角点检测
    // corner_detection();
    // 保存图片
    // save_image(road_edge);
    // 计算偏离程度
    // int16 deviation = calculate_deviation();
    // 返回偏离程度
    // return deviation;
    return 0;
}



    

// // 测试函数
// int16 main(){
//     // // 定义一个测试用的图片，你可以用自己的图片替换它
//     // int16 image[M][N] = {
//     //     {0, 0, 0, 0, 0, 0, 0, 0, ..., 0},
//     //     {0, 0, 0, 0, 255, 255, 255, 255, ..., 255},
//     //     {0, 0, 0, 255, 255, 255, 255, 255, ..., 255},
//     //     {0, 0, 255, 255, 255, 255, 255, 255, ..., 255},
//     //     ...,
//     //     {0, 255, 255, 255, ..., ..., ..., ..., ..., ..., ..., ..., ..., ..., ..., ..., ..., ..., ..., ..., ..., ..., ..., ...},
//     //     {255, 255, ...}
//     // };
//     // 将const类型的数组的值复制到非const类型的数组中
//     int16 i, j;
//     for (i = 0; i < HEIGHT; i++) {
//         for (j = 0; j < WIDTH; j++)
//             image[i][j] = image_12[i][j];
//     }

//     printf("The deviation is %d\n", process_image());    
//     // for (i = 0; i < HEIGHT; i++) {
//     //     for (j = 0; j < WIDTH; j++)
//     //         image[i][j] = image_6[i][j];
//     // }
//     // process_image();
//     // 给图片画2个像素的黑框
//     // for (i = 0; i < M; i++) {
//     //     image[i][0] = 0;
//     //     image[i][1] = 0;
//     //     image[i][N - 1] = 0;
//     //     image[i][N - 2] = 0;
//     // }
//     // // 定义一个测试用的道路中点，你可以用自己的值替换它
//     // float center = N / 2.0;
//     // // 调用处理图片的函数，并打印结果
//     // float result = process_image(image);
//     // printf("The deviation is %f\n", result);
//     return 0;
// }


