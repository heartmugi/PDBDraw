#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include "./config.h"

DIR *dir;
struct dirent *dp;
char dirpath[] = "./";

int getFileLists(void){
  dir = opendir(dirpath);
  if (dir == NULL) { return 0; }
  return 1;
}

void printFileLists(void){
  dp = readdir(dir);
  fprintf(stdout, "*********** PDB File Lists ***********\n");
  while (dp != NULL){
    char *ext  = strrchr(dp->d_name, '.');
    if(ext!=NULL && strcmp(ext, ".pdb")==0)
      printf("%s\n", dp->d_name);
    dp = readdir(dir);
  }
  fprintf(stdout, "**************************************\n");

  if (dir != NULL) { closedir(dir); }
}

void myFileLists(void){
  if(getFileLists()){
    printFileLists();
  } else {
    fprintf(stdout, "getFileLists error!\n");
  }
} 
