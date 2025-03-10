#include"bird.h"

bird ::Bird(double _x,double_y,SDL_Texture *bird){
    x=_x;
    y=_y;
    speed=0;
    gravity=1;
    jumpstrength=-0.5;
    texture=bird;
}
void bird::falling(){
    speed+=gravity;
    y+=speed;
}
