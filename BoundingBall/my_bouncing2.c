#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct condition
{
  const int width; // 見えている範囲の幅
  const int height; // 見えている範囲の高さ
  const double G; // 重力定数
  const double dt; // シミュレーションの時間幅
  const double cor; // 壁の反発係数
} Condition;

 //個々の物体を表す構造体
typedef struct object
{
  double m;
  double x;
  double y;
  double prev_x;
  double prev_y; // 壁からの反発に使用
  double vx;
  double vy;
} Object;

void my_plot_objects(Object objs[], const size_t numobj, const double t, const Condition cond);
void my_update_velocities(Object objs[], const size_t numobj, const Condition cond);
void my_update_positions(Object objs[], const size_t numobj, const Condition cond);
void my_bounce(Object objs[], const size_t numobj, const Condition cond);
void together(Object objs[], const size_t numobj, const Condition cond);

int main(int argc, char **argv)
{
  const Condition cond = {
		    .width  = 75,
		    .height = 40,
		    .G = 1.0,
		    .dt = 1.0,
		    .cor = 0.8
  };
  
  size_t objnum = 7;
  Object objects[objnum];


  // objects[1] は巨大な物体を画面外に... 地球のようなものを想定
  objects[0] = (Object){ .m = 6.0, .x = 0.0, .y = -19.9, .vx = -2.0, .vy = 2.0};
  objects[1] = (Object){ .m = 4.0, .x = 10.0, .y = -1.0, .vx = 3.0, .vy = 1.0};
  objects[2] = (Object){ .m = 2.0, .x = -20.0, .y = 10.0, .vx = -1.0, .vy = 3.0};
  objects[3] = (Object){ .m = 5.0, .x = -5.0, .y = 4.0, .vx = 4.0, .vy = 5.0};
  objects[4] = (Object){ .m = 6.0, .x = -8.0, .y = 7.0, .vx = -1.0, .vy = 4.0};
  objects[5] = (Object){ .m = 3.0, .x = -15.0, .y = 12.0, .vx = 5.0, .vy = 3.0};
  objects[6] = (Object){ .m = 100000.0, .x = 1000, .y = 1000, .vy = 0.0};

  // シミュレーション. ループは整数で回しつつ、実数時間も更新する
  const double stop_time = 400;
  double t = 0;
  for (int i = 0 ; t <= stop_time ; i++){
    t = i * cond.dt;
    my_update_velocities(objects, objnum, cond);
    my_update_positions(objects, objnum, cond);
    my_bounce(objects, objnum, cond);

    together(objects, objnum, cond);
    
    // 表示の座標系は width/2, height/2 のピクセル位置が原点となるようにする
    my_plot_objects(objects, objnum, t, cond);
    
    usleep(200 * 1000); // 200 x 1000us = 200 ms ずつ停止
    printf("\e[%dA", cond.height+3);// 壁とパラメータ表示分で3行
  }
  return EXIT_SUCCESS;
}

// 実習: 以下に my_ で始まる関数を実装する
// 最終的に phisics2.h 内の事前に用意された関数プロトタイプをコメントアウト
void my_update_velocities(Object objs[], const size_t numobj, const Condition cond)
{
  for(int i=0;i<numobj-1;i++)
  {
    double fx=0.0,fy=0.0;
    for(int j=0;j<numobj;j++)
    {
      if(j!=i)
      {
        double d = sqrt((objs[j].x-objs[i].x)*(objs[j].x-objs[i].x)+(objs[j].y-objs[i].y)*(objs[j].y-objs[i].y));
        double fij = cond.G * objs[i].m * objs[j].m / (d*d);
        double fi = fij/d*(objs[j].x-objs[i].x);
        double fj = fij/d*(objs[j].y-objs[i].y);
        fx+=fi;
        fy+=fj;
      }
    }
    double ax=fx/objs[i].m;
    double ay=fy/objs[i].m;
    objs[i].vx+=ax;
    objs[i].vy+=ay;
  }
}

