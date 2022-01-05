#include <stdio.h>
#include <math.h>
#include "./myGLUT.h"
#include "./PDB.h"
#include "./config.h"

// このファイルの中だけで参照可能な静的変数
// カメラの現在位置を示している。 
static cameraInfo __camera = {
  40.0,             /* Fovy: 視野角 [degree]*/ 
  1.0,              /* Aspect: アスペクト比 */ 
  1.0,              /* ClipNear(目に近い側のクリッピング面) 目の位置からの奥行き(z軸負の向き)への距離 */
  10000000.0,           /* ClipFar(目から遠い側のクリッピング面) */
  {0.0, 0.0, 100.0}, /* Eye: 目の位置: (eyex, eyey, eyez) */
  {0.0, 0.0, 0.0},  /* Centre: 注視点: (centerx, centrey, centrez) */
  {0.0,1.0,0.0}	  /* Up:  目の上向き: (upx, upy, upz) */	
};

#define N 4

// 他のファイルからカメラの情報を変更できるようにポインタを引き渡す
cameraInfo*
getMyCamera()
{
  return &__camera;
}

// カメラの上向きを計算する。
void 
upCalc()
{
  GLdouble* up     = __camera.Up;
  GLdouble norm;

  int i;

  norm = up[0]*up[0] + up[1]*up[1] + up[2]*up[2];
  if(0 == norm) { 
    up[0] = 0;	
    up[1] = 1;	
    up[2] = 0;	
  } else {
    norm = sqrt(norm);
    for(i=0; i<3; i++) {
      up[i] /= norm;
    }
  }
}

//　画面のＸ軸の周りに回転させる
void
myCameraRotateX(GLdouble angle) 
{
  GLdouble  dummy[3];							
  GLdouble  norm[3];							
  GLfloat* light; 
  int i;
  double length;

  dummy[0] = __camera.Eye[0]-__camera.Centre[0];
  dummy[1] = __camera.Eye[1]-__camera.Centre[1];
  dummy[2] = __camera.Eye[2]-__camera.Centre[2];

  norm[0] = __camera.Up[1]*dummy[2] - dummy[1]*__camera.Up[2];	
  norm[1] = __camera.Up[2]*dummy[0] - dummy[2]*__camera.Up[0];	
  norm[2] = __camera.Up[0]*dummy[1] - dummy[0]*__camera.Up[1];	

  length = sqrt(norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2]);	
  norm[0] /= length;
  norm[1] /= length;
  norm[2] /= length;

  __camera.Eye[0] = __camera.Centre[0] 
    + (norm[0]*norm[0]*(1-cos(angle))+cos(angle))        *dummy[0]
    + (norm[0]*norm[1]*(1-cos(angle))-norm[2]*sin(angle))*dummy[1]
    + (norm[0]*norm[2]*(1-cos(angle))+norm[1]*sin(angle))*dummy[2];
  __camera.Eye[1] = __camera.Centre[1] 
    + (norm[1]*norm[0]*(1-cos(angle))+norm[2]*sin(angle))*dummy[0]
    + (norm[1]*norm[1]*(1-cos(angle))+cos(angle))        *dummy[1]
    + (norm[1]*norm[2]*(1-cos(angle))-norm[0]*sin(angle))*dummy[2];
  __camera.Eye[2] = __camera.Centre[2] 
    + (norm[2]*norm[0]*(1-cos(angle))-norm[1]*sin(angle))*dummy[0]
    + (norm[2]*norm[1]*(1-cos(angle))+norm[0]*sin(angle))*dummy[1]
    + (norm[2]*norm[2]*(1-cos(angle))+cos(angle))        *dummy[2];

  dummy[0] = __camera.Up[0];
  dummy[1] = __camera.Up[1];
  dummy[2] = __camera.Up[2];

  __camera.Up[0] =  (norm[0]*norm[0]*(1-cos(angle))+cos(angle))        *dummy[0]
    + (norm[0]*norm[1]*(1-cos(angle))-norm[2]*sin(angle))*dummy[1]
    + (norm[0]*norm[2]*(1-cos(angle))+norm[1]*sin(angle))*dummy[2];
  __camera.Up[1] =  (norm[1]*norm[0]*(1-cos(angle))+norm[2]*sin(angle))*dummy[0]
    + (norm[1]*norm[1]*(1-cos(angle))+cos(angle))        *dummy[1]
    + (norm[1]*norm[2]*(1-cos(angle))-norm[0]*sin(angle))*dummy[2];
  __camera.Up[2] =  (norm[2]*norm[0]*(1-cos(angle))-norm[1]*sin(angle))*dummy[0]
    + (norm[2]*norm[1]*(1-cos(angle))+norm[0]*sin(angle))*dummy[1]
    + (norm[2]*norm[2]*(1-cos(angle))+cos(angle))        *dummy[2];

  length = sqrt(__camera.Up[0]*__camera.Up[0]+__camera.Up[1]*__camera.Up[1]+__camera.Up[2]*__camera.Up[2]);	
  __camera.Up[0] /= length;
  __camera.Up[1] /= length;
  __camera.Up[2] /= length;

  if(lightControl) {
    for(i=0; i<2; i++) {
      light = getMyLightPos(i);	

      dummy[0] = light[0] - __camera.Centre[0];
      dummy[1] = light[1] - __camera.Centre[1];
      dummy[2] = light[2] - __camera.Centre[2];

      light[0] = __camera.Centre[0]
	+ (norm[0]*norm[0]*(1-cos(angle))+cos(angle))        *dummy[0]
	+ (norm[0]*norm[1]*(1-cos(angle))-norm[2]*sin(angle))*dummy[1]
	+ (norm[0]*norm[2]*(1-cos(angle))+norm[1]*sin(angle))*dummy[2];
      light[1] = __camera.Centre[1]
	+ (norm[1]*norm[0]*(1-cos(angle))+norm[2]*sin(angle))*dummy[0]
	+ (norm[1]*norm[1]*(1-cos(angle))+cos(angle))        *dummy[1]
	+ (norm[1]*norm[2]*(1-cos(angle))-norm[0]*sin(angle))*dummy[2];
      light[2] = __camera.Centre[2]
	+ (norm[2]*norm[0]*(1-cos(angle))-norm[1]*sin(angle))*dummy[0]
	+ (norm[2]*norm[1]*(1-cos(angle))+norm[0]*sin(angle))*dummy[1]
	+ (norm[2]*norm[2]*(1-cos(angle))+cos(angle))        *dummy[2];
    }
  }
  glFlush();
}

