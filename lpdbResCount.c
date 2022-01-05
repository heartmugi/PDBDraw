#include <stdio.h>
#include <string.h>
#include"PDB.h"
#include "config.h"

void lpdbResCount(PDB* pdb){
  int i;
  char tmp[21][4]={"GLY","ALA","SER","THR","ASN","GLN","ASP","GLU","LYS","ARG","HIS","VAL","LEU","ILE","TYR","PHE","TRP","PRO","MET","CYS","OTH"};

  /* kindの初期化 */
  pdb->kind.resNum = 21;
  for(i=0; i<pdb->kind.resNum; i++){
    strcpy(pdb->kind.res[i].name, tmp[i]);
    pdb->kind.res[i].count = 0;
  }
  
  /*ここですべての原子をカウント。*/
  for(pdb->current=pdb->top;pdb->current!=NULL;pdb->current=pdb->current->nextAtom){
    for(i=0; i<pdb->kind.resNum; i++){
      if(i == pdb->kind.resNum - 1){
	pdb->kind.res[pdb->kind.resNum-1].count++;
	break;
      }
      if(strcmp(pdb->kind.res[i].name, pdb->current->atom.resName)==0){
	  pdb->kind.res[i].count++;
	  break;
      }
    }
  }
}
