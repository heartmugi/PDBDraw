#include <stdio.h>
#include "PDB.h"
#include "config.h"

void lpdbMaxOccu(PDB* pdb){

  pdb->maxOccu = -1000000;
  
  for(pdb->currentCA=pdb->topCA; pdb->currentCA!=NULL; pdb->currentCA=pdb->currentCA->nextCA){
    if(pdb->currentCA->atom.occupancy > pdb->maxOccu)
      pdb->maxOccu = pdb->currentCA->atom.occupancy;
  }
}
