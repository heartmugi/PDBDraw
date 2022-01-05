#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "./myGLUT.h"
#include "./PDB.h"
#include "config.h"
 
#define N 9
 
static GLuint    displayList1          = 1;
static GLuint    displayList2          = 2;
static GLuint    displayList3          = 3;
static GLuint    displayList4          = 4;
static GLuint    displayList5          = 5;
static GLuint    displayList6          = 6;
static GLboolean needDisplayListMaking = GL_TRUE;
  
static char elements[N][4]={"C ","H ","N ","O ","P ","S ","Na+ ","Mg ","X "};
static GLfloat colors[N][3] = {{0.5,0.5,0.5},{1.0,1.0,1.0},{0.0,0.0,1.0},{1.0,0.0,0.0},{1.0,0.5,0.0},{1.0,1.0,0.0},{0.0,0.0,0.5},{0.0,0.5,0.0},{0.0,1.0,0.0}};
static float radiuses[N] = {1.7, 1.2, 1.55, 1.52, 1.8, 1.8, 1.2, 1.73, 1.3};

static char resNames[21][4] = {"GLY","ALA","SER","THR","ASN","GLN","ASP","GLU","LYS","ARG","HIS","VAL","LEU","ILE","TYR","PHE","TRP","PRO","MET","CYS","OTH"};
static GLfloat resColors[21][3] = {{0.5,0.5,0.5},{0.5,1.0,0.0},{0.5,1.0,0.0},{0.5,1.0,0.0},{0.5,1.0,0.0},{0.5,1.0,0.0},{0.5,1.0,0.0},{1.0,0.0,0.0},{1.0,0.0,0.0},{0.0,0.0,1.0},{0.0,0.0,1.0},{0.0,0.0,1.0},{0.5,0.5,0.5},{0.5,0.5,0.5},{0.5,0.5,0.5},{0.5,1.0,0.0},{0.5,0.5,0.5},{0.5,0.5,0.5},{0.5,0.5,0.5},{0.5,1.0,0.0},{0.0,0.0,0.0}};
 
static GLfloat materialColor[4];
static GLfloat materialSpecular[4] = {0.2, 0.2, 0.2, 1.0 } ;
static GLfloat materialAmbient[4] = {0.1, 0.1, 0.1, 1.0 } ;

/*
  色の設定のための関数: indexがminからmaxに変化するにつれて、赤から青に変化する。
  青から赤に変化させるためには、プログラムの変更が必要である
  もしくは、
  index/max/minの使い方の工夫が必要である
*/
void
__getColorRGB(GLfloat color[3], GLdouble index, GLdouble min, GLdouble max)
{
  GLdouble half = (max+min)/2.0;

  // 青から赤
  if(index<half) {
    color[0] = 0;
    color[1] = (index - min  )/(half-min);
    color[2] = (half  - index)/(half-min);
  } else {
    color[0] = (index - half )/(max - half);
    color[1] = (max   - index)/(max - half);
    color[2] = 0;
  }
  
  /* 赤から青
  if(index<half) {
    color[0] = (half  - index)/(half-min);
    color[1] = (index - min  )/(half-min);
    color[2] = 0;
  } else {
    color[0] = 0;
    color[1] = (max   - index)/(max - half);
    color[2] = (index - half )/(max - half);
  }
  */
}

void __getColorRGB2d(GLfloat color[3], char resName[4]){
  int i;
 
  for(i=0; i<21; i++){
    if(strncmp(resName, resNames[i], 3)==0){
      color[0] = resColors[i][0];
      color[1] = resColors[i][1];
      color[2] = resColors[i][2];
      break;
    }
  }
}
 
/*
  多数のプリミティブの集合をひとつの命令で実行できるようにする。
  高速化のために必須。	
*/

