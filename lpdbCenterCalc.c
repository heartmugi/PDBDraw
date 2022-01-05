#include<stdio.h>
#include "PDB.h"
#include "config.h"

void lpdbCenterCalc(PDB* pdb){

  float sumx, sumy, sumz;
  sumx = sumy = sumz = 0.0;
  
  for(pdb->current=pdb->top; pdb->current!=NULL; pdb->current=pdb->current->nextAtom){
    sumx += pdb->current->atom.x;
    sumy += pdb->current->atom.y;
    sumz += pdb->current->atom.z;
  }

  pdb->Center.x = sumx / pdb->numAtom;
  pdb->Center.y = sumy / pdb->numAtom;
  pdb->Center.z = sumz / pdb->numAtom;
}
