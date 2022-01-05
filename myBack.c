#include <stdio.h>
#include "./myTest.h"
#include "./myGLUT.h"
#include "config.h"

#define N 10

char cLists[N][30] = {"カスタム","黒","赤","青","緑","ピンク","水色","黄緑","オレンジ","白"};
float rgb[N][3] = {{0,0,0},{0.2,0.2,0.2},{1.0,0.0,0.0},{0.0,0.0,1.0},{0.0,1.0,0.0},{1.0,0.5,1.0},{0.5,1.0,1.0},{0.5,1.0,0.5},{1.0,0.5,0.0},{1.0,1.0,1.0}};

void showColorLists(void){
  int i;
  
  fprintf(stdout, "*********** Back Color Lists **********\n");
  for(i=0; i<N; i++){
    fprintf(stdout, "%d: %s\n", i, cLists[i]);
  }
  fprintf(stdout, "***************************************\n");
}

int decideColor(void){
  int c;
  
  showColorLists();
  fprintf(stdout, "Color Index: ");
  scanf("%d", &c);
  
  return c;
}

void setColor(int c){
  if(c == 0){
    float r, g, b, a;
    fprintf(stdout, "Input R,G,B,A (0.00 ≦ val <= 1.00): ");
    scanf("%f %f %f %f", &r,&g,&b,&a);
    glClearColor(r, g, b, a);
  } else {
    glClearColor(rgb[c][0], rgb[c][1], rgb[c][2], 0.0);
  }
}

void myBack(void){
  setColor(decideColor());
}