/* Space-fillモデル */
void
myDisplayListMake1(PDB* pdb, GLuint displayList)
{
  GLdouble radius;	        /* 作成する球面の半径 */	
  GLint    slices=10;             /* 球面を分割するためのパラメータ */
  GLint    stacks=10;
  int colorNumber, i;
 
  glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular); /* 物質の鏡面反射の色の設定 */
  glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);     /* 物質の拡散光の色の設定  */
  glMaterialf(GL_FRONT, GL_SHININESS, 10.0);             /* 物質の鏡面指数 */
  glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);   /* 物質の環境光の色の設定 */
  materialColor[3] = 1.0;
 
  glNewList(displayList, GL_COMPILE);
  for(pdb->current=pdb->top; pdb->current!=NULL; pdb->current=pdb->current->nextAtom){
    /* 色の設定 */
    // 半径・色の取得
    for(i=0; i<N; i++){
      /* その他 */
      if(i == N-1){
	colorNumber = i;
	radius = radiuses[i];
	break;
      }
      /* 原子発見 */
      // Na+
      if(i == 6){
	if(strncmp(pdb->current->atom.atomName, elements[i], 4)==0){
	  colorNumber = i;
	  radius = radiuses[i];
	  break;
	}
      }
      // Mg
      if(i == 7){
	if(strncmp(pdb->current->atom.atomName, elements[i], 3)==0){
	  colorNumber = i;
	  radius = radiuses[i];
	  break;
	}
      }
      // その他
      else {
	if(strncmp(pdb->current->atom.element, elements[i], 2)==0 || strncmp(pdb->current->atom.atomName, elements[i], 2)==0){
	  colorNumber = i;
	  radius = radiuses[i];
	  break;
	}
      }
    }
    if(radiusFlag == 0) radius = 0.5;

    glColor3f(colors[colorNumber][0], colors[colorNumber][1], colors[colorNumber][2]);              /* 照明がないときのための色 */
    materialColor[0] = colors[colorNumber][0];	materialColor[1] = colors[colorNumber][1];	materialColor[2] = colors[colorNumber][2];	
    materialColor[3] = 1.0;
    glMaterialfv(GL_FRONT, GL_SPECULAR, materialSpecular); /* 物質の鏡面反射の色の設定 */
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColor);     /* 物質の拡散光の色の設定  */
    glMaterialf(GL_FRONT, GL_SHININESS, 10.0);             /* 物質の鏡面指数 */
    glMaterialfv(GL_FRONT, GL_AMBIENT, materialAmbient);   /* 物質の環境光の色の設定 */
	
    /* 球面の創出 */
    glPushMatrix();
    //glScalef (1.0, 1.0, 1.0);                /* x, y, z軸に沿って、拡大縮小 */ 
    //glRotatef(0.0, 0.0, 0.0, 1.0);           /* z軸の回りに回転 */
    //glTranslatef(__points[i].x, __points[i].y, __points[i].z); /* 移動 */
    glTranslatef(pdb->current->atom.x,pdb->current->atom.y, pdb->current->atom.z); /* 移動 */
    glutSolidSphere(radius, slices, stacks); 
    /* 位置(0,0,0)に半径radiusで球面を作成。刻幅slices/stacks 
       その後、上記のMatrixを使って、変換する。
    */
    glPopMatrix();
  }
  glEndList();
}

