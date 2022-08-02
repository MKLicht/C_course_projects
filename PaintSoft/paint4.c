#if 0
paint3をもとに,塗りつぶしのコマンド"fill","unfill"を追加しました。
また、コピー機能"copy"も追加しました。
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <errno.h> // for error catch

#define bufsize 1000
#define pi 3.14
#define R 2.2//mend ration

// Structure for canvas
typedef struct
{
  int width;
  int height;
  char **canvas;
  char pen;
} Canvas;

struct node{
  char *str;
  struct node * next;
};

typedef struct node Node;

Node *history = NULL;

Node *push_front(Node *begin, const char *str)
{
  // Create a new element
  Node *p = (Node *)malloc(sizeof(Node));
  char *s = (char *)malloc(strlen(str) + 1);
  strcpy(s, str);
  p->str = s;
  p->next = begin; 

  return p;  // Now the new element is the first element in the list
}


Node *push_back(Node *begin, const char *str)
{
  if (begin == NULL) {   // If the list is empty
    return push_front(begin, str);
  }

  // Find the last element
  Node *p = begin;
  while (p->next != NULL) {
    p = p->next;
  }

  // Create a new element
  Node *q = (Node *)malloc(sizeof(Node));
  char *s = (char *)malloc(strlen(str) + 1);
  strcpy(s, str);
  q->str = s;
  q->next = NULL;

  // The new element should be linked from the previous last element
  p->next = q;

  return begin;
}

Node *pop_back(Node *begin){
  if (begin == NULL) {   // If the list is empty
    printf("we can not delete for there isn't anything!\n");
  }

  // Find the last element
  Node *p = begin;
  Node *q = p->next;
  while (q->next != NULL) {
    q = q->next;
    p = p->next;
  }
  p->next = NULL;
  free(q);
  return begin;
}


// functions for Canvas type
Canvas *init_canvas(int width, int height, char pen);
void reset_canvas(Canvas *c);
void print_canvas(FILE *fp, Canvas *c);
void free_canvas(Canvas *c);

// display functions
void rewind_screen(FILE *fp,unsigned int line);
void clear_command(FILE *fp);
void clear_screen(FILE *fp);

// enum for interpret_command results
typedef enum res{ EXIT, NORMAL, COMMAND} Result;

int max(const int a, const int b);
void draw_line(Canvas *c, const int x0, const int y0, const int x1, const int y1);
void draw_circle(Canvas *c, const int x0, const int y0, const int r);
Result interpret_command(const char *command, Canvas *c, int *hsize);
void save_history(const char *filename, const int hsize);

int flag = 0;

int main(int argc, char **argv)
{
  int width;
  int height;
  //const int bufsize = 1000;
  if (argc != 3){
    fprintf(stderr,"usage: %s <width> <height>\n",argv[0]);
    return EXIT_FAILURE;
  } else{
    long w = strtol(argv[1],NULL,10);
    if(errno == ERANGE){
      fprintf(stderr, "%s: %s\n",argv[1],strerror(errno));
      return EXIT_FAILURE;
    }
    long h = strtol(argv[2],NULL,10);
    if(errno == ERANGE){
      fprintf(stderr, "%s: %s\n",argv[2],strerror(errno));
      return EXIT_FAILURE;
    }
    width = (int)w;
    height = (int)h;    
  }
  char pen = '*';

  FILE *fp;
  char buf[bufsize];
  fp = stdout;
  Canvas *c = init_canvas(width,height, pen);
  int hsize = 0;

  fprintf(fp,"\n"); // required especially for windows env
  while (1) {
    print_canvas(fp,c);
    printf("%d > ", hsize);
    fgets(buf, bufsize, stdin);

    const Result r = interpret_command(buf,c,&hsize);
    if (r == EXIT) break;
    if (r == NORMAL) {
      history = push_back(history, buf);
      hsize++;
    }

    rewind_screen(fp,2); // command results
    clear_command(fp); // command itself
    rewind_screen(fp, height+2); // rewind the screen to command input
    //clear_command(fp); // clear the previous command
  }

  clear_screen(fp);
  free_canvas(c);
  fclose(fp);

  return 0;
}

