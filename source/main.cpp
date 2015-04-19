/*Include files*/
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <iostream>
#include <sstream>

//Screen Width
#define ScreenWidth 600
//Screen Height
#define ScreenHeight 600
//Size of Mine Image
#define MineSize 20
//Number of Mines x
#define Mine_x ScreenHeight/MineSize
//Number of Mines y
#define Mine_y ScreenWidth/MineSize
//Number of mines
#define mine_count 99

/*Function declarations*/
//To update displaymap, recursive
int update(int x,int y);
//To generate new map
void createMines(void);
//initialize the maps
void initMaps(void);
//draw the map
void drawMap(void);

//global variables
int storagemap[Mine_x][Mine_y];
int displaymap[Mine_x][Mine_y];
//image blocks
ALLEGRO_BITMAP *blocks[11];
int totalus = 0;

int main(void)
{
    //DISPLAYS ERROR MESSAGE IF INITIALISATION OF ALLEGRO IS FAILED
    if(!al_init())
    {
        al_show_native_message_box(NULL,NULL,NULL,"ALLEGRO 5 CANNOT BE INITIALISED",NULL,ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }
    //TO CREATE GAME WINDOW
    al_set_new_display_flags(ALLEGRO_WINDOWED);

    //create allegro display
    ALLEGRO_DISPLAY *display = al_create_display(ScreenWidth , ScreenHeight);   //display the window
    if(al_show_native_message_box(display,"MINESWEEPER","Do you Want to start?","Let The Game Begin!!",NULL,ALLEGRO_MESSAGEBOX_OK_CANCEL))
    {
        //TO SET TITLE OF THE GAME WINDOW
        al_set_window_title(display,"MINESWEEPER");
        al_set_window_position(display, 384, 84);

        //TO SHOW ERROR MESSAGE IF GAME SCREEN IS NOT CREATED
        if(!display)
        {
            al_show_native_message_box(NULL,"MINESWEEPER","ËRROR","DISPLAY BE CANNOT BE CREATED",NULL,ALLEGRO_MESSAGEBOX_ERROR);
            return-1;
        }

        //INITIALIZE IMAGE ADDON
        al_init_image_addon();
        //INSTALL KEYBOARD AND MOUSE COMMANDS
        al_install_keyboard();
        al_install_mouse();

    }
    //initialize the maps, generate mines
    initMaps();
    createMines();

    //load all images
    blocks[10] = al_load_bitmap("C:/Users/mikail/Desktop/Mine/blankblock.png");
    blocks[1]  = al_load_bitmap("C:/Users/mikail/Desktop/Mine/block1.png");
    blocks[2]  = al_load_bitmap("C:/Users/mikail/Desktop/Mine/block2.png");
    blocks[3]  = al_load_bitmap("C:/Users/mikail/Desktop/Mine/block3.png");
    blocks[4]  = al_load_bitmap("C:/Users/mikail/Desktop/Mine/block4.png");
    blocks[5]  = al_load_bitmap("C:/Users/mikail/Desktop/Mine/block5.png");
    blocks[6]  = al_load_bitmap("C:/Users/mikail/Desktop/Mine/block6.png");
    blocks[7]  = al_load_bitmap("C:/Users/mikail/Desktop/Mine/block7.png");
    blocks[8]  = al_load_bitmap("C:/Users/mikail/Desktop/Mine/block8.png");
    blocks[9]  = al_load_bitmap("C:/Users/mikail/Desktop/Mine/blockmine.png");
    blocks[0]  = al_load_bitmap("C:/Users/mikail/Desktop/Mine/filledblock.png");

    drawMap();

    //TO CREATE EVENT QUEUE
    ALLEGRO_EVENT_QUEUE *eventqueue = al_create_event_queue();
    al_register_event_source(eventqueue, al_get_keyboard_event_source());
    al_register_event_source(eventqueue, al_get_display_event_source(display));
    al_register_event_source(eventqueue, al_get_mouse_event_source());

    //dummies
    int blocknumberx,blocknumbery;

    while(true)
    {
        ALLEGRO_EVENT event;
        int mousex,mousey;


        //TO DISPLAY MAIN SCREEN OF THE GAME
        al_wait_for_event(eventqueue, &event);

        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            mousex = event.mouse.x;
            mousey = event.mouse.y;
            blocknumberx= mousex/MineSize;
            blocknumbery=mousey/MineSize;
            if(storagemap[blocknumberx][blocknumbery]==9)
            {
                //if click a mine, open all mines and offer shutdown
                al_clear_to_color(al_map_rgb(0, 0, 0));
                for(int i = 0;i<30;i++)
                {
                    for(int j = 0;j<30;j++)
                    {
                        if(storagemap[i][j]==9)
                            displaymap[i][j]=storagemap[i][j];
                        al_draw_bitmap(blocks[displaymap[i][j]],i*20,j*20,NULL);
                    }
                }
                al_flip_display();

                al_show_native_message_box(display,"MINESWEEPER","GAME OVER!!!!","YOU HAVE LOST!!!",NULL,ALLEGRO_MESSAGEBOX_OK_CANCEL);

                if(al_show_native_message_box(display, "Restart?","Do you want to restart?",NULL,NULL,ALLEGRO_MESSAGEBOX_YES_NO))
                {
                    initMaps();
                    createMines();
                    drawMap();
                    continue;
                }
                //quit the game otherwise
                else break;
            }
            if(update(blocknumberx,blocknumbery))
            {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                for(int i = 0;i<30;i++)
                {
                    for(int j = 0;j<30;j++)
                    {
                        if(storagemap[i][j]==9)
                            displaymap[i][j]=storagemap[i][j];
                        al_draw_bitmap(blocks[displaymap[i][j]],i*20,j*20,NULL);
                    }
                }
                al_flip_display();

                al_show_native_message_box(display,"MINESWEEPER","GAME WON!!!!","YOU ARE THE BOSS!!!",NULL,ALLEGRO_MESSAGEBOX_OK_CANCEL);

                if(al_show_native_message_box(display, "Restart?","Do you want to restart?",NULL,NULL,ALLEGRO_MESSAGEBOX_YES_NO))
                {
                    initMaps();
                    createMines();
                    drawMap();
                    continue;
                }
                //quit the game otherwise
                else break;
            }
            drawMap();
        }
    }
    return 0;
}