// 本来のＸ軸の周りに回転させる。
void
myCameraRotateXX(GLdouble angle) 
{
  GLdouble  dummy[3];							
  GLfloat* light; 
  int i;
  /*
    eyex     1   0      0  \  / EyeX \
    eyey =   0  cos   -sin |  | EyeY |
    eyez	 0  sin    cos /  \ EyeZ /
		
  */	

  dummy[1] = (__camera.Eye[1]-__camera.Centre[1])*cos(angle) - (__camera.Eye[2]-__camera.Centre[2])*sin(angle) ; 
  dummy[2] = (__camera.Eye[1]-__camera.Centre[1])*sin(angle) + (__camera.Eye[2]-__camera.Centre[2])*cos(angle) ; 

  __camera.Eye[1] = dummy[1] + __camera.Centre[1];
  __camera.Eye[2] = dummy[2] + __camera.Centre[2];


  __camera.Up[1] = __camera.Up[1]*cos(angle) - __camera.Up[2]*sin(angle) ; 
  __camera.Up[2] = __camera.Up[1]*sin(angle) + __camera.Up[2]*cos(angle) ; 

  upCalc();

  /* カメラ位置の回転 */
  if(lightControl) {
    for(i=0; i<2; i++) {
      light = getMyLightPos(i);	

      dummy[0] =  light[0];
      dummy[1] =  (light[1]-__camera.Centre[1])*cos(angle) - (light[2]-__camera.Centre[2])*sin(angle);
      dummy[2] =  (light[1]-__camera.Centre[1])*sin(angle) + (light[2]-__camera.Centre[2])*cos(angle);

      light[0] = dummy[0];
      light[1] = dummy[1]+__camera.Centre[1];
      light[2] = dummy[2]+__camera.Centre[2];
    }
  }
  glFlush();
}