Canvas *init_canvas(int width,int height, char pen)
{
  Canvas *new = (Canvas *)malloc(sizeof(Canvas));
  new->width = width;
  new->height = height;
  new->canvas = (char **)malloc(width * sizeof(char *));

  char *tmp = (char *)malloc(width*height*sizeof(char));
  memset(tmp, ' ', width*height*sizeof(char));
  for (int i = 0 ; i < width ; i++){
    new->canvas[i] = tmp + i * height;
  }
  
  new->pen = pen;
  return new;
}

void reset_canvas(Canvas *c)
{
  const int width = c->width;
  const int height = c->height;
  memset(c->canvas[0], ' ', width*height*sizeof(char));
}


void print_canvas(FILE *fp, Canvas *c)
{
  const int height = c->height;
  const int width = c->width;
  char **canvas = c->canvas;
  

  fprintf(fp,"+");
  for (int x = 0 ; x < width ; x++)
    fprintf(fp, "-");
  fprintf(fp, "+\n");


  for (int y = 0 ; y < height ; y++) {
    fprintf(fp,"|");
    for (int x = 0 ; x < width; x++){
      const char c = canvas[x][y];
      fputc(c, fp);
    }
    fprintf(fp,"|\n");
  }
  
  
  fprintf(fp, "+");
  for (int x = 0 ; x < width ; x++)
    fprintf(fp, "-");
  fprintf(fp, "+\n");
  fflush(fp);
}

void free_canvas(Canvas *c)
{
  free(c->canvas[0]); //  for 2-D array free
  free(c->canvas);
  free(c);
}

void rewind_screen(FILE *fp,unsigned int line)
{
  fprintf(fp,"\e[%dA",line);
}

void clear_command(FILE *fp)
{
  fprintf(fp,"\e[2K");
}

void clear_screen(FILE *fp)
{
  fprintf(fp, "\e[2J");
}

int max(const int a, const int b)
{
  return (a > b) ? a : b;
}

void draw_line(Canvas *c, const int x0, const int y0, const int x1, const int y1)
{
  const int width = c->width;
  const int height = c->height;
  char pen = c->pen;
  
  const int n = max(abs(x1 - x0), abs(y1 - y0));
  c->canvas[x0][y0] = pen;
  for (int i = 1; i <= n; i++) {
    const int x = x0 + i * (x1 - x0) / n;
    const int y = y0 + i * (y1 - y0) / n;
    if ( (x >= 0) && (x< width) && (y >= 0) && (y < height))
      c->canvas[x][y] = pen;
  }
  //printf("1 line drawn\n");
}

void draw_circle(Canvas *c, const int x0, const int y0, const int r)
{
  const int width = c->width;
  const int height = c->height;
  char pen = c->pen;
  
  float x1, y1;

  for(int i = 0; i <= 360; i++)
  {
    x1 = x0 + r*cos(i*pi/180);
    y1 = y0 + r*sin(i*pi/180);
    c->canvas[(int)x1+1][(int)(y1/R)+1] =pen;
  }
  if(flag){
    for(float x=x0-r+1; x<=x0+r-1; x=x+0.1)
        for(float y=y0-r+1; y<=y0+r-1; y=y+0.1){
            if(c->canvas[(int)x][(int)(y/R)] == ' ' && c->canvas[(int)x-1][(int)(y/R)] == pen)
            {
              c->canvas[(int)x][(int)(y/R)] = pen;
              if(c->canvas[(int)x+1][(int)(y/R)]==pen)
                break;
            }
        }
   }
}

void save_history(const char *filename, const int history_size)
{
  const char *default_history_file = "history.txt";
  if (filename == NULL)
    filename = default_history_file;
  
  FILE *fp;
  if ((fp = fopen(filename, "w")) == NULL) {
    fprintf(stderr, "error: cannot open %s.\n", filename);
    return;
  }
  int i;
  Node* begin = history;
  while(begin!= NULL){
    fprintf(fp, "%s", begin->str);
    begin = begin ->next;
  }
  //printf("saved as \"%s\"\n", filename);
  fclose(fp);
}