/* Backboneモデル(アミノ酸) */
void
myDisplayListMake2(PDB* pdb, GLuint displayList)
{
  GLfloat color[3]; /* 使用する色の指定 */
 
  materialColor[3] = 1.0;
 
  glNewList(displayList, GL_COMPILE);
 
  glPushMatrix();
  glBegin(GL_LINE_STRIP);
  /*  プリミティブ(初歩的な図形) を描き始める。
      GL_POINTS		個々の独立した点
      GL_LINES		個々の独立した線
      GL_POLYGON		単純な凸多角形の境界線
      GL_TRIANGLES 		個々の独立した三角形
      GL_QUADS		個々の独立した四角形
      GL_LINE_STRIP 		連結した線分
      GL_LINE_LOOP		GL_LINE_STRIP + 最初と最後の点を閉じる
      GL_TRIANGLE_STRIP	連結した三角形
      GL_TRIANGLE_FAN		扇型に連結した三角形
      GL_QUAD_STRIP		連結した四角形
  */
  pdb->currentCA = pdb->topCA;
  // 色の設定
  __getColorRGB2d(color, pdb->currentCA->atom.resName);
  glColor3f(color[0], color[1], color[2]); /* 照明を考えないときの色 */	
  // 頂点の設定
  glVertex3f(pdb->currentCA->atom.x, pdb->currentCA->atom.y, pdb->currentCA->atom.z);
  for(pdb->currentCA=pdb->topCA->nextCA; pdb->currentCA->nextCA->nextCA!=NULL; pdb->currentCA=pdb->currentCA->nextCA) {
    __getColorRGB2d(color, pdb->currentCA->atom.resName);
    glColor3f(color[0], color[1], color[2]);	
    glVertex3f((pdb->currentCA->nextCA->atom.x + pdb->currentCA->atom.x)/2.0,
	       (pdb->currentCA->nextCA->atom.y + pdb->currentCA->atom.y)/2.0,
	       (pdb->currentCA->nextCA->atom.z + pdb->currentCA->atom.z)/2.0);
 
    __getColorRGB2d(color, pdb->currentCA->atom.resName);
    glColor3f(color[0], color[1], color[2]);	
    glVertex3f(pdb->currentCA->nextCA->atom.x,
	       pdb->currentCA->nextCA->atom.y,
	       pdb->currentCA->nextCA->atom.z);
  }
  __getColorRGB2d(color, pdb->currentCA->atom.resName);
  glColor3f(color[0], color[1], color[2]);	
  glVertex3f((pdb->currentCA->nextCA->atom.x + pdb->currentCA->atom.x)/2.0,
	     (pdb->currentCA->nextCA->atom.y + pdb->currentCA->atom.y)/2.0,
	     (pdb->currentCA->nextCA->atom.z + pdb->currentCA->atom.z)/2.0);
  glVertex3f(pdb->currentCA->nextCA->atom.x,
	     pdb->currentCA->nextCA->atom.y,
	     pdb->currentCA->nextCA->atom.z);
 
  glEnd();
  glPopMatrix();
  glEndList();
 
}

/* BackBoneモデル 温度因子 */
void
myDisplayListMake3(PDB* pdb, GLuint displayList)
{
  int i;
  GLfloat color[3]; /* 使用する色の指定 */
 
  materialColor[3] = 1.0;
 
  glNewList(displayList, GL_COMPILE);
 
  glPushMatrix();
  glBegin(GL_LINE_STRIP);
  /*  プリミティブ(初歩的な図形) を描き始める。
      GL_POINTS		個々の独立した点
      GL_LINES		個々の独立した線
      GL_POLYGON		単純な凸多角形の境界線
      GL_TRIANGLES 		個々の独立した三角形
      GL_QUADS		個々の独立した四角形
      GL_LINE_STRIP 		連結した線分
      GL_LINE_LOOP		GL_LINE_STRIP + 最初と最後の点を閉じる
      GL_TRIANGLE_STRIP	連結した三角形
      GL_TRIANGLE_FAN		扇型に連結した三角形
      GL_QUAD_STRIP		連結した四角形
  */
  pdb->currentCA = pdb->topCA;
  // 色の設定
  __getColorRGB(color, pdb->currentCA->atom.tempFactor, pdb->minTemp, pdb->maxTemp);
  glColor3f(color[0], color[1], color[2]); /* 照明を考えないときの色 */	
  // 頂点の設定
  glVertex3f(pdb->currentCA->atom.x, pdb->currentCA->atom.y, pdb->currentCA->atom.z);
  for(pdb->currentCA=pdb->topCA->nextCA; pdb->currentCA->nextCA->nextCA!=NULL; pdb->currentCA=pdb->currentCA->nextCA) {
    __getColorRGB(color, pdb->currentCA->atom.tempFactor, pdb->minTemp, pdb->maxTemp);
    glColor3f(color[0], color[1], color[2]);	
    glVertex3f((pdb->currentCA->nextCA->atom.x + pdb->currentCA->atom.x)/2.0,
	       (pdb->currentCA->nextCA->atom.y + pdb->currentCA->atom.y)/2.0,
	       (pdb->currentCA->nextCA->atom.z + pdb->currentCA->atom.z)/2.0);
 
    __getColorRGB(color, pdb->currentCA->atom.tempFactor, pdb->minTemp, pdb->maxTemp);
    glColor3f(color[0], color[1], color[2]);	
    glVertex3f(pdb->currentCA->nextCA->atom.x,
	       pdb->currentCA->nextCA->atom.y,
	       pdb->currentCA->nextCA->atom.z);
    i++;
  }
  __getColorRGB(color, pdb->currentCA->atom.tempFactor, pdb->minTemp, pdb->maxTemp);
  glColor3f(color[0], color[1], color[2]);	
  glVertex3f((pdb->currentCA->nextCA->atom.x + pdb->currentCA->atom.x)/2.0,
	     (pdb->currentCA->nextCA->atom.y + pdb->currentCA->atom.y)/2.0,
	     (pdb->currentCA->nextCA->atom.z + pdb->currentCA->atom.z)/2.0);
  glVertex3f(pdb->currentCA->nextCA->atom.x,
	     pdb->currentCA->nextCA->atom.y,
	     pdb->currentCA->nextCA->atom.z);
 
  glEnd();
  glPopMatrix();
  glEndList();
}


