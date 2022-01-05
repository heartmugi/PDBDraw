#include<stdio.h>
#include"PDB.h"
#include "config.h"

void lpdbMaxCalc(PDB* pdb){

  pdb->max.x=-100000;
  pdb->max.y=-100000;
  pdb->max.z=-100000;
  
  for(pdb->current=pdb->top;pdb->current!=NULL;pdb->current=pdb->current->nextAtom){
    if(pdb->max.x<pdb->current->atom.x){
      pdb->max.x=pdb->current->atom.x;
    }
    if(pdb->max.y<pdb->current->atom.y){
      pdb->max.y=pdb->current->atom.y;
    }
    if(pdb->max.z<pdb->current->atom.z){
      pdb->max.z=pdb->current->atom.z;
    }
  }
}
