#include<stdio.h>
#include"PDB.h"
#include "config.h"

void lpdbMinCalc(PDB* pdb){

  pdb->min.x = 10000;
  pdb->min.y = 10000;
  pdb->min.z = 10000;
  
  for(pdb->current=pdb->top; pdb->current!=NULL; pdb->current=pdb->current->nextAtom){
    if(pdb->min.x > pdb->current->atom.x){
      pdb->min.x = pdb->current->atom.x;
    }
    if(pdb->min.y > pdb->current->atom.y){
      pdb->min.y = pdb->current->atom.y;
    }
    if(pdb->min.z > pdb->current->atom.z){
      pdb->min.z = pdb->current->atom.z;
    }
  }
}