/* Backboneモデル(N=>C 青=>赤) */
void
myDisplayListMake4(PDB* pdb, GLuint displayList)
{
  int i;
  GLfloat color[3]; /* 使用する色の指定 */
 
  materialColor[3] = 1.0;
 
  glNewList(displayList, GL_COMPILE);
 
  glPushMatrix();
  glBegin(GL_LINE_STRIP);
  i = 0;
  pdb->currentCA = pdb->topCA;
  // 色の設定
  __getColorRGB(color, i, 0, pdb->numCA-1);
  glColor3f(color[0], color[1], color[2]); /* 照明を考えないときの色 */	
  // 頂点の設定
  glVertex3f(pdb->currentCA->atom.x, pdb->currentCA->atom.y, pdb->currentCA->atom.z);
  i++;
  for(pdb->currentCA=pdb->topCA->nextCA; pdb->currentCA->nextCA->nextCA!=NULL; pdb->currentCA=pdb->currentCA->nextCA) {
    __getColorRGB(color, i-0.5, 0, pdb->numCA-1); 
    glColor3f(color[0], color[1], color[2]);	
    glVertex3f((pdb->currentCA->nextCA->atom.x + pdb->currentCA->atom.x)/2.0,
	       (pdb->currentCA->nextCA->atom.y + pdb->currentCA->atom.y)/2.0,
	       (pdb->currentCA->nextCA->atom.z + pdb->currentCA->atom.z)/2.0);
 
    __getColorRGB(color, i, 0, pdb->numCA-1);
    glColor3f(color[0], color[1], color[2]);	
    glVertex3f(pdb->currentCA->nextCA->atom.x,
	       pdb->currentCA->nextCA->atom.y,
	       pdb->currentCA->nextCA->atom.z);
    i++;
  }
  __getColorRGB(color, ++i, 0, pdb->numCA-1);
  glColor3f(color[0], color[1], color[2]);	
  glVertex3f((pdb->currentCA->nextCA->atom.x + pdb->currentCA->atom.x)/2.0,
	     (pdb->currentCA->nextCA->atom.y + pdb->currentCA->atom.y)/2.0,
	     (pdb->currentCA->nextCA->atom.z + pdb->currentCA->atom.z)/2.0);
  glVertex3f(pdb->currentCA->nextCA->atom.x,
	     pdb->currentCA->nextCA->atom.y,
	     pdb->currentCA->nextCA->atom.z);
 
  glEnd();
  glPopMatrix();
  glEndList();
 
}

