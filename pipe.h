#ifndef PIPE_H
#define PIPE_H

#include<SDL.h>
#include<SDL_image.h>
#include<vector>
struct pipe{
    double x,y;
    double width,height;
    SDL_Texture *texture;
    SDL_Rect pipeRect;
    bool passed;
    // Cập nhật vị trí ống nước
    void update(int speed);

    // Vẽ ống nước
    void render(SDL_Renderer* renderer)const ;

    // Kiểm tra nếu pipe ra khỏi màn hình
    bool isOffScreen()const ;
    pipe(int x, int y, SDL_Texture* texture);
};
// Hàm tạo ống nước mới và thêm vào danh sách

#endif // PIPE_H