void drawMap(void)
{
    al_clear_to_color(al_map_rgb(255,255,255));
    for(int i = 0;i<Mine_x;i++)
    {
        for(int j = 0;j<Mine_y;j++)
            al_draw_bitmap(blocks[displaymap[i][j]],i*MineSize,j*MineSize,NULL);
    }
    al_flip_display();
}

void initMaps(void)
{
    for(int i = 0;i<Mine_x;i++)
        for(int j = 0;j<Mine_y;j++)
        {
            displaymap[i][j] = 10;
            storagemap[i][j] = 0;
        }
}

void createMines(void)
{
    int mine_x,mine_y;
    int mines = mine_count;
    totalus = Mine_x*Mine_y;
    while(mines>0)
    {
        //calculate random positions of mine
        mine_x = rand()%Mine_x;
        mine_y = rand()%Mine_y;
        if(storagemap[mine_x][mine_y]!=9)
        {
            storagemap[mine_x][mine_y]=9;
            if(mine_x!=0)
            {
                if(storagemap[mine_x-1][mine_y]!=9)     //check if no mine
                    storagemap[mine_x-1][mine_y]++;
                if(mine_y!=0)
                    if(storagemap[mine_x-1][mine_y-1]!=9)     //check if no mine
                        storagemap[mine_x-1][mine_y-1]++;
                if(mine_y!=Mine_y-1)
                     if(storagemap[mine_x-1][mine_y+1]!=9)     //check if no mine
                        storagemap[mine_x-1][mine_y+1]++;
            }
            if(mine_x!=Mine_x-1)
            {
                if(storagemap[mine_x+1][mine_y]!=9)     //check if no mine
                    storagemap[mine_x+1][mine_y]++;
                if(mine_y!=0)
                    if(storagemap[mine_x+1][mine_y-1]!=9)     //check if no mine
                        storagemap[mine_x+1][mine_y-1]++;
                if(mine_y!=Mine_y-1)
                     if(storagemap[mine_x+1][mine_y+1]!=9)     //check if no mine
                        storagemap[mine_x+1][mine_y+1]++;
            }
            if(mine_y!=0)
            {
                if(storagemap[mine_x][mine_y-1]!=9)     //check if no mine
                    storagemap[mine_x][mine_y-1]++;
            }
            if(mine_y!=0)
            {
                if(storagemap[mine_x][mine_y+1]!=9)     //check if no mine
                    storagemap[mine_x][mine_y+1]++;
            }
            mines--;
        }
    }
}

int update(int x,int y)
{
    displaymap[x][y] = storagemap[x][y];
    totalus--;
    if(totalus<=mine_count)
        return 1;
    if(storagemap[x][y]==9)return 0;  //if updating mine, don't recurse
    if(storagemap[x][y]>0 && storagemap[x][y]<9)return 0; //if already opened, return
    if(x!=0)
    {
        if(storagemap[x-1][y]!=9 && displaymap[x-1][y]==10) //top of current block
            update(x-1,y);
        if(y!=0)
            if(storagemap[x-1][y-1]!=9 && displaymap[x-1][y-1]==10) //top left of current block
                update(x-1,y-1);
        if(y!=Mine_y)
             if(storagemap[x-1][y+1]!=9 && displaymap[x-1][y+1]==10)    //top right of current block
                update(x-1,y+1);
    }
    if(x!=Mine_x)
    {
        if(storagemap[x+1][y]!=9 && displaymap[x+1][y]==10) //bottom of current block
            update(x+1,y);
        if(y!=0)
            if(storagemap[x+1][y-1]!=9 && displaymap[x+1][y-1]==10) //bottom left of current block
                update(x+1,y-1);
        if(y!=Mine_y)
             if(storagemap[x+1][y+1]!=9 && displaymap[x+1][y+1]==10)    //bottom right of current block
                update(x+1,y+1);
    }
    if(y!=0)
    {
        if(storagemap[x][y-1]!=9 && displaymap[x][y-1]==10) //left
            update(x,y-1);
    }
    if(y!=0)
    {
        if(storagemap[x][y+1]!=9 && displaymap[x][y+1]==10) //right
            update(x,y+1);
    }
    return 0;
}
