#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PDB.h"
#include "config.h"

int pdbRead(FILE* fpt, PDB *pdb){
  char str[1024];
  char x[9], y[9], z[9];
  /* \拡張部分の変数 */
  char resName[4];     // 残基名（3文字表記）を格納
  char resNumber[5];   // 残基番号を格納
  char tempFactor[7];  // 温度因子を格納
  char occupancy[7];   // 占有度を格納
  char atomName[5];    // 原子名を格納
  char element[3];     // 元素記号を格納
  
  /* 初期化 */
  pdb->numAtom = 0;
  pdb->numCA = 0;
  pdb->current = pdb->top = NULL;
  pdb->currentCA = pdb->topCA = NULL;
  
  
  //fgetsの戻り値がnullになるまで続ける
  //strにファイルからバイト取得し格納
  while((fgets(str,256,fpt))!=NULL){
    char str_cmp[8];
    strncpy(str_cmp, str, 7);
    str_cmp[7] = '\0';
    if(strstr(str_cmp,"ATOM ")!=NULL || strstr(str_cmp,"HETATM ")!=NULL){    // "ATOM "か"HETATM "が文字列にある
      pdb->numAtom++;
      strncpy(x, str+30, 8);      // x座標取得
      strncpy(y, str+38, 8);      // y座標取得
      strncpy(z, str+46, 8);       // z座標取得
      x[8] = y[8] = z[8] = '\0';  // "\0"を付加
      /* 拡張部分の抽出ポイント */
      strncpy(element, str+86, 2);     // 元素記号取得
      //printf("%s\n",element);
      strncpy(atomName, str+13, 4);   // 原子名取得
      strncpy(resName, str+17, 3);    // 残基名取得
      strncpy(resNumber, str+22, 4);  // 残基番号取得
      strncpy(occupancy, str+54, 6);  // 占有度取得
      strncpy(tempFactor, str+60, 6); // 温度因子取得
      element[2] = '\0';
      atomName[4] = '\0';
      resName[3] = '\0';
      resNumber[4] = '\0';
      occupancy[6] = '\0';
      tempFactor[6] = '\0';
      
      recordPDB* tmp;
      tmp = (recordPDB*)malloc(sizeof(recordPDB));
      tmp->nextAtom = NULL;
      tmp->nextCA = NULL;
      tmp->atom.x = atof(x);
      tmp->atom.y = atof(y);
      tmp->atom.z = atof(z);
      /* 拡張部分の格納 */
      strcpy(tmp->atom.resName, resName);
      tmp->atom.resNumber = atoi(resNumber);
      tmp->atom.tempFactor = atof(tempFactor);
      tmp->atom.occupancy = atof(occupancy);
      strcpy(tmp->atom.atomName, atomName);
      strcpy(tmp->atom.element, element);
      
      if(pdb->current == NULL){
	pdb->current = 	pdb->top = tmp;
      } else {
	pdb->current->nextAtom = tmp;
	pdb->current = tmp;
      }
      char str_cmp2[6];
      strncpy(str_cmp2, str+12, 5);
      str_cmp2[5] = '\0';
      if(strstr(str_cmp2,"CA ")!=NULL){    // "CA "が文字列にある
	pdb->numCA++;
	recordPDB* tmp2;
	tmp2 = (recordPDB*)malloc(sizeof(recordPDB));
	tmp2->nextAtom = NULL;
	tmp2->nextCA = NULL;
	tmp2->atom.x = atof(x);
	tmp2->atom.y = atof(y);
	tmp2->atom.z = atof(z);
	/* 拡張部分の格納 */
	strcpy(tmp2->atom.resName, resName);
	tmp2->atom.resNumber = atoi(resNumber);
	tmp2->atom.tempFactor = atof(tempFactor);
	tmp2->atom.occupancy = atof(occupancy);
	strcpy(tmp2->atom.atomName, atomName);
	strcpy(tmp2->atom.element, element);
      
	if(pdb->currentCA == NULL){
	  pdb->currentCA = pdb->topCA = tmp2;
	} else {
	  pdb->currentCA->nextCA = tmp2;
	  pdb->currentCA = tmp2;
	}
      }
    }
  }

  /* 最後にNULL挿入 */
  
  return 0;
}

int __pdbRead(char fileName[], PDB *pdb){
  FILE *fpdb;

  if((fpdb=fopen(fileName, "r"))==NULL){
    fprintf(stderr, "fopen error!\n");
    return 0;
  }

  /* PDBの初期化 */
  pdbRead(fpdb, pdb);
  lpdbCenterCalc(pdb);
  lpdbSizeCalc(pdb);
  lpdbMinCalc(pdb);
  lpdbMaxCalc(pdb);
  lpdbResCount(pdb);
  lpdbEleCount(pdb);
  lpdbMinTemp(pdb);
  lpdbMaxTemp(pdb);
  lpdbMinOccu(pdb);
  lpdbMaxOccu(pdb);
  fclose(fpdb);
  
  return 1;
}
