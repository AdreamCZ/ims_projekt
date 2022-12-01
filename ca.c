#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define WIDTH 30
#define HEIGHT 30

typedef enum { empty, sand, water , wall } TYPE;

typedef struct cell{
    TYPE type;
}Cell;

Cell buf1[HEIGHT][WIDTH];
Cell buf2[HEIGHT][WIDTH];

int frame = 0;

int printMessage = 0;

void process(){
   // printf("Frame %d \n",frame);
    memcpy(buf2,buf1,sizeof(buf1));
    for(int x = 0; x < WIDTH; x++){
        for(int y = 0; y < HEIGHT; y++){
            Cell currCell = buf1[y][x];
            switch(currCell.type){
                case sand:
                    if( y+1 < HEIGHT ){
                        if(buf1[y+1][x].type == empty){
                        // printf("fall y%d x%d type%d \n",y,x,currCell.type);
                            buf2[y][x].type = empty;
                            buf2[y+1][x].type = sand;
                        }else if( buf1[y+1][x-1].type == empty && x-1 >= 0){
                            buf2[y][x].type = empty;
                            buf2[y+1][x-1].type = sand;
                        }
                        else if(buf1[y+1][x+1].type == empty && x+1 < WIDTH){
                            buf2[y][x].type = empty;
                            buf2[y+1][x+1].type = sand;
                        }
                    }
                break;
                case water:
                    if(buf1[y+1][x].type == empty){
                        buf2[y][x].type = empty;
                        buf2[y+1][x].type = water;
                        if(frame > 75)
                            printMessage = 1;
                    }
                    else if( buf1[y+1][x-1].type == empty && x-1 >= 0){
                        buf2[y][x].type = empty;
                        buf2[y+1][x-1].type = water;
                        printMessage = 1;
                    }
                    else if(buf1[y+1][x+1].type == empty && x+1 < WIDTH){
                        buf2[y][x].type = empty;
                        buf2[y+1][x+1].type = water;
                        printMessage = 1;


                    }else if(buf1[y][x-1].type == empty && x-1 >= 0){
                        buf2[y][x].type = empty;
                        buf2[y][x-1].type = water;

                    }else if(buf1[y][x+1].type == empty && x+1 < WIDTH){
                        buf2[y][x].type = empty;
                        buf2[y][x+1].type = water;
                    }
                break;
            }

        }
    }
    memcpy(buf1,buf2,sizeof(buf1));

}

void draw(){
    printf("\E[H\E[2J"); // Clear termimnal
    for(int y = 0; y < WIDTH; y++){
        for(int x = 0; x < HEIGHT; x++){
            Cell currCell = buf1[y][x];

            if( currCell.type == sand ){
                printf("O");
            }else if(currCell.type == wall){
                printf("#");
            }else if(currCell.type == water){
                printf("X");
            }else{
                printf(".");
            }
        }
        printf("\t");
        for(int x = 0; x < HEIGHT; x++){
            Cell currCell = buf2[y][x];

            if( currCell.type == sand ){
                printf("O");
            }else if(currCell.type == wall){
                printf("#");
            }else if(currCell.type == water){
                printf("X");
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
 //   buf1[0][(int)WIDTH / 2].type = sand;
    while(1){
        if(frame%3 == 0 && frame < 19){
          //  buf1[0][3].type = sand;
        }
        if(frame % 1 == 0 && 30 < frame && frame < 45 ){
            buf1[0][WIDTH - 5].type = water;
        }
        frame++;
        draw();
        usleep(50000);
        process();
        if(printMessage == 1){
            printf("\n went left \n");
            //printMessage = 0;
        }
    }
    return 0;
}