/* Backboneモデル(占有度) */
void
myDisplayListMake5(PDB* pdb, GLuint displayList)
{
  GLfloat color[3]; /* 使用する色の指定 */
 
  materialColor[3] = 1.0;
 
  glNewList(displayList, GL_COMPILE);
 
  glPushMatrix();
  glBegin(GL_LINE_STRIP);
  pdb->currentCA = pdb->topCA;
  // 色の設定
  __getColorRGB(color, pdb->currentCA->atom.occupancy, pdb->minOccu, pdb->maxOccu);
  glColor3f(color[0], color[1], color[2]); /* 照明を考えないときの色 */	
  // 頂点の設定
  glVertex3f(pdb->currentCA->atom.x, pdb->currentCA->atom.y, pdb->currentCA->atom.z);
  for(pdb->currentCA=pdb->topCA->nextCA; pdb->currentCA->nextCA->nextCA!=NULL; pdb->currentCA=pdb->currentCA->nextCA) {
    __getColorRGB(color, pdb->currentCA->atom.occupancy, pdb->minOccu, pdb->maxOccu);
    glColor3f(color[0], color[1], color[2]);	
    glVertex3f((pdb->currentCA->nextCA->atom.x + pdb->currentCA->atom.x)/2.0,
	       (pdb->currentCA->nextCA->atom.y + pdb->currentCA->atom.y)/2.0,
	       (pdb->currentCA->nextCA->atom.z + pdb->currentCA->atom.z)/2.0);
 
    __getColorRGB(color, pdb->currentCA->atom.occupancy, pdb->minOccu, pdb->maxOccu);
    glColor3f(color[0], color[1], color[2]);	
    glVertex3f(pdb->currentCA->nextCA->atom.x,
	       pdb->currentCA->nextCA->atom.y,
	       pdb->currentCA->nextCA->atom.z);
  }
  __getColorRGB(color, pdb->currentCA->atom.occupancy, pdb->minOccu, pdb->maxOccu);
  glColor3f(color[0], color[1], color[2]);	
  glVertex3f((pdb->currentCA->nextCA->atom.x + pdb->currentCA->atom.x)/2.0,
	     (pdb->currentCA->nextCA->atom.y + pdb->currentCA->atom.y)/2.0,
	     (pdb->currentCA->nextCA->atom.z + pdb->currentCA->atom.z)/2.0);
  glVertex3f(pdb->currentCA->nextCA->atom.x,
	     pdb->currentCA->nextCA->atom.y,
	     pdb->currentCA->nextCA->atom.z);
 
  glEnd();
  glPopMatrix();
  glEndList();
}

