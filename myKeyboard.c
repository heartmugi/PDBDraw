#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "./myGLUT.h"
#include "./PDB.h"
#include "./config.h"

// キーボードイベントに関するマウスの位置
static int keyx = 0;
static int keyy = 0;

void
myKeyboard(unsigned char key,  /* キーボードから打ち込まれた文字 */
	   int x,         /* キーボードが打ち込まれたときのマウスの位置 */ 
	   int y)
	
{
#ifdef KEYBOARD_FUNCTION 
  keyx = x;
  keyy = y;	
  myKeyboard0(key, x, y);
#endif	
  /*
    再描画イベントを起こすための関数
    glutDisplayFuncで登録された関数がコールバックされる
  */
  glutPostRedisplay();	
}


void
myKeyboard0(unsigned char key,  /* キーボードから打ち込まれた文字 */
	    int x,          /* キーボードが打ち込まれたときのマウスの位置 */ 
	    int y)
	
{
  switch(key) {
  case 'q': {
    fprintf(stdout, "終了\n");
    exit(EXIT_FAILURE);
    break;
  }
  case 'x': {
    myCameraRotateX(1.0/M_PI);
    operationModeSet(xrotation);			
    break;
  }
  case 'y': {
    myCameraRotateY(1.0/M_PI);
    operationModeSet(yrotation);			
    break;
  }
  case 'z': {
    myCameraRotateZ(1.0/M_PI);
    operationModeSet(zrotation);			
    break;
  }
  case '1': {
    displayFlag = 1;
    fprintf(stdout, "分子モデル： Space-fillモデル\n");
    break;
  }
  case '2': {
    displayFlag = 2;
    fprintf(stdout, "分子モデル： Backboneモデル(アミノ酸)\n");
    break;
  }
  case '3': {
    displayFlag = 3;
    fprintf(stdout, "分子モデル： Backboneモデル(温度因子)\n");
    break;
  }
  case '4': {
    displayFlag = 4;
    fprintf(stdout, "分子モデル： Backboneモデル(N末端からC末端へ青から赤)\n");
    break;
  }
  case '5': {
    displayFlag = 5;
    fprintf(stdout, "分子モデル： Backboneモデル(占有度)\n");
    break;
  }
  case '6': {
    displayFlag = 6;
    fprintf(stdout, "分子モデル： Backboneモデル(重心からの距離)\n");
    break;
  }
  case 's': {
    animationFlag = animationFlag ^ 1;
    if(animationFlag) fprintf(stdout, "Animation: 再生\n");
    else fprintf(stdout, "Animation: 停止\n");
    break;
  }
  case 'b': {
    myBack();
    break;
  }
  case 'r': {
    myCameraSet(pdb);
    myLightReset();
    //glutPostRedisplay();
    myDisplay();
    break;
  }
  case 'f': {
    myDisplayListChange(); 
    break;
  }
  case 'p': {
    int mode;
    fprintf(stdout, "******************* Mode Lists ****************\n");
    fprintf(stdout, "\
  1: 中心座標\n\
  2: 最大半径\n\
  4: 最小座標\n\
  8: 最大座標\n\
 16: 残基の数リスト\n\
 32: 元素の数リスト\n\
 64: 最小温度因子\n\
128: 最大温度因子\n\
ex) 3(1|2)の時は中心座標と最大半径が表示される\n\
");
    fprintf(stdout, "***********************************************\n");
    fprintf(stdout, "mode: ");
    scanf("%d", &mode);
    lpdbInfoPrint(pdb, mode);
    break;
  }
  case 'd': {
    if(shadingFlag==1){
      glShadeModel(GL_FLAT);
      fprintf(stdout, "Shading Model: フラットシェーディング\n");
    }  else {
      glShadeModel(GL_SMOOTH);
      fprintf(stdout, "Shading Model: スムーズシェーディング\n");
    }
    shadingFlag = shadingFlag ^ 1;
    break;
  }
  case 'h': {
    myHelp();
    break;
  }
  case 'j': {
    if(projectionFlag == 1)
      fprintf(stdout, "Projection Mode: 正射影\n");
    else
      fprintf(stdout, "Projection Mode: 透視投影\n");
    projectionFlag = projectionFlag ^ 1;
    break;
  }
  case 'v': {
    float in;
    fprintf(stdout, "Input velocity value(0.1<=v<=10.0): ");
    scanf("%f", &in);
    if(0.1 <= in && in <= 10.0) velocity = in;
    else fprintf(stdout, "Error: out of range\n");
    break;
  }
  case 'u': {
    changeMyRadius();
    break;
  }
  default: { /* 定義されていないキーが入力された時には、何もしない。 */
    break;
  }
  }
}
