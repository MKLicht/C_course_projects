#include<stdio.h>

int main()
{
    int N,W;/*Nは品物の数、Wは重さ制限で、その2つの量を入力します。*/
    scanf("%d %d", &N, &W);
    int v[1000],w[1000];
    int V[101][10001];
    for(int i=1;i<=N;i++)
    {
        scanf("%d %d", &v[i], &w[i]);/*各品物の価値と重さを入力します。*/
    }
    int i,j;
    for(i=0;i<=N;i++)
       for(j=0;j<=W;j++)
          V[i][j]=0;
    for(i=1;i<=N;i++)
       for(j=1;j<=W;j++)
       {
           if(j-w[i]<0)
             V[i][j]=V[i-1][j];
           else
           {
               int v1=v[i]+V[i-1][j-w[i]];
               int v2=V[i-1][j];
               if(v1>v2)
                 V[i][j]=v1;
               else V[i][j]=v2;
           }
       }
    printf("%d\n", V[N][W]);
}