/* Backboneモデル(重心からの距離) */
void
myDisplayListMake6(PDB* pdb, GLuint displayList)
{
  GLfloat color[3]; /* 使用する色の指定 */
  GLfloat d;
 
  materialColor[3] = 1.0;
 
  glNewList(displayList, GL_COMPILE);
 
  glPushMatrix();
  glBegin(GL_LINE_STRIP);
  pdb->currentCA = pdb->topCA;
  // 色の設定
  d = sqrt(pow(pdb->Center.x - pdb->currentCA->atom.x,2)+pow(pdb->Center.y - pdb->currentCA->atom.y,2)+pow(pdb->Center.z - pdb->currentCA->atom.z,2));
  __getColorRGB(color, d, 0, pdb->maxRadiusCA);
  glColor3f(color[0], color[1], color[2]); /* 照明を考えないときの色 */	
  // 頂点の設定
  glVertex3f(pdb->currentCA->atom.x, pdb->currentCA->atom.y, pdb->currentCA->atom.z);
  for(pdb->currentCA=pdb->topCA->nextCA; pdb->currentCA->nextCA->nextCA!=NULL; pdb->currentCA=pdb->currentCA->nextCA) {
    d = sqrt(pow(pdb->Center.x - pdb->currentCA->atom.x,2)+pow(pdb->Center.y - pdb->currentCA->atom.y,2)+pow(pdb->Center.z - pdb->currentCA->atom.z,2));
  __getColorRGB(color, d, 0, pdb->maxRadiusCA);
    glColor3f(color[0], color[1], color[2]);	
    glVertex3f((pdb->currentCA->nextCA->atom.x + pdb->currentCA->atom.x)/2.0,
	       (pdb->currentCA->nextCA->atom.y + pdb->currentCA->atom.y)/2.0,
	       (pdb->currentCA->nextCA->atom.z + pdb->currentCA->atom.z)/2.0);
 
    d = sqrt(pow(pdb->Center.x - pdb->currentCA->atom.x,2)+pow(pdb->Center.y - pdb->currentCA->atom.y,2)+pow(pdb->Center.z - pdb->currentCA->atom.z,2));
  __getColorRGB(color, d, 0, pdb->maxRadiusCA);
    glColor3f(color[0], color[1], color[2]);	
    glVertex3f(pdb->currentCA->nextCA->atom.x,
	       pdb->currentCA->nextCA->atom.y,
	       pdb->currentCA->nextCA->atom.z);
  }
  d = sqrt(pow(pdb->Center.x - pdb->currentCA->atom.x,2)+pow(pdb->Center.y - pdb->currentCA->atom.y,2)+pow(pdb->Center.z - pdb->currentCA->atom.z,2));
  __getColorRGB(color, d, 0, pdb->maxRadiusCA);
  glColor3f(color[0], color[1], color[2]);	
  glVertex3f((pdb->currentCA->nextCA->atom.x + pdb->currentCA->atom.x)/2.0,
	     (pdb->currentCA->nextCA->atom.y + pdb->currentCA->atom.y)/2.0,
	     (pdb->currentCA->nextCA->atom.z + pdb->currentCA->atom.z)/2.0);
  glVertex3f(pdb->currentCA->nextCA->atom.x,
	     pdb->currentCA->nextCA->atom.y,
	     pdb->currentCA->nextCA->atom.z);
 
  glEnd();
  glPopMatrix();
  glEndList();
}

void 
myDisplayListMake(void)
{
  myDisplayListMake1(pdb, displayList1);
  myDisplayListMake2(pdb, displayList2);
  myDisplayListMake3(pdb, displayList3);
  myDisplayListMake4(pdb, displayList4);
  myDisplayListMake5(pdb, displayList5);
  myDisplayListMake6(pdb, displayList6);
  needDisplayListMaking = GL_FALSE;
}
 
