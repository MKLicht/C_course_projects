#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "encode.h"


// 各シンボルの数を数える為に
//このソースファイルでのみ有効なstaticな配列を設定
// 数を数える為に、1byteの上限+1で設定しておく
static const int nsymbols = 256 + 1; 
static int symbol_count[nsymbols];

// ノードを表す構造体
typedef struct node
{
  int symbol;
  int count;
  struct node *left;
  struct node *right;
  int bit; //そのノードに対応する枝が0か1かを表示
} Node;

// このソースで有効なstatic関数のプロトタイプ宣言
// 一方で、ヘッダファイルは外部からの参照を許す関数の宣言のみ


// ファイルを読み込み、static配列の値を更新する関数
static void count_symbols(const char *filename);

// 与えられた引数でNode構造体を作成し、そのアドレスを返す関数
static Node *create_node(int symbol, int count, Node *left, Node *right);

// Node構造体へのポインタが並んだ配列から、最小カウントを持つ構造体をポップしてくる関数
// n は 配列の実効的な長さを格納する変数を指している（popするたびに更新される）
static Node *pop_min(int *n, Node *nodep[]);

// ハフマン木を構成する関数（根となる構造体へのポインタを返す）
static Node *build_tree(void);

// 木を深さ優先で操作する関数
static void traverse_tree(const int depth, const Node *np);



// 以下 static関数の実装
static void count_symbols(const char *filename)
{
  FILE *fp = fopen(filename, "rb");
  if (fp == NULL) {
    fprintf(stderr, "error: cannot open %s\n", filename);
    exit(1);
  }
  
  for (int i = 0; i < nsymbols; i++) {
    symbol_count[i] = 0;
  }
  
  // 1Byteずつ読み込み、カウントする
  int i = fgetc(fp);
  while(i != EOF)
  {
    if(i >= 0 && i< nsymbols)
      symbol_count[i]++;
    i = fgetc(fp);
  }
  symbol_count[nsymbols-1]++;

  fclose(fp);
}

static Node *create_node(int symbol, int count, Node *left, Node *right)
{
  Node *ret = (Node *)malloc(sizeof(Node));
  *ret = (Node){ .symbol = symbol, .count = count, .left = left, .right = right};
  return ret;
}

static Node *pop_min(int *n, Node *nodep[])
{
  // Find the node with the smallest count
  // カウントが最小のノードを見つけてくる
  int argmin = 0;
  for (int i = 0; i < *n; i++) {
    if (nodep[i]->count < nodep[argmin]->count) {
      argmin = i;
    }
  }

  Node *node_min = nodep[argmin];

  // Remove the node pointer from nodep[]
  // 見つかったノード以降の配列を前につめていく
  for (int i = argmin; i < (*n) - 1; i++) {
    nodep[i] = nodep[i + 1];
  }
  // 合計ノード数を一つ減らす
  (*n)--;

  return node_min;
}

static Node *build_tree()
{
  int n = 0;
  Node *nodep[nsymbols];

  for (int i = 0; i < nsymbols; i++) {
    // カウントの存在しなかったシンボルには何もしない
    if (symbol_count[i] == 0) continue;

    // 以下は nodep[n++] = create_node(i, symbol_count[i], NULL, NULL); と1行でもよい
    nodep[n] = create_node(i, symbol_count[i], NULL, NULL);
    n++;
  }

  //const int dummy = -1; // ダミー用のsymbol を用意しておく
  while (n >= 2) {
    Node *node1 = pop_min(&n, nodep);
    Node *node2 = pop_min(&n, nodep);

    // Create a new node
    // 選ばれた2つのノードを元に統合ノードを新規作成
    // 作成したノードはnodep にどうすればよいか?
    nodep[n] = malloc(sizeof(Node));
    int symbol = node1->count + node2->count;
    int count = node1->count + node2->count;
    nodep[n] = create_node(symbol, count, node1, node2);
    nodep[n]->left->bit = 0;
    nodep[n]->right->bit = 1;
    n++;
  }
  return nodep[0];
}

// Perform depth-first traversal of the tree
// 深さ優先で木を走査する
// 現状は何もしていない（再帰してたどっているだけ）

char code[100];

static void traverse_tree(const int depth, const Node *np)
{ 
  if (np == NULL) return;
  if (np->bit)
     code[depth-1] = '1';
  if (!np->bit)
     code[depth-1] = '0';
  if (!np->left && !np->right)
  {
    for(int i = 0; i<depth; i++)
       printf(" ");
    if(np->symbol==256)
      printf("NULL code: %s\n", code);
    else if(np->symbol==10)
      printf("NEWLINE code: %s\n", code);
    else if(np->symbol==32)
      printf("SPACE code: %s\n", code);
    else
      printf("%c code: %s\n", np->symbol, code);
    code[depth-1] = '\0';
  }
  traverse_tree(depth + 1, np->left);
  traverse_tree(depth + 1, np->right);
}

// この関数のみ外部 (main) で使用される (staticがついていない)
int encode(const char *filename)
{
  count_symbols(filename);
  Node *root = build_tree();
  traverse_tree(0, root);
  //printf("code 111: SPACE\ncode 110010101100: NULL\ncode 0001100: RETURN\n");
  return EXIT_SUCCESS;
}
