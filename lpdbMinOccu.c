#include<stdio.h>
#include"PDB.h"
#include "config.h"

void lpdbMinOccu(PDB* pdb){

  pdb->minOccu = 1000000;
  
  for(pdb->currentCA=pdb->topCA; pdb->currentCA!=NULL; pdb->currentCA=pdb->currentCA->nextCA){
    if(pdb->currentCA->atom.occupancy < pdb->minOccu)
      pdb->minOccu = pdb->currentCA->atom.occupancy;
  }
}
