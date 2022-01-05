#include <stdio.h>
#include <stdlib.h>
#include "PDB.h"

int main(int argc, char *argv[]){
  FILE *fp;
  PDB *pdb;

  /* 初期化 */
  pdb = (PDB*)malloc(sizeof(PDB));
  pdb->top = NULL;
  pdb->topCA = NULL;

  /* コマンドラインのエラー */
  if(argc != 2){
    printf("コマンドライン引数が誤っています\n");
    exit(1);
  }

  /* 保存ファイル指定エラー */
  if(NULL==(fp=fopen(argv[1],"r"))){
    printf("fopen error!\n");
    exit(1);
  }


  /* 書き込み部分 */
  pdbRead(fp, pdb);
  lpdbMinCalc(pdb);
  printf("%lf %lf %lf\n", pdb->min.x,pdb->min.y,pdb->min.z);
  fclose(fp);


  return 0;
}