void my_update_positions(Object objs[7], const size_t objnum, const Condition cond)
{
  for(int i=0;i<objnum-1;i++)
  {
    objs[i].prev_x=objs[i].x;
    objs[i].prev_y=objs[i].y;
    objs[i].x+=objs[i].vx;
    objs[i].y+=objs[i].vy;
  }
}

void my_bounce(Object objs[7], const size_t numobj, const Condition cond)
{
  for(int i=0;i<numobj-1;i++)
  {
    if(objs[i].x<=-(cond.width/2))
    {
      objs[i].y+=2*((objs[i].prev_x-(-(cond.width/2)))*((objs[i].prev_y-objs[i].y)/(objs[i].prev_x-objs[i].x)));
      objs[i].vx=-objs[i].vx;
    }
    else if(objs[i].x>=(cond.width/2))
    {
      objs[i].y+=2*(((cond.width/2)-objs[i].prev_x)*((objs[i].prev_y-objs[i].y)/(objs[i].prev_x-objs[i].x)));
      objs[i].vx=-objs[i].vx;
    }
    else if(objs[i].y>=(cond.height/2))
    {
      objs[i].x+=2*(((cond.height/2)-objs[i].prev_y)*((objs[i].x-objs[i].prev_x)/(objs[i].y-objs[i].prev_y)));
      objs[i].vy=-objs[i].vy;
    }
    else if(objs[i].y<=-(cond.height/2))
    {
      objs[i].x+=2*((objs[i].prev_y-(-(cond.height/2)))*((objs[i].x-objs[i].prev_x)/(objs[i].prev_y-objs[i].y)));
      objs[i].vy=-objs[i].vy;
    }
    while(objs[i].x<=-(cond.width/2)||objs[i].x>=(cond.width/2)||objs[i].y>=(cond.height/2)||objs[i].y<=-(cond.height/2))
    {
        if(objs[i].x<=-(cond.width/2))
        {
            objs[i].x+=2*(-(cond.width)/2-objs[i].x);
            objs[i].vx=-objs[i].vx;
        }
        else if(objs[i].x>=(cond.width/2))
        {
            objs[i].x-=2*(objs[i].x-(cond.width)/2);
            objs[i].vx=-objs[i].vx;
        }
        else if(objs[i].y>=(cond.height/2))
        {
            objs[i].y-=2*(objs[i].y-(cond.height)/2);
            objs[i].vy=-objs[i].vy;
        }
        else if(objs[i].y<=-(cond.height/2))
        {
            objs[i].y+=2*(-(cond.height)/2-objs[i].y);
            objs[i].vy=-objs[i].vy;
        }
    }
  }
}

void together(Object objs[7], const size_t numobj, const Condition cond)
{
    for(int i=0;i<numobj-1;i++)
    {
        for(int j=i+1;j<numobj-1;j++)
        {
            if(i!=j && objs[i].x==objs[j].x && objs[i].y==objs[j].y)
            {
                objs[j].m=0;
                objs[i].vx+=objs[j].vx;
                objs[i].vy+=objs[j].vy;
            }
        }
    }
}

void my_plot_objects(Object objs[7], const size_t objnum, const double t, const Condition cond)
{
  printf("+");
  for(int x = -(int)(cond.width/2) ; x < (int)(cond.width/2) ; x++)
      printf("-");
  printf("+\n");
  for(int y = -(int)(cond.height/2) ; y < (int)(cond.height/2) ; y++) 
  {
      printf("|");
      for (int x = -(int)(cond.width/2) ; x < (int)(cond.width/2) ; x++)
      {
          int flag=0;
          for(int i=0;i<objnum-1;i++)
          {
            if(x==(int)(objs[i].x) && y==(int)(objs[i].y) && objs[i].m!=0)
            {
              printf("o");
              flag=1;
              break;
            }
          }
          if(!flag)
          {
	            printf(" ");
          }
      }
      printf("|\n");
  }
  printf("+");
  for (int x = -(int)(cond.width/2) ; x < (int)(cond.width/2) ; x++)
      printf("-");
  printf("+\n");
  printf("t = %.1lf\n",t);
  for(int i=0;i<objnum;i++)
     printf("objs[%d].x = %.2lf, objs[%d].y = %.2lf\n", i, objs[i].x, i, objs[i].y);
}