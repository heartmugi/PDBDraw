#include <stdio.h>
#include <stdlib.h>
#include "PDB.h"

int main(int argc, char *argv[]){
  FILE *fp, *fo;
  PDB *pdb;
  int mode;

  /* 初期化 */
  pdb = (PDB*)malloc(sizeof(PDB));
  pdb->top = NULL;
  pdb->topCA = NULL;

  /* コマンドラインのエラー */
  if(argc != 3){
    printf("コマンドライン引数が誤っています\n");
    exit(1);
  }

  /* ファイル指定エラー */
  if(NULL==(fp=fopen(argv[1],"r"))){
    printf("fopen error!\n");
    exit(1);
  }
  if(NULL==(fo=fopen(argv[2],"w"))){
    printf("fopen error!\n");
    exit(1);
  }


  /* 書き込み部分 */
  pdbRead(fp, pdb);
  lpdbCenterCalc(pdb);
  lpdbSizeCalc(pdb);
  lpdbMinCalc(pdb);
  lpdbMaxCalc(pdb);
  lpdbResCount(pdb);
  lpdbEleCount(pdb);
  printf("mode: ");
  scanf("%d", &mode);
  lpdbInfoPrint(fo,pdb,mode);
  fclose(fp);
  fclose(fo);


  return 0;
}
