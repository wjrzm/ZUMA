#include "stdio.h"
#include "math.h"
#include "assert.h"
 
typedef struct
{
    float X;
    float Y;
} PointF;
 
PointF bezier_interpolation_func(float t, PointF* points, int count)
{
    assert(count>0);
 
    PointF tmp_points[count];
    for (int i = 1; i < count; ++i)
    {
        for (int j = 0; j < count - i; ++j)
        {
            if (i == 1)
            {
                tmp_points[j].X = (float)(points[j].X * (1 - t) + points[j + 1].X * t);
                tmp_points[j].Y = (float)(points[j].Y * (1 - t) + points[j + 1].Y * t);
                continue;
            }
            tmp_points[j].X = (float)(tmp_points[j].X * (1 - t) + tmp_points[j + 1].X * t);
            tmp_points[j].Y = (float)(tmp_points[j].Y * (1 - t) + tmp_points[j + 1].Y * t);
        }
    }
    return tmp_points[0];
}
 
void draw_bezier_curves(PointF* points, int count, PointF* out_points,int out_count)
{
    float step = 1.0 / out_count;
    float t =0;
    for(int i=0; i<out_count; i++)
    {
        PointF temp_point = bezier_interpolation_func(t, points, count);    // 计算插值点
        t += step;
        out_points[i] = temp_point;
    }
}
 
int main(int argc, char **argv)
{
    PointF in[3] = {{50,950},{500,50},{950,950}}; // 输入点
    
    int num = 100;     // 输出点数
    PointF out[num];    // 输出点数组
    
    draw_bezier_curves(in,3,out,num);// 二阶贝塞尔曲线
    
    for(int j=0; j<num; j++)    // 输出路径点
    {
        printf("%d\t X=%f \t Y=%f \r\n",j,out[j].X,out[j].Y);
    }
    return 0;
}
