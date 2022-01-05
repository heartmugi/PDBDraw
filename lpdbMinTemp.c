#include<stdio.h>
#include"PDB.h"
#include "config.h"

void lpdbMinTemp(PDB* pdb){

  pdb->minTemp = 1000000;
  
  for(pdb->currentCA=pdb->topCA; pdb->currentCA!=NULL; pdb->currentCA=pdb->currentCA->nextCA){
    if(pdb->currentCA->atom.tempFactor < pdb->minTemp)
      pdb->minTemp = pdb->currentCA->atom.tempFactor;
  }
}
