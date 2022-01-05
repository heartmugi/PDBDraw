#include "./myGLUT.h"
#include "./config.h"

#define THETA 30

void
myIdle(void)
{
#ifdef IDLE_FUNCTION
#endif
  float tmp;
  
  // 物体を回転(±30度)
  if(animationFlag){
    tmp = theta + animationFlag*turn*velocity;
    if(tmp >= THETA){
      theta = THETA;
      turn = -1;
    } else if(tmp <= -THETA) {
      theta = -THETA;
      turn = 1;
    } else {
      theta = tmp;
    }
    glutPostRedisplay();
  }
}
