#include <stdio.h>
#include <unistd.h>
#include <string.h>

#define WIDTH 30
#define HEIGHT 30

typedef enum { empty, sand, wall } TYPE;

typedef struct cell{
    TYPE type; 
}Cell;

Cell buf1[HEIGHT][WIDTH];
Cell buf2[HEIGHT][WIDTH];

int frame = 0;

void process(){
   // printf("Frame %d \n",frame);
    memcpy(buf2,buf1,sizeof(buf1));
    for(int x = 0; x < WIDTH; x++){
        for(int y = 0; y < HEIGHT; y++){
            Cell currCell = buf1[y][x];
            if(currCell.type == sand){
                if(buf1[y+1][x].type == empty){
                   // printf("fall y%d x%d type%d \n",y,x,currCell.type);
                    buf2[y][x].type = empty;
                    buf2[y+1][x].type = sand;
                }else if(buf1[y+1][x-1].type == empty){
                    currCell.type = empty;
                    buf2[y+1][x-1].type = sand;
                }
                else if(buf1[y+1][x+1].type == empty){
                    currCell.type = empty;
                    buf2[y+1][x+1].type = sand;
                }
            }
        }
    }
    memcpy(buf1,buf2,sizeof(buf1));

}

void draw(){
    printf("\033[0;0H\033[2J"); // Clear terminal
    for(int y = 0; y < WIDTH; y++){
        for(int x = 0; x < HEIGHT; x++){
            Cell currCell = buf1[y][x];
            
            if( currCell.type == sand ){
                printf("o");
            }else if(currCell.type == wall){
                printf("#");
            }else{
                printf(".");
            }
        }
        printf("\n");
    }
}

int main(int argc, char **argv){
    for(int i = 0; i < WIDTH; i++){
        buf1[HEIGHT - 1][i].type = wall;
        if( i < WIDTH / 3){
            buf1[HEIGHT/2][i].type = wall;
        }
        if (i < WIDTH / 2){
                buf1[2* (HEIGHT/3)][i].type = wall;

        }
    }
    buf1[0][(int)WIDTH / 2].type = sand;
    while(1){
        if(frame%3 == 0){
                buf1[0][(int)WIDTH / 2 - 5].type = sand;
        }
        frame++;
        draw();
        usleep(50000);
        process();
    }
    return 0;
}