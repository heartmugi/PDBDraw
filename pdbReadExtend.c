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
  /*
  pdb->currentCA = pdb->topCA;
  while(pdb->currentCA != NULL){
    printf("%s %d %lf %lf %s %s %lf %lf %lf %s\n",pdb->currentCA->atom.resName,pdb->currentCA->atom.resNumber,pdb->currentCA->atom.tempFactor,pdb->currentCA->atom.occupancy,pdb->currentCA->atom.atomName,pdb->currentCA->atom.element,pdb->currentCA->atom.x,pdb->currentCA->atom.y,pdb->currentCA->atom.z, pdb->currentCA->atom.element);
    pdb->currentCA = pdb->currentCA->nextCA;
  }
  */
  pdb->current = pdb->top;
  while(pdb->current != NULL){
    printf("%s %d %lf %lf %s %s %lf %lf %lf\n",pdb->current->atom.resName,pdb->current->atom.resNumber,pdb->current->atom.tempFactor,pdb->current->atom.occupancy,pdb->current->atom.atomName,pdb->current->atom.element,pdb->current->atom.x,pdb->current->atom.y,pdb->current->atom.z);
    pdb->current = pdb->current->nextAtom;
  }
  fclose(fp);


  return 0;
}
