#include <stdio.h>

struct student
{
  int id;
  char name[100];
  int age;
  double height;
  double weight;
};

struct tagged_student1
{
  int id;
  char name[100];
  int age;
  double height;
  double weight;
  char tag;
};

struct tagged_student2
{
  char tag;
  int id;
  char name[100];
  int age;
  double height;
  double weight;
};

int main (int argc, char**argv)
{
  struct student s_begin;
  struct student s1;
  struct tagged_student1 s2;
  struct tagged_student2 s3;
  struct tagged_student2 s_end;

  /* 以下に各構造体の中身のポインタを表示するプログラムを書く */
  /* printf で %p フォーマットを指定する*/
  /* 逆順に表示（send, s3, s2, s1, sbegin の順）*/

  // do_something!!

  printf("s_end %p\n",&s_end);
  printf("s3 %p\n",&s3);
  printf("s2 %p\n",&s2);
  printf("s1 %p\n",&s1);
  printf("s_begin %p\n",&s_begin);

  /* 以下には各構造体のサイズをsizeof演算子で計算し、表示する */
  /* printf 表示には%ld を用いる*/

  // do_something!!

  printf("sizeof(s1) %ld\n", sizeof(struct student));
  printf("sizeof(s2) %ld\n", sizeof(struct tagged_student1));
  printf("sizeof(s3) %ld\n", sizeof(struct tagged_student2));

  /*メンバ変数に割り当てられているアドレス */
  printf("studentのメンバー変数のアドレス\n");
  printf("s1.id %p\n", &s1.id);
  printf("s1.name %p\n", s1.name);
  printf("s1.age %p\n", &s1.age);
  printf("s1.height %p\n", &s1.height);
  printf("s1.weight %p\n", &s1.weight);

  printf("tagged_student1のメンバー変数のアドレス\n");
  printf("s2.id %p\n", &s2.id);
  printf("s2.name %p\n", s2.name);
  printf("s2.age %p\n", &s2.age);
  printf("s2.height %p\n", &s2.height);
  printf("s2.weight %p\n", &s2.weight);
  printf("s2.tag %p\n", &s2.tag);

  printf("tagged_student2のメンバー変数のアドレス\n");
  printf("s3.tag %p\n", &s3.tag);
  printf("s3.id %p\n", &s3.id);
  printf("s3.name %p\n", s3.name);
  printf("s3.age %p\n", &s3.age);
  printf("s3.height %p\n", &s3.height);
  printf("s3.weight %p\n", &s3.weight);

  return 0;
}
