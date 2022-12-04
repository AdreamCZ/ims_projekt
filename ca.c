#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 100 // Size of window
#define HEIGHT 100
#define FRICTION 30 // probability that sand will not fall into an empty cell underneath, when surrounded by sand on both sides ( in percents )

typedef enum { empty, sand, water , wall } TYPE;

typedef struct cell{
    TYPE type;
    bool isUnderPressure;
}Cell;

Cell buf1[HEIGHT][WIDTH];
Cell buf2[HEIGHT][WIDTH];

int frame = 0;

int printMessage = 0;

int grainCount = 0;

void ProcessSand(int x, int y){
    if( y+1 < HEIGHT ) // can fall
    {
        if(buf1[y+1][x].type == empty){ // Falling straight down
            if(x+1 >= WIDTH || buf1[y][x+1].type != sand ){
                buf2[y][x].type = empty;
                buf2[y+1][x].type = sand;
            }else if(x-1 < 0 || buf1[y][x-1].type != sand){
                buf2[y][x].type = empty;
                buf2[y+1][x].type = sand;
            }else if(x-1 >= 0 && x+1 < WIDTH && buf1[y][x-1].type == sand && buf1[y][x+1].type == sand){ // Friction on both sides
                if(rand()%100 > FRICTION){
                    buf2[y][x].type = empty;
                    buf2[y+1][x].type = sand;
                }
            }
        }else if(buf1[y+1][x].type == sand || buf1[y+1][x].type == wall){ // Tumbling sideways
            bool rightPossible = false;
            bool leftPossible = false;
            if(x-1 >= 0 && buf1[y][x-1].type == empty && buf1[y+1][x-1].type == empty){
                leftPossible = true;
            }
            if(leftPossible && x-2 >=0 && buf1[y][x-2].type == sand && (buf1[y+1][x-2].type == sand || buf1[y+1][x-2].type == wall)){
                // Another grain can tumble to the same spot on the left
                leftPossible = false;
            }
            if(x+1 < WIDTH && buf1[y][x+1].type == empty && buf1[y+1][x+1].type == empty){
                rightPossible = true;
            }
            if(rightPossible && x+2 < WIDTH && buf1[y][x+2].type == sand && (buf1[y+1][x+2].type == sand || buf1[y+1][x+2].type == wall)){
                // Another grain can tumble to the same spot on the right
                rightPossible = false;
            }

            if(rightPossible && leftPossible){
                //rand
                if(rand()%100 <= 50){
                    buf2[y][x].type = empty;
                    buf2[y+1][x+1].type = sand;
                }else{
                    buf2[y][x].type = empty;
                    buf2[y+1][x-1].type = sand;
                }
            }else if(rightPossible){
                buf2[y][x].type = empty;
                buf2[y+1][x+1].type = sand;
            }else if(leftPossible){
                buf2[y][x].type = empty;
                buf2[y+1][x-1].type = sand;
            }
        }
    }
}