void
myCameraRotateY(GLdouble angle) 
{
  GLdouble  dummy[3];							
  GLdouble  norm[3];							
  GLfloat* light; 
  int i;

  dummy[0] = __camera.Eye[0]-__camera.Centre[0];
  dummy[1] = __camera.Eye[1]-__camera.Centre[1];
  dummy[2] = __camera.Eye[2]-__camera.Centre[2];

  norm[0] = __camera.Up[0];	
  norm[1] = __camera.Up[1];	
  norm[2] = __camera.Up[2];	
  __camera.Eye[0] = __camera.Centre[0] 
    + (norm[0]*norm[0]*(1-cos(angle))+cos(angle))        *dummy[0]
    + (norm[0]*norm[1]*(1-cos(angle))-norm[2]*sin(angle))*dummy[1]
    + (norm[0]*norm[2]*(1-cos(angle))+norm[1]*sin(angle))*dummy[2];
  __camera.Eye[1] = __camera.Centre[1] 
    + (norm[1]*norm[0]*(1-cos(angle))+norm[2]*sin(angle))*dummy[0]
    + (norm[1]*norm[1]*(1-cos(angle))+cos(angle))        *dummy[1]
    + (norm[1]*norm[2]*(1-cos(angle))-norm[0]*sin(angle))*dummy[2];
  __camera.Eye[2] = __camera.Centre[2] 
    + (norm[2]*norm[0]*(1-cos(angle))-norm[1]*sin(angle))*dummy[0]
    + (norm[2]*norm[1]*(1-cos(angle))+norm[0]*sin(angle))*dummy[1]
    + (norm[2]*norm[2]*(1-cos(angle))+cos(angle))        *dummy[2];


  if(lightControl) {
    for(i=0; i<2; i++) {
      light = getMyLightPos(i);	

      dummy[0] = light[0] - __camera.Centre[0];
      dummy[1] = light[1] - __camera.Centre[1];
      dummy[2] = light[2] - __camera.Centre[2];

      light[0] = __camera.Centre[0]
	+ (norm[0]*norm[0]*(1-cos(angle))+cos(angle))        *dummy[0]
	+ (norm[0]*norm[1]*(1-cos(angle))-norm[2]*sin(angle))*dummy[1]
	+ (norm[0]*norm[2]*(1-cos(angle))+norm[1]*sin(angle))*dummy[2];
      light[1] = __camera.Centre[1]
	+ (norm[1]*norm[0]*(1-cos(angle))+norm[2]*sin(angle))*dummy[0]
	+ (norm[1]*norm[1]*(1-cos(angle))+cos(angle))        *dummy[1]
	+ (norm[1]*norm[2]*(1-cos(angle))-norm[0]*sin(angle))*dummy[2];
      light[2] = __camera.Centre[2]
	+ (norm[2]*norm[0]*(1-cos(angle))-norm[1]*sin(angle))*dummy[0]
	+ (norm[2]*norm[1]*(1-cos(angle))+norm[0]*sin(angle))*dummy[1]
	+ (norm[2]*norm[2]*(1-cos(angle))+cos(angle))        *dummy[2];
    }
  }
  glFlush();
}

void
myCameraRotateYY(GLdouble angle) 
{
  GLdouble  dummy[3];							
  GLfloat* light; 
  int i;
  /*
    eyex    cos  0   sin  \  / EyeX \
    eyey =   0   1    0    | | EyeY |
    eyez	-sin 0    cos /  \ EyeZ /
		
  */	
  dummy[0] =  (__camera.Eye[0]-__camera.Centre[0])*cos(angle) + (__camera.Eye[2]-__camera.Centre[2])*sin(angle) ; 
  dummy[2] = -(__camera.Eye[0]-__camera.Centre[0])*sin(angle) + (__camera.Eye[2]-__camera.Centre[2])*cos(angle) ; 

  __camera.Eye[0] = dummy[0] + __camera.Centre[0];
  __camera.Eye[2] = dummy[2] + __camera.Centre[2];

  __camera.Up[0] =  __camera.Up[0]*cos(angle) + __camera.Up[2]*sin(angle) ; 
  __camera.Up[2] = -__camera.Up[0]*sin(angle) + __camera.Up[2]*cos(angle) ; 

  upCalc();

	
  if(lightControl) {
    for(i=0; i<2; i++) {
      light = getMyLightPos(i);	

      dummy[0] =  (light[0]-__camera.Centre[0])*cos(angle) + (light[2]-__camera.Centre[2])*sin(angle);
      dummy[1] =  (light[1]-__camera.Centre[1]);
      dummy[2] = -(light[0]-__camera.Centre[0])*sin(angle) + (light[2]-__camera.Centre[2])*cos(angle);

      light[0] = dummy[0] + __camera.Centre[0];
      light[1] = dummy[1] + __camera.Centre[1];
      light[2] = dummy[2] + __camera.Centre[2];
    }
  }
  glFlush();
}