Result interpret_command(const char *command, Canvas *c, int *hsize)
{
  char buf[bufsize];
  strcpy(buf, command);
  buf[strlen(buf) - 1] = 0; // remove the newline character at the end

  const char *s = strtok(buf, " ");

  // The first token corresponds to command
  if (strcmp(s, "line") == 0) {
    int x0, y0, x1, y1;
    x0 = 0; y0 = 0; x1 = 0; y1 = 0; // initialize
    char *b[4];
    for (int i = 0 ; i < 4; i++){
      b[i] = strtok(NULL, " ");
      if (b[i] == NULL){
        printf("the number of point is not enough.\n");
        return COMMAND;
      }
    }
    x0 = strtol(b[0],NULL,10);
    y0 = strtol(b[1],NULL,10);
    x1 = strtol(b[2],NULL,10);
    y1 = strtol(b[3],NULL,10);

    draw_line(c, x0, y0, x1, y1);
    printf("1 line drawn\n");
    return NORMAL;
  }
  
  if (strcmp(s, "rec") == 0){
    int x0, y0, x1, y1, x2, y2, x3, y3;
    x0 = atoi(strtok(NULL, " "));
    y0 = atoi(strtok(NULL, " "));
    x1 = atoi(strtok(NULL, " "));
    y1 = atoi(strtok(NULL, " "));
    x2 = atoi(strtok(NULL, " "));
    y2 = atoi(strtok(NULL, " "));
    x3 = atoi(strtok(NULL, " "));
    y3 = atoi(strtok(NULL, " "));    
    draw_line(c, x0, y0, x1, y1);    
    draw_line(c, x1, y1, x2, y2);
    draw_line(c, x2, y2, x3, y3);
    draw_line(c, x3, y3, x0, y0);
    if(flag)
	{
      for(int x = x0+1; x < x1; x++)
          for(int y = y0+1; y<y2; y++)
              c->canvas[x][y] = c->pen;
	}
    return NORMAL;
  }
  
  if (strcmp(s, "circle") == 0) {
    int x0, y0, r, x1, y1;
    x0 = atoi(strtok(NULL, " "));
    y0 = atoi(strtok(NULL, " "));
    r =  atoi(strtok(NULL, " "));
    draw_circle(c, x0, y0, r);
    return NORMAL;
  }
  
  if (strcmp(s, "save") == 0) {
    s = strtok(NULL, " ");
    save_history(s, *hsize);
    printf("saved as \"%s\"\n",(s==NULL)?"history.txt":s);
    return COMMAND;
  }

  if (strcmp(s, "undo") == 0) {
    reset_canvas(c);
    history = pop_back(history);
    Node *p;
    for (p = history; p != NULL; p = p->next) {
      interpret_command(p->str, c, hsize);
    }
    (*hsize)--;
    return COMMAND;
  }
  
  if (strcmp(s, "change") == 0) {
    if(c->pen == '*')
      c->pen = '#';
    else 
      c->pen = '*';
    return COMMAND;
  }
  
  if (strcmp(s, "load") == 0){
        char b[bufsize];
        FILE *fp1 = fopen("history.txt", "r");
        while(fgets(b, bufsize, fp1)!=NULL){
            interpret_command(b, c, 0);
        }
      return NORMAL;
  }
  
  if(strcmp(s, "fill") == 0){
        flag = 1;
        return NORMAL;
  }
  
  if(strcmp(s, "unfill") == 0){
        flag = 0;
        return NORMAL;
  }
  
  if (strcmp(s, "copy") == 0){
      char b[bufsize];
      printf("history.txt has been copied to copy.txt\n");
      FILE *fp1 = fopen("history.txt", "r");
      FILE *fp2 = fopen("copy.txt","w");
      while(fgets(b, bufsize, fp1)!=NULL){
          fputs(b,fp2);
      }
      return NORMAL;
    }

  if (strcmp(s, "quit") == 0) {
    return EXIT;
  }

  printf("error: unknown command.\n");

  return COMMAND;
}