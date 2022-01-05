#include <stdio.h>
#include "PDB.h"
#include "config.h"

void lpdbMaxTemp(PDB* pdb){

  pdb->maxTemp = -1000000;
  
  for(pdb->currentCA=pdb->topCA; pdb->currentCA!=NULL; pdb->currentCA=pdb->currentCA->nextCA){
    if(pdb->currentCA->atom.tempFactor > pdb->maxTemp)
      pdb->maxTemp = pdb->currentCA->atom.tempFactor;
  }
}