void
myCameraRotateZ(GLdouble angle) 
{
  GLdouble  dummy[3];							
  GLdouble  norm[3];							
  GLfloat* light; 
  int i;
  double length;

  norm[0] = __camera.Eye[0]-__camera.Centre[0];
  norm[1] = __camera.Eye[1]-__camera.Centre[1];
  norm[2] = __camera.Eye[2]-__camera.Centre[2];
  length = sqrt(norm[0]*norm[0]+norm[1]*norm[1]+norm[2]*norm[2]);	
  norm[0] /= length;
  norm[1] /= length;
  norm[2] /= length;

  dummy[0] = __camera.Up[0];
  dummy[1] = __camera.Up[1];
  dummy[2] = __camera.Up[2];

  __camera.Up[0] =  (norm[0]*norm[0]*(1-cos(angle))+cos(angle))        *dummy[0]
    + (norm[0]*norm[1]*(1-cos(angle))-norm[2]*sin(angle))*dummy[1]
    + (norm[0]*norm[2]*(1-cos(angle))+norm[1]*sin(angle))*dummy[2];
  __camera.Up[1] =  (norm[1]*norm[0]*(1-cos(angle))+norm[2]*sin(angle))*dummy[0]
    + (norm[1]*norm[1]*(1-cos(angle))+cos(angle))        *dummy[1]
    + (norm[1]*norm[2]*(1-cos(angle))-norm[0]*sin(angle))*dummy[2];
  __camera.Up[2] =  (norm[2]*norm[0]*(1-cos(angle))-norm[1]*sin(angle))*dummy[0]
    + (norm[2]*norm[1]*(1-cos(angle))+norm[0]*sin(angle))*dummy[1]
    + (norm[2]*norm[2]*(1-cos(angle))+cos(angle))        *dummy[2];

  length = sqrt(__camera.Up[0]*__camera.Up[0]+__camera.Up[1]*__camera.Up[1]+__camera.Up[2]*__camera.Up[2]);	
  __camera.Up[0] /= length;
  __camera.Up[1] /= length;
  __camera.Up[2] /= length;


  if(lightControl) {
    for(i=0; i<2; i++) {
      light = getMyLightPos(i);	

      dummy[0] = light[0] - __camera.Centre[0];
      dummy[1] = light[1] - __camera.Centre[1];
      dummy[2] = light[2] - __camera.Centre[2];

      light[0] = __camera.Centre[0]
	+ (norm[0]*norm[0]*(1-cos(angle))+cos(angle))        *dummy[0]
	+ (norm[0]*norm[1]*(1-cos(angle))-norm[2]*sin(angle))*dummy[1]
	+ (norm[0]*norm[2]*(1-cos(angle))+norm[1]*sin(angle))*dummy[2];
      light[1] = __camera.Centre[1]
	+ (norm[1]*norm[0]*(1-cos(angle))+norm[2]*sin(angle))*dummy[0]
	+ (norm[1]*norm[1]*(1-cos(angle))+cos(angle))        *dummy[1]
	+ (norm[1]*norm[2]*(1-cos(angle))-norm[0]*sin(angle))*dummy[2];
      light[2] = __camera.Centre[2]
	+ (norm[2]*norm[0]*(1-cos(angle))-norm[1]*sin(angle))*dummy[0]
	+ (norm[2]*norm[1]*(1-cos(angle))+norm[0]*sin(angle))*dummy[1]
	+ (norm[2]*norm[2]*(1-cos(angle))+cos(angle))        *dummy[2];
    }
  }
  glFlush();
}

// 行列の積
void matrix_mul(GLdouble c[][N], GLdouble a[][N], GLdouble b[][N])
{
  int i, j, k ;

  for(i=0;i<N;i++) {
    for(j=0;j<N;j++) {
      c[i][j] = 0.0 ;
      for(k=0;k<N;k++) c[i][j] += a[i][k]*b[k][j] ;
    }
  }
  return ;
}

