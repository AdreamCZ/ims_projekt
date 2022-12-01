#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define WIDTH 60
#define HEIGHT 60

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
    //memset(buf2, 0, sizeof(buf2))
    memcpy(buf2,buf1,sizeof(buf1));
    for(int x = 0; x < WIDTH; x++){
        for(int y = 0; y < HEIGHT; y++){
            Cell currCell = buf1[y][x];
            switch(currCell.type){
                case sand:
                    if( y+1 < HEIGHT ){
                        if(buf2[y+1][x].type == empty){
                        // printf("fall y%d x%d type%d \n",y,x,currCell.type);
                            buf2[y][x].type = empty;
                            buf2[y+1][x].type = sand;
                        }else if( buf2[y+1][x-1].type == empty && x-1 >= 0){
                            buf2[y][x].type = empty;
                            buf2[y+1][x-1].type = sand;
                        }
                        else if(buf2[y+1][x+1].type == empty && x+1 < WIDTH){
                            buf2[y][x].type = empty;
                            buf2[y+1][x+1].type = sand;
                        }else if(buf2[y+1][x].type == water){  // Interactions with water
                            buf2[y][x].type = water;
                            buf2[y+1][x].type = sand;
                        }else if( x-1 >= 0 && buf2[y+1][x-1].type == water  ){
                            if(buf2[y][x-1].type == empty){
                                buf2[y][x-1].type = water;
                            }else{
                                buf2[y][x].type = water;
                            }
                            buf2[y+1][x-1].type = sand;
                        }
                        else if( x+1 < WIDTH && buf2[y+1][x+1].type == water ){
                            if(buf2[y][x+1].type == empty){
                                buf2[y][x+1].type = water;
                            }else{
                                buf2[y][x].type = water;
                            }
                            buf2[y+1][x+1].type = sand;
                        }                            
                    }
                break;
                case water:
                    if(buf2[y+1][x].type == empty){
                        buf2[y][x].type = empty;
                        buf2[y+1][x].type = water;
                        if(frame > 75)
                            printMessage = 1;
                    }
                    else if( x-1 >= 0 && buf2[y+1][x-1].type == empty){
                        buf2[y][x].type = empty;
                        buf2[y+1][x-1].type = water;
                        if(frame > 75)
                            printMessage = 1;
                    }
                    else if( x+1 < WIDTH && buf2[y+1][x+1].type == empty  ){
                        buf2[y][x].type = empty;
                        buf2[y+1][x+1].type = water;
                        if(frame > 75)
                            printMessage = 1;
                    }else if( x-1 >= 0 && buf2[y][x-1].type == empty ){
                        buf2[y][x].type = empty;
                        buf2[y][x-1].type = water;

                    }else if(x+1 < WIDTH && buf2[y][x+1].type == empty ){
                        buf2[y][x].type = empty;
                        buf2[y][x+1].type = water;
                    }
                break;
                case wall:
                    buf2[y][x].type = wall;
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
                printf("\e[43m ");
            }else if(currCell.type == wall){
                printf("\e[47m ");
            }else if(currCell.type == water){
                printf("\e[44m ");
            }else{
                printf("\e[40m ");
            }
        }
        /*
        printf("\t");
        for(int x = 0; x < HEIGHT; x++){
            Cell currCell = buf2[y][x];

            if( currCell.type == sand ){
                printf("\e[0;33mO");
            }else if(currCell.type == wall){
                printf("\e[0;37m#");
            }else if(currCell.type == water){
                printf("\e[0;34mX");
            }else{
                printf("\e[0;30.");
            }
        }*/
        printf("\e[40m\n");
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
        if( 40 < i && i < 70){
            buf1[2 * (HEIGHT / 3)][i].type = wall;
        }
    }
    
    buf1[2 * (HEIGHT / 3) - 1 ][41].type = wall;
    buf1[2 * (HEIGHT / 3) - 2 ][41].type = wall;

 //   buf1[0][(int)WIDTH / 2].type = sand;
    while(1){
        
        if(frame%2 == 0){
            if(frame > 30 && frame < 500)
                buf1[0][10].type = sand;
            if(frame > 400 & frame < 600)
                buf1[0][45].type = sand;    
        }
        if(frame % 1 == 0){
            if(frame < 80){
                buf1[0][WIDTH - 5].type = water;
                buf1[0][45].type = water;
            }
            if(frame <200)
                buf1[0][WIDTH / 2].type = water;
        }
        
        
        frame++;
        draw();
        usleep(30000);
        process();
        if(printMessage == 1){
            printf("\n went left \n");
            //printMessage = 0;
        }
    }
    return 0;
}