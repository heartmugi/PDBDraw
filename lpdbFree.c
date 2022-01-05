#include <stdio.h>
#include <stdlib.h>
#include "./PDB.h"

void lpdbFree(PDB* pdb){

  /* ATOMを解放 */
  for(pdb->current=pdb->top; pdb->current!=NULL; ){
    recordPDB* tmp;
    pdb->current = pdb->current->nextAtom;
    tmp = pdb->current;
    free(tmp);
  }

  /* CAを解放 */
  for(pdb->currentCA=pdb->topCA; pdb->currentCA!=NULL; ){
    recordPDB* tmp;
    pdb->currentCA = pdb->currentCA->nextCA;
    tmp = pdb->currentCA;
    free(tmp);
  }

}
