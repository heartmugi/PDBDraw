#include<math.h>
#include "./myGLUT.h"
#include "./PDB.h"
#include "./config.h"

// マウスイベントのコールバック関数間でやりとりするための変数
// ファイルに関する静的変数なので、このファイルの中だけで有効
static int drag_mouse_l = 0;
static int drag_mouse_r = 0;
static int drag_mouse_m = 0;
static int last_mouse_x = 0;
static int last_mouse_y = 0;
static int pressedButton = 1;

void
myMouse(int button, int state, int x, int y)
{
#ifdef MOUSE_FUNCTION
  // 左ボタンが押されたらドラッグ開始
  if ( ( button == GLUT_LEFT_BUTTON ) && ( state == GLUT_DOWN ) )
    drag_mouse_l = 1;
  // 左ボタンが離されたらドラッグ終了
  if ( ( button == GLUT_LEFT_BUTTON ) && ( state == GLUT_UP ) ) 
    drag_mouse_l = 0;

  // 右ボタンが押されたらドラッグ開始
  if ( ( button == GLUT_RIGHT_BUTTON ) && ( state == GLUT_DOWN ) )
    drag_mouse_r = 1;
  // 右ボタンが離されたらドラッグ終了
  else if ( ( button == GLUT_RIGHT_BUTTON ) && ( state == GLUT_UP ) ) 
    drag_mouse_r = 0;

  // 中ボタンが押されたらドラッグ開始
  if ( ( button == GLUT_MIDDLE_BUTTON ) && ( state == GLUT_DOWN ) )
    drag_mouse_m = 1;
  // 中ボタンが離されたらドラッグ終了
  else if ( ( button == GLUT_MIDDLE_BUTTON ) && ( state == GLUT_UP ) )
    drag_mouse_m = 0;

  // シフトキー開始
  if ((glutGetModifiers()&GLUT_ACTIVE_SHIFT ))
    pressedButton = 1;
  // シフトキー終了
  if (!(glutGetModifiers()&GLUT_ACTIVE_SHIFT ) )
    pressedButton = 0;
  
  last_mouse_x = x;
  last_mouse_y = y;			

#endif	
  // 再描画イベント
  glutPostRedisplay();
}


void
myMouseMotion(int x, int y)
{
#ifdef MOTION_FUNCTION
  GLdouble delx, dely;

  delx = (x - last_mouse_x)*alpha;
  dely = (y - last_mouse_y)*alpha;
  
  if(drag_mouse_l==1 && pressedButton==0){
    myCameraRotateX(dely);
    myCameraRotateY(delx);
  }
  if(drag_mouse_r==1 && pressedButton==0){
    myCameraTranslateX(-delx*10);
    myCameraTranslateY(dely*10);
  }
  if(drag_mouse_l & pressedButton){
    myCameraTranslateZ(dely);
  }
  if(drag_mouse_r & pressedButton){
    myCameraRotateZ(delx);
  }
  last_mouse_x = x;
  last_mouse_y = y;
#endif
  // 再描画イベント
  glutPostRedisplay();
}


void
myMousePassiveMotion(int x, int y)
{
#ifdef PASSIVEMOTION_FUNCTION
#endif
  // 再描画イベント
  glutPostRedisplay();
}