//　カメラをＸ軸の方向に動かす。
void
myCameraTranslateX(GLdouble pan) 
{
  /*
    eyex    cos  0   sin  \  / EyeX \
    eyey =   0   1    0    | | EyeY |
    eyez    -sin 0    cos /  \ EyeZ /
		
  */
  //__camera.Centre[0] -= pan;
  int i;

  /* 方向ベクトルf */
  GLdouble F[3] = {__camera.Centre[0] - __camera.Eye[0], __camera.Centre[1] - __camera.Eye[1], __camera.Centre[2] - __camera.Eye[2]};
  GLdouble F_norm=0, f[3];
  pan *=10;
  for(i=0; i<3; i++){
    F_norm += pow(F[i],2);
  }
  F_norm = sqrt(F_norm);
  // 正規化
  for(i=0; i<3; i++){
    f[i] = F[i] / F_norm;
  }

  /* Uの正規化 */
  upCalc();
  GLdouble U_norm=0, u[3];
  for(i=0; i<3; i++){
    U_norm += pow(__camera.Up[i], 2);
  }
  U_norm = sqrt(U_norm);
  for(i=0; i<3; i++){
    u[i] = __camera.Up[i] / U_norm;
  }

  /* 正規化された外積 */
  GLdouble S[3] = {f[1]*u[2]-f[2]*u[1], f[2]*u[0]-f[0]*u[2], f[0]*u[1]-f[1]*u[0]};
  GLdouble S_norm=0, s[3];
  for(i=0; i<3; i++){
    S_norm += pow(S[i], 2);
  }
  S_norm = sqrt(S_norm);
  for(i=0; i<3; i++){
    s[i] = S[i] / S_norm;
  }

  /* 変化 */
  pan *= 0.1;
  for(i=0; i<3; i++){
    __camera.Centre[i] -= s[i]*pan;
  }

  glFlush();
}

//　カメラをY軸の方向に動かす。
void
myCameraTranslateY(GLdouble pan) 
{
  int i;
  
  /* Uの正規化 */
  upCalc();
  GLdouble U_norm=0, u[3];
  for(i=0; i<3; i++){
    U_norm += pow(__camera.Up[i], 2);
  }
  U_norm = sqrt(U_norm);
  for(i=0; i<3; i++){
    u[i] = __camera.Up[i] / U_norm;
  }

  /* 変化 */
  for(i=0; i<3; i++){
    __camera.Centre[i] -= u[i]*pan;
  }
  
  glFlush();
}

//　カメラをZ軸の方向に動かす。
void
myCameraTranslateZ(GLdouble pan) 
{
  int i;

  /* 方向ベクトル */
  GLdouble F[3] = {__camera.Centre[0] - __camera.Eye[0], __camera.Centre[1] - __camera.Eye[1], __camera.Centre[2] - __camera.Eye[2]};
  GLdouble F_norm=0, f[3];
  pan *=10;
  for(i=0; i<3; i++){
    F_norm += pow(F[i],2);
  }
  F_norm = sqrt(F_norm);
  // 正規化
  for(i=0; i<3; i++){
    f[i] = F[i] / F_norm;
  }

  /* 変化 */
  GLdouble tmpE[3], d = 0;
  for(i=0; i<3; i++){
    //tmpC[i] = __camera.Centre[i] - f[i]*pan;
    tmpE[i] = __camera.Eye[i] - f[i]*pan;
    d += pow(__camera.Centre[i] - tmpE[i], 2);
  }
  d = sqrt(d);
  if(d >= 50){
    for(i=0; i<3; i++){
      //__camera.Centre[i] = tmpC[i];
      __camera.Eye[i] = tmpE[i];
    }
  }

  /* 正射影 */
  float tmp;
  tmp = length + pan*2.5;
  if(tmp >= 40)
    length = tmp;

  glFlush();
}

void myCameraSet(PDB* pdb){
  __camera.ClipNear = 1.0;
  
  __camera.Eye[0] = pdb->Center.x;
  __camera.Eye[1] = pdb->Center.y;
  __camera.Eye[2] = length = pdb->Center.z + 3*pdb->maxRadius;

  __camera.Centre[0] = pdb->Center.x;
  __camera.Centre[1] = pdb->Center.y;
  __camera.Centre[2] = pdb->Center.z;

  __camera.Up[0] = 0.0;
  __camera.Up[1] = 1.0;
  __camera.Up[2] = 0.0;

  __camera.ClipFar = 10000000.0;
  //glutPostRedisplay();
}
