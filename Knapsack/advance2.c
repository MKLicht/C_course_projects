#include<stdio.h>

int main()
{
    int N,W;
    scanf("%d %d", &N, &W);
    int v[101],w[101];
    int V[101][10001];
    for(int i=1;i<=N;i++)
    {
        scanf("%d %d", &v[i], &w[i]);
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
              int v1=v[i]+V[i][j-w[i]];/*ここでiを1減らさないことにします*/
              int v2=V[i-1][j];
              if(v1>v2)
                V[i][j]=v1;
              else 
                V[i][j]=v2;
           }
       }
    printf("%d\n", V[N][W]);
}