#include <math.h>
#include "./myTest.h"
#include "./myGLUT.h"
#include "./PDB.h"
#include "./config.h"

void
myInit(void)
{
  GLfloat kougen[4] = {-20.0,-20.0,100.0,1.0};
  GLfloat kakusan[3] = {1.0,1.0,1.0};
  GLfloat hansya[3] = {0.25,0.25,0.25};

  glLightfv(GL_LIGHT0, GL_POSITION, kougen);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, kakusan);
  glLightfv(GL_LIGHT0, GL_AMBIENT, hansya);
  
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  myLightSet();

  glShadeModel(GL_SMOOTH); 
  /* シェーディングモデルを設定する： 
     GL_FLAT(フラットシェーディング) これは()内の記述内容
     or 
     GL_SMOOTH(スムーズシェーディング・グローシェーディング)
  */
  glDepthFunc(GL_LEQUAL);
  /*  デプスを計算するための比較関数の定義
      GL_LEQUAL: Less than or equal	
  */
  glEnable(GL_DEPTH_TEST); 
  /*  デプステストを有効にする。 */


  //glEnable(GL_CULL_FACE);     	// カリングを有効にする	
  //glEnable(GL_CW);             	// 時計回りに変更 
  //glEnable(GL_BACK);			// 裏側をカリング 86

  
  //glClearColor(backR, backG, backB, backA);
  glClearColor(0.2,0.2,0.2,0.0);
  /* 背景色の設定　*/

  /* __cameraの設定 */
  myCameraSet(pdb);
}