void Process(){
   // printf("Frame %d \n",frame);
    //memset(buf2, 0, sizeof(buf2))
    grainCount=0;
    memcpy(buf2,buf1,sizeof(buf1));
    for(int x = 0; x < WIDTH; x++){
        for(int y = 0; y < HEIGHT; y++){
            switch(buf1[y][x].type){
                case sand:
                    ProcessSand(x,y);
                    grainCount++;
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

void Draw(){
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

void PlatformsBuild(){
    for(int i = 0; i < WIDTH; i++){
        buf1[HEIGHT - 1][i].type = wall;
        if( i < WIDTH / 3){
            buf1[HEIGHT/2][i].type = wall;
        }
        if (i < WIDTH / 2){
            buf1[2* (HEIGHT/3)][i].type = wall;

        }
        if( WIDTH-(WIDTH/5) < i && i < WIDTH){
            buf1[2 * (HEIGHT / 3)][i].type = wall;
        }
    }

    buf1[2 * (HEIGHT / 3) - 2 ][WIDTH-(WIDTH/5)].type = wall;
    buf1[2 * (HEIGHT / 3) - 3 ][WIDTH-(WIDTH/5)].type = wall;
    buf1[2 * (HEIGHT / 3) - 4 ][WIDTH-(WIDTH/5)].type = wall;
}

void PlatformsTick(){
    if(frame%2 == 0){
        if(frame < 700)
            buf1[0][WIDTH/4].type = sand;
        if(frame > 400 & frame < 800)
            buf1[0][WIDTH-(WIDTH/6)].type = sand;
    }
    /*
    if(frame % 1 == 0){
        if(frame < 80){
            buf1[0][WIDTH - 5].type = water;
            buf1[0][45].type = water;
        }
        if(frame <200)
            buf1[0][WIDTH / 2].type = water;
    }
    */
}

void HourglassBuild(){
    int y = 0;
    int x = 0;
    for(; x <= WIDTH/2 - (WIDTH/10) && y <= HEIGHT/2; x++){
        buf1[y-1][x].type = wall;
        buf1[y][x].type = wall;
        buf1[HEIGHT - y-1][x].type = wall;
        buf1[HEIGHT - y][x].type = wall;
        // right hand side :
        buf1[y-1][WIDTH - x].type = wall;
        buf1[y][WIDTH - x].type = wall;
        buf1[HEIGHT - y-1][WIDTH - x].type = wall;
        buf1[HEIGHT - y][WIDTH - x].type = wall;
        y++;
    }
    // middle tunnel
    int endOfTunel = HEIGHT - y; //47 47
    int startOfTunel = y;
    for(;y < endOfTunel; y++){
        buf1[y][x].type = wall;
        buf1[y][WIDTH-x].type = wall;
    }
    // top and bottom
    for(int x = 0; x < WIDTH; x++){
        buf1[0][x].type = wall;
        buf1[HEIGHT-1][x].type = wall;
    }
    //sand
    int xOffset = 2;
    for(int y = 1; y < startOfTunel; y++){
        for(int x = xOffset; x < WIDTH - xOffset; x++){
            buf1[y][x].type = sand;
        }
        xOffset++;
    }

}

void HourglassTick(){

}
/*
void TestBuild(){
    buf1[HEIGHT-1][0].type = sand;
    buf1[HEIGHT-2][0].type = sand;
    buf1[HEIGHT-2][1].type = wall;
}
*/
void TestBuild(){
    for(int y = 10; y <=30; y++){
        for(int x = 10; x <= 30; x++){
            buf1[y][x].type = sand;
        }
    }
}

void GlassesBuild(){
    for(int x = WIDTH/2 - WIDTH/6; x < WIDTH/2 + WIDTH/6; x++ ){
        buf1[HEIGHT - HEIGHT/4][x].type = wall;
    }
    for(int y = HEIGHT - HEIGHT/4 - 2; y > HEIGHT - HEIGHT/2; y--){
        buf1[y][WIDTH/2 - WIDTH/6 - 1].type = wall;
        buf1[y][WIDTH/2 + WIDTH/6].type = wall;
    }
    for(int x = WIDTH/2 - WIDTH/3; x < WIDTH/2 + WIDTH/3; x++){
        buf1[HEIGHT - HEIGHT/7][x].type = wall;
    }
}

void GlassesTick(){
    buf1[0][WIDTH/2 - 1].type = sand;
    buf1[0][WIDTH/2 ].type = sand;
    buf1[0][WIDTH/2 + 1].type = sand;
}

typedef enum {platforms, hourglass, test, glasses} worldtype;

int main(int argc, char **argv){

    worldtype type = hourglass;

    if(argc > 1){
        if(strcmp(argv[1], "hourglass") == 0){
            type = hourglass;
        }else if(strcmp(argv[1],"test") == 0){
            type = test;
        }else if(strcmp(argv[1],"platforms") == 0){
            type = platforms;
        }else if(strcmp(argv[1],"glasses") == 0){
            type = glasses;
        }
    }

    switch(type){
        case platforms:
            PlatformsBuild();
        break;
        case hourglass:
            HourglassBuild();
        break;
        case test:
            TestBuild();
        break;
        case glasses:
            GlassesBuild();
        break;
    }


    while(1){

        switch(type){
            break;
            case platforms:
                PlatformsTick();
            break;
            case hourglass:
                HourglassTick();
            break;
            case glasses:
                GlassesTick();
            break;
        }

        frame++;
        Draw();
        usleep(40000);
        Process();
        if(1){
            printf("\n%d\n",grainCount);
            //printMessage = 0;
        }
    }
    return 0;
}