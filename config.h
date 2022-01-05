#ifndef CONFIG
#define CONFIG
#include <stdio.h>
#include "./PDB.h"
#include "./myGLUT.h"
/* グローバル変数 */
extern PDB* pdb;
extern PDB* pdb2;
extern int animationFlag;  // アニメーションさせるかどうかに関するフラグ変数。外部変数として宣言、１でアニメーション、０でストップ
extern  int displayFlag;   // どの表示方法を選択するかに関するフラグ変数。外部変数として宣言。１，２で画像表示方法を変更する

// 物体の回転のための変数
extern int     turn;
extern float   theta;
extern float velocity;   // 正負も許可。マウスにより、速度を制御不可
extern float alpha;      // 比例係数
     
// カメラの回転のための変数
extern float   camera_yaw; // Ｙ軸を中心とする回転角度
extern float   camera_pitch; // Ｘ軸を中心とする回転角度
extern float   camera_roll; // Ｘ軸を中心とする回転角度
extern float   camera_distance; // 中心からカメラの距離

// シェーディング
extern int shadingFlag;

// 射影
extern int projectionFlag;
extern float length;

// 半径フラグ
extern int radiusFlag;

/* プロトタイプ宣言 */
//extern GLboolean* getNeedDisplayListMaking();
extern void myDisplayListChange(void);
extern void myCameraSet(PDB* pdb);
extern void myFileLists(void);
extern void myBack(void);
extern void myHelp(void);
extern void myLightReset();
extern void changeMyRadius();

#endif
