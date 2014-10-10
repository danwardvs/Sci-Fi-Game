#include<allegro.h>
#include<alpng.h>
#include<time.h>

#define GAME 1
#define SPACEMAP 2

BITMAP* buffer;
BITMAP* planet;
BITMAP* spaceship;
BITMAP* character_left;
BITMAP* character_right;
BITMAP* character_gun_right;
BITMAP* character_gun_left;
BITMAP* character_shoot_1_right;
BITMAP* character_shoot_1_left;
BITMAP* spacemap_overlay;

bool close_button_pressed;

int GAME_STATE=GAME;
int scroll_x;
int scroll_y;
int player_x=200;
int player_direction=1;
int weapon;
bool shooting;

// FPS System
volatile int ticks = 0;
const int updates_per_second = 60;
volatile int game_time = 0;

int fps;
int frames_done;
int old_time;

void ticker(){
  ticks++;
}
END_OF_FUNCTION(ticker)

void game_time_ticker(){
  game_time++;
}
END_OF_FUNCTION(ticker)

void close_button_handler(void){
  close_button_pressed = TRUE;
}
END_OF_FUNCTION(close_button_handler)

// Random number generator. Use int random(highest,lowest);
int random(int newLowest, int newHighest)
{
  int lowest = newLowest, highest = newHighest;
  int range = (highest - lowest) + 1;
  int randomNumber = lowest+int(range*rand()/(RAND_MAX + 1.0));
  return randomNumber;
}


//A function to streamline error reporting in file loading
void abort_on_error(const char *message){
	 set_window_title("Error!");
	 if (screen != NULL){
	    set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	 }
	 allegro_message("%s.\n %s\n", message, allegro_error);
	 exit(-1);
}

void update(){

    if(GAME_STATE==GAME){
        if(key[KEY_TILDE])weapon=0;
        if(key[KEY_1])weapon=1;
        if(key[KEY_SPACE])shooting=true;
        else shooting=false;


        if(key[KEY_LEFT] ){
            player_direction=1;
            if(player_x<100 && scroll_x<1024){
                scroll_x+=5;
            }else player_x-=5;
        }
        if(key[KEY_RIGHT]){
            player_direction=2;
            if(player_x>SCREEN_W-100 && scroll_x>-1024){
                scroll_x-=5;
            }else player_x+=5;
        }
    }
    if(GAME_STATE==SPACEMAP){

    }





}

void draw(){

    if(GAME_STATE==GAME){
        draw_sprite(buffer, planet,scroll_x,0);
        draw_sprite(buffer, planet,scroll_x+1024,0);
        draw_sprite(buffer, planet,scroll_x+1024*2,0);
        draw_sprite(buffer, planet,scroll_x+1024*3,0);
        draw_sprite(buffer, planet,scroll_x-1024,0);
        draw_sprite(buffer, spaceship,200+scroll_x,100);

        if(player_direction==1 && weapon==0)draw_sprite(buffer, character_left,player_x,500);
        if(player_direction==2 && weapon==0)draw_sprite(buffer, character_right,player_x,500);
        if(player_direction==1 && weapon==1 && !shooting)draw_sprite(buffer, character_gun_left,player_x,500);
        if(player_direction==2 && weapon==1 && !shooting)draw_sprite(buffer, character_gun_right,player_x,500);
        if(player_direction==1 && weapon==1 && shooting)draw_sprite(buffer, character_shoot_1_left,player_x,500);
        if(player_direction==2 && weapon==1 && shooting)draw_sprite(buffer, character_shoot_1_right,player_x,500);
    }

    if(GAME_STATE==SPACEMAP){
        draw_sprite(buffer, spacemap_overlay,0,0);

    }
    draw_sprite(screen,buffer,0,0);
}







void setup(){
    buffer=create_bitmap(1024,768);


    srand(time(NULL));

     // Setup for FPS system
    LOCK_VARIABLE(ticks);
    LOCK_FUNCTION(ticker);
    install_int_ex(ticker, BPS_TO_TIMER(updates_per_second));

    LOCK_VARIABLE(game_time);
    LOCK_FUNCTION(game_time_ticker);
    install_int_ex(game_time_ticker, BPS_TO_TIMER(10));

    // Close button
    LOCK_FUNCTION(close_button_handler);
    set_close_button_callback(close_button_handler);

    if (!(planet = load_bitmap("planet.png", NULL)))
      abort_on_error("Cannot find image planet.png\nPlease check your files and try again");

    if (!(spaceship = load_bitmap("spaceship.png", NULL)))
      abort_on_error("Cannot find image spaceship.png\nPlease check your files and try again");

    if (!(character_right = load_bitmap("character_right.png", NULL)))
      abort_on_error("Cannot find image character_right.png\nPlease check your files and try again");

    if (!(character_left = load_bitmap("character_left.png", NULL)))
      abort_on_error("Cannot find image character_gun_left.png\nPlease check your files and try again");

    if (!(character_gun_left = load_bitmap("character_gun_left.png", NULL)))
      abort_on_error("Cannot find image character_left.png\nPlease check your files and try again");

    if (!(character_gun_right = load_bitmap("character_gun_right.png", NULL)))
      abort_on_error("Cannot find image character_gun_right.png\nPlease check your files and try again");

    if (!(character_shoot_1_right = load_bitmap("character_shoot_1_right.png", NULL)))
      abort_on_error("Cannot find image character_shoot_1_right.png\nPlease check your files and try again");

    if (!(character_shoot_1_left = load_bitmap("character_shoot_1_left.png", NULL)))
      abort_on_error("Cannot find image character_shoot_1_left.png\nPlease check your files and try again");

    if (!(spacemap_overlay = load_bitmap("spacemap_overlay.png", NULL)))
      abort_on_error("Cannot find image spacemap_overlay.png\nPlease check your files and try again");
}






int main(){

  allegro_init();
  alpng_init();
  install_timer();
  install_keyboard();
  install_mouse();
  set_color_depth(32);


  set_gfx_mode(GFX_AUTODETECT_WINDOWED,1024,768, 0, 0);
  install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,".");



  set_window_title("Sci-Fi game!");
  setup();


      while(!key[KEY_ESC] && !close_button_pressed){
        while(ticks == 0){
            rest(1);
        }
    while(ticks > 0){
        int old_ticks = ticks;

        update();

        ticks--;
        if(old_ticks <= ticks){
            break;
        }
    }
        if(game_time - old_time >= 10){
            fps = frames_done;
            frames_done = 0;
            old_time = game_time;
        }
        draw();
    }


	return 0;
}
END_OF_MAIN()