/*
  Displayコールバックにより呼び出される関数。
  実際にDisplayを行う。	
*/
void
myDisplay(void)
{
  cameraInfo* camera;
 
  /* カメラ位置の設定 */
  camera = getMyCamera();
  glMatrixMode(GL_PROJECTION);              /* 内部で取り扱う行列のモードの切替え: 
					       GL_PROJECTION : 射影変換行列  
					       GL_MODELVIEW  : モデルビュー行列  */  
  glLoadIdentity();                         /* 単位行列の読み込み */  
  myLightSet();
 
  //　カメラの視線の指定
  if(projectionFlag == 1){
    gluPerspective(camera->Fovy, camera->Aspect, camera->ClipNear, camera->ClipFar);
  } else {
    glOrtho(-length*tan(camera->Fovy/180/2.0*M_PI)*camera->Aspect,
	    +length*tan(camera->Fovy/180/2.0*M_PI)*camera->Aspect,
	    -length*tan(camera->Fovy/180/2.0*M_PI),
	    +length*tan(camera->Fovy/180/2.0*M_PI),
	    camera->ClipNear, camera->ClipFar);
  }
  gluLookAt(camera->Eye[0], camera->Eye[1], camera->Eye[2],
	    camera->Centre[0], camera->Centre[1], camera->Centre[2],
	    camera->Up[0], camera->Up[1], camera->Up[2]);
  //myLightSet();
 
  /* GLUを使った視線 
     対象を眺めるカメラ(目)の位置     : eyex, eyey, eyez	
     シーンの中心となる注視点   : centrex, cetrey, centrez 
     シーンの上方を表すベクトル : upx, upy, upz
  */
  /* GLUを使った透視変換: 
     y-z平面における視界の角度 : 0<Fovy<180.0
     アスペクト比: Aspect = 視界幅/視界高
     クリップ面と視点の距離: ClipNear(>0), ClipFar(>0)
  */  
  // 正射影変換の場合
  // 正射影変換の定義: left, right, bottom, top, near, far */
  /* glOrtho(-length*tan(camera->Fovy/180/2.0*M_PI)*camera->Aspect,
     +length*tan(camera->Fovy/180/2.0*M_PI)*camera->Aspect,
     -length*tan(camera->Fovy/180/2.0*M_PI),
     +length*tan(camera->Fovy/180/2.0*M_PI),
     camera->ClipNear, camera->ClipFar);    */
  //
  // 透視変換の場合
  // glFrustum(-1.0, 1.0, -1.0, 1.0, 0.5, 1.0);   /* 透視変換の定義 left, right, bottom, top, near(>0), far(>0) */
  //       あるいは
  // gluPerspective(camera->Fovy, camera->Aspect, camera->ClipNear, camera->ClipFar);   
  /* GLUを使った透視変換: 
     y-z平面における視界の角度 : 0<Fovy<180.0
     アスペクト比: Aspect = 視界幅/視界高
     クリップ面と視点の距離: ClipNear(>0), ClipFar(>0)
  */  
 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();                         /* 単位行列の読み込み */  
  //myLightSet();
 
  // バッファ領域のクリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
  // 高速化のためにディスプレイリストを使用する。
  if(GL_TRUE == needDisplayListMaking) {
    myDisplayListMake();
  }
  glPushMatrix();
  {
    glRotated(theta, 0.0f, 1.0f, 0.0f);
    switch(displayFlag){
    case 1: {
      glPushMatrix();
      //myLightSet();
      glCallList(displayList1); /* ディスプレイリスト1を取り出す */
      glPopMatrix();
      break;
    }
    case 2: {
      glPushMatrix();
      glDisable(GL_LIGHTING);
      glCallList(displayList2); /* ディスプレイリスト2を取り出す */
      glEnable(GL_LIGHTING);
      glPopMatrix();
      break;
    }
    case 3: {
      glPushMatrix();
      glDisable(GL_LIGHTING);
      glCallList(displayList3); /* ディスプレイリスト2を取り出す */
      glEnable(GL_LIGHTING);
      glPopMatrix();
      break;
    }
    case 4: {
     glPushMatrix();
      glDisable(GL_LIGHTING);
      glCallList(displayList4); /* ディスプレイリスト4を取り出す */
      glEnable(GL_LIGHTING);
      glPopMatrix();
      break;
    }
    case 5: {
     glPushMatrix();
      glDisable(GL_LIGHTING);
      glCallList(displayList5); /* ディスプレイリスト5を取り出す */
      glEnable(GL_LIGHTING);
      glPopMatrix();
      break;
    }
    case 6: {
     glPushMatrix();
      glDisable(GL_LIGHTING);
      glCallList(displayList6); /* ディスプレイリスト6を取り出す */
      glEnable(GL_LIGHTING);
      glPopMatrix();
      break;
    }
    }
  }
  glutSwapBuffers();
}

/*
#pragma INTERRUPT showFileLists
void showFileLists(void){
  myFileLists();
}
*/

void myDisplayListChange(void){
  char fileName[100];

  myFileLists();
  fprintf(stdout, "File Name: ");
  scanf("%s", fileName);
  lpdbFree(pdb);
  if(__pdbRead(fileName, pdb)){
    needDisplayListMaking = GL_TRUE;
    /*
    glDeleteLists(displayList1, 1);
    glDeleteLists(displayList2, 1);
    glDeleteLists(displayList3, 1);
    glDeleteLists(displayList4, 1);
    glDeleteLists(displayList5, 1);
    glDeleteLists(displayList6, 1);
    */
    myCameraSet(pdb);
    myLightReset();
    //myInit();
    fprintf(stdout, "changed PDB\n");
  }
}

void changeMyRadius(void){
  radiusFlag = radiusFlag ^ 1;
  if(radiusFlag) fprintf(stdout, "Radius: VdW半径\n");
  else           fprintf(stdout, "Radius: All 0.5\n");
  needDisplayListMaking = GL_TRUE;
}
