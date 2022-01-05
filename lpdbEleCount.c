#include <stdio.h>
#include <string.h>
#include "PDB.h"
#include "config.h"

void lpdbEleCount(PDB* pdb){
  int i;
  char tmp[6][3]={"C ","H ","N ","O ","P ","X "};


  /* elementkindの初期化 */
  pdb->kind.eleNum = 6;
  for(i=0; i<pdb->kind.eleNum; i++){
    strcpy(pdb->kind.ele[i].name, tmp[i]);
    pdb->kind.ele[i].name[2] = '\0';
    pdb->kind.ele[i].count = 0;
  }
  
  /*ここですべての原子をカウント。*/
  for(pdb->current=pdb->top;pdb->current!=NULL;pdb->current=pdb->current->nextAtom){
    for(i=0; i<pdb->kind.eleNum; i++){
      // その他
      if(i == pdb->kind.eleNum - 1){
	pdb->kind.ele[pdb->kind.eleNum-1].count++;
	break;
      }
      // 発見
      if(strcmp(pdb->kind.ele[i].name, pdb->current->atom.element)==0 || strncmp(pdb->kind.ele[i].name, pdb->current->atom.atomName, 2)==0){
	pdb->kind.ele[i].count++;
	break;
      }
    }
  }
}
