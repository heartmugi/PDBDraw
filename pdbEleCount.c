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
  lpdbEleCount(pdb);
  /*ここでそれぞれの元素表示*/
  int i;
  for(i=0; i<pdb->kind.eleNum; i++){
    printf("%.4s:      %d\n",pdb->kind.ele[i].name, pdb->kind.ele[i].count);
  }
  fclose(fp);


  return 0;
}
