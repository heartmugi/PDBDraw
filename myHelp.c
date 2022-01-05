#include <stdio.h>
#include "./config.h"

void myHelp(void){
  fprintf(stdout, "******* Help: Keybord Comand Lists ******\n");
  fprintf(stdout, "\
q: 終了\n\
x: 1.0/piだけx軸回転\n\
y: 1.0/piだけy軸回転\n\
z: 1.0/piだけz軸回転\n\
1: Space-fillモデル\n\
2: Backboneモデル(アミノ酸)\n\
3: Backboneモデル(温度因子)\n\
4: Backboneモデル(N末端からC末端へ青から赤)\n\
5: Backboneモデル(占有度)\n\
6: Backboneモデル(重心からの距離)\n\
s: アニメーションの再生・停止\n\
b: 背景変更\n\
r: リセット\n\
f: PDBデータ変更\n\
p: PDBの情報を表示\n\
d: シェーディングモデルの変更\n\
v: アニメーション速度の変更\n\
u: 半径の変更\n\
");
  fprintf(stdout, "*****************************************\n\n");
  fprintf(stdout, "******* Help: Mouse Operation Lists *****\n");
  fprintf(stdout, "\
左クリック： 分子の回転\n\
右クリック： 分子の平行移動\n\
SHIFT + 左クリック： ズームアウト・イン\n\
SHIFT + 右クリック： z軸回転\n\
");
  fprintf(stdout, "*****************************************\n");
}
