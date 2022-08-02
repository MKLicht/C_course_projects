#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // sleep()関数を使う
/*#include "gol.h"*/

void my_init_cells(const int, const int, int cell[][70], FILE*);
void my_print_cells(FILE*, int, const int, const int, int cell[][70]);
int my_count_adjacent_cells(int, int, const int, const int, int cell[][70]);
void my_update_cells(const int, const int, int cell[][70]);

int main(int argc, char **argv)
{
  FILE *fp = stdout;
  const int height = 40;
  const int width = 70;

  int cell[height][width];
  for(int y = 0 ; y < height ; y++){
    for(int x = 0 ; x < width ; x++){
      cell[y][x] = 0;
    }
  }

  /* ファイルを引数にとるか、ない場合はデフォルトの初期値を使う */
  if ( argc > 2 ) {
    fprintf(stderr, "usage: %s [filename for init]\n", argv[0]);
    return EXIT_FAILURE;
  }
  else if (argc == 2) {
    FILE *lgfile;
    if ( (lgfile = fopen(argv[1],"r")) != NULL ) {
      my_init_cells(height,width,cell,lgfile); // ファイルによる初期化
    }
    else{
      fprintf(stderr,"cannot open file %s\n",argv[1]);
      return EXIT_FAILURE;
    }
    fclose(lgfile);
  }
  else{
    my_init_cells(height, width, cell, NULL); // デフォルトの初期値を使う
  }

  my_print_cells(fp, 0, height, width, cell); // 表示する
  sleep(1); // 1秒休止

  /* 世代を進める*/
  for (int gen = 1 ;; gen++) {
    my_update_cells(height, width, cell); // セルを更新
    my_print_cells(fp, gen, height, width, cell);  // 表示する
    sleep(1); //1秒休止する
    fprintf(fp,"\e[%dA",height+3);//height+3 の分、カーソルを上に戻す(壁2、表示部1)
  }

  return EXIT_SUCCESS;
}

void my_init_cells(const int height, const int width, int cell[height][width], FILE* fp)
{
   cell[30][20]=1;
   cell[30][22]=1;
   cell[31][22]=1;
   cell[31][23]=1;
   cell[32][20]=1;
}

double live(const int height, const int width, int cell[height][width])
{
    int sum=0;
    for(int i=0;i<height;i++)
       for(int j=0;j<width;j++)
       {
           if(cell[i][j])
             sum++;
       }
    double live = (double)sum / (double)(38*68);
    return live;
}

void my_print_cells(FILE *fp, int gen, const int height, const int width, int cell[height][width])
{
    fp = stdout;
    double l = live(height,width,cell);
    fprintf(fp,"generation=%d\n",gen);
    fprintf(fp,"生存率=%lf\n",l);
    fprintf(fp,"+");
    for(int x = 0 ; x < width ; x++)
        fprintf(fp, "-");
    fprintf(fp, "+\n");
    for(int y = 0; y < height; y++) 
    {
        fprintf(fp,"|");
        for (int x = 0; x < width; x++) 
        {
           if(cell[y][x])
           {
	             fprintf(fp, "\e[31m#\e[0m");
           }
           else
           {
	             fprintf(fp, " ");
           }
         }
         fprintf(fp,"|\n");
     }
    fprintf(fp, "+");
    for (int x = 0 ; x < width ; x++)
        fprintf(fp, "-");
    fprintf(fp, "+\n");
    fflush(fp);
}

int my_count_adjacent_cells(int h, int w, const int height, const int width, int cell[height][width])
{
    int sum=0;
    const int dx[]={1,0,-1,0,-1,1,-1,1}, dy[]={0,-1,0,1,-1,1,1,-1};
    for(int i=0;i<8;i++)
    {
       if(cell[h+dx[i]][w+dy[i]])
         sum++;
    }
    return sum;
}


void my_update_cells(const int height, const int width, int cell[height][width])
{
   int c[height][width];
    for(int i=0;i<height;i++)
       for(int j=0;j<width;j++)
          c[i][j]=cell[i][j];
   for(int i=1; i<height-1;i++)
      for(int j=1; j<width-1; j++)
      {
         int sum = my_count_adjacent_cells(i,j,height,width,c);
         if(cell[i][j])
         {
            if(sum!=2 && sum!=3)
              cell[i][j]=0;
         }
         else
         {
             if(sum==3)
               cell[i][j]=1;
         }
      }
}

