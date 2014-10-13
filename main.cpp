#include<allegro.h>
#include<alpng.h>
#include<time.h>

#define GAME 1
#define SPACEMAP 2

FONT* descriptor;
FONT* f1;
FONT* f2;
FONT* f3;
FONT* f4;
FONT* f5;

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
BITMAP* spacemap_space;
BITMAP* cursor;
BITMAP* planet_nebula;
BITMAP* planet_darkmore;
BITMAP* planet_purplax;
BITMAP* planet_heberion;
BITMAP* spacemap_descriptor_background;
BITMAP* spacemap_scanner;

bool close_button_pressed;

int GAME_STATE=SPACEMAP;
int scroll_x;
int scroll_y;
int player_x=200;
int player_direction=1;
int weapon;
int map_scroll_x;
int map_scroll_y;
int map_zoom_level;
int mouse_z_old;
int planet_selected;
float spacemap_scanner_angle;

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

bool location_clicked(int min_x,int max_x,int min_y,int max_y){
    if(mouse_x>min_x && mouse_x<max_x && mouse_y>min_y && mouse_y<max_y && mouse_b & 1)
        return true;
    else return false;
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
        spacemap_scanner_angle+=0.2;
        if(spacemap_scanner_angle==255)spacemap_scanner_angle=0;
        if(mouse_z_old>mouse_z)map_zoom_level++;
        if(mouse_z_old<mouse_z && map_zoom_level>1)map_zoom_level--;
        mouse_z_old=mouse_z;
        if(map_zoom_level==0)map_zoom_level=1;

        if(mouse_x>SCREEN_W-40)map_scroll_x-=2*map_zoom_level;
        if(mouse_x<40)map_scroll_x+=2*map_zoom_level;
        if(mouse_y>SCREEN_H-40)map_scroll_y-=2*map_zoom_level;
        if(mouse_y<40)map_scroll_y+=2*map_zoom_level;

        if(location_clicked((100+map_scroll_x)/map_zoom_level,((100+map_scroll_x)/map_zoom_level)+200/map_zoom_level,(1000+map_scroll_y)/map_zoom_level,((1000+map_scroll_y)/map_zoom_level)+200/map_zoom_level)){
            planet_selected=1;
        }
        if(location_clicked((700+map_scroll_x)/map_zoom_level,((700+map_scroll_x)/map_zoom_level)+200/map_zoom_level,(500+map_scroll_y)/map_zoom_level,((500+map_scroll_y)/map_zoom_level)+200/map_zoom_level)){
            planet_selected=2;
        }
        if(location_clicked((200+map_scroll_x)/map_zoom_level,((200+map_scroll_x)/map_zoom_level)+200/map_zoom_level,(2000+map_scroll_y)/map_zoom_level,((2000+map_scroll_y)/map_zoom_level)+200/map_zoom_level)){
            planet_selected=3;
        }

        if(location_clicked((600+map_scroll_x)/map_zoom_level,((600+map_scroll_x)/map_zoom_level)+200/map_zoom_level,(-500+map_scroll_y)/map_zoom_level,((-500+map_scroll_y)/map_zoom_level)+200/map_zoom_level)){
            planet_selected=4;
        }


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
        draw_sprite(buffer, spacemap_space,map_scroll_x/map_zoom_level,map_scroll_y/map_zoom_level);
        draw_sprite(buffer, spacemap_space,0,0);
        draw_sprite(buffer, spacemap_space,-3072+map_scroll_x/map_zoom_level,map_scroll_y/map_zoom_level);
        draw_sprite(buffer, spacemap_space,-3072+map_scroll_x/map_zoom_level,-3072+map_scroll_y/map_zoom_level);
        draw_sprite(buffer, spacemap_space,-3072+map_scroll_x/map_zoom_level,3072+map_scroll_y/map_zoom_level);

        stretch_sprite(buffer, planet_darkmore,(100+map_scroll_x)/map_zoom_level,(1000+map_scroll_y)/map_zoom_level,200/map_zoom_level,200/map_zoom_level);
        stretch_sprite(buffer, planet_purplax,(700+map_scroll_x)/map_zoom_level,(500+map_scroll_y)/map_zoom_level,200/map_zoom_level,200/map_zoom_level);
        stretch_sprite(buffer, planet_nebula,(200+map_scroll_x)/map_zoom_level,(2000+map_scroll_y)/map_zoom_level,200/map_zoom_level,200/map_zoom_level);
        stretch_sprite(buffer, planet_heberion,(600+map_scroll_x)/map_zoom_level,(-500+map_scroll_y)/map_zoom_level,200/map_zoom_level,200/map_zoom_level);


        if(planet_selected!=0){
            draw_sprite(buffer, spacemap_descriptor_background,40,80);
            if(planet_selected==1)textprintf_ex(buffer,descriptor,45,75,makecol(0,0,0),-1,"Planet: Darkmore");
            if(planet_selected==2)textprintf_ex(buffer,descriptor,45,75,makecol(0,0,0),-1,"Planet: Purplax");
            if(planet_selected==3)textprintf_ex(buffer,descriptor,45,75,makecol(0,0,0),-1,"Planet: Nebula");
            if(planet_selected==4)textprintf_ex(buffer,descriptor,45,75,makecol(0,0,0),-1,"Planet: Heberion");
        }
        rotate_sprite(buffer, spacemap_scanner, 512, -384, itofix(spacemap_scanner_angle));
        draw_sprite(buffer, spacemap_overlay,0,0);




    }
    draw_sprite(buffer, cursor,mouse_x-9,mouse_y-9);
    draw_sprite(screen,buffer,0,0);
}







void setup(){
    buffer=create_bitmap(1024,768);


    if(!(f1 = load_font("descriptor.pcx", NULL, NULL))){
        abort_on_error( "Cannot find font descriptor.pcx \n Please check your files and try again");
    }
    f2 = extract_font_range(f1, ' ', 'A'-1);
    f3 = extract_font_range(f1, 'A', 'Z');
    f4 = extract_font_range(f1, 'Z'+1, 'z');

    //Merge temporary fonts to create "pixelart"
    descriptor = merge_fonts(f4, f5 = merge_fonts(f2, f3));


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

    if (!(spacemap_space = load_bitmap("spacemap_space.png", NULL)))
      abort_on_error("Cannot find image spacemap_space.png\nPlease check your files and try again");

    if (!(cursor = load_bitmap("cursor.png", NULL)))
      abort_on_error("Cannot find image cursor.png\nPlease check your files and try again");

    if (!(planet_heberion = load_bitmap("planet_heberion.png", NULL)))
      abort_on_error("Cannot find image planet_heberion.png\nPlease check your files and try again");

    if (!(planet_darkmore = load_bitmap("planet_darkmore.png", NULL)))
      abort_on_error("Cannot find image planet_darkmore.png\nPlease check your files and try again");

    if (!(planet_nebula = load_bitmap("planet_nebula.png", NULL)))
      abort_on_error("Cannot find image planet_nebula.png\nPlease check your files and try again");

    if (!(planet_purplax = load_bitmap("planet_purplax.png", NULL)))
      abort_on_error("Cannot find image planet_purplax.png\nPlease check your files and try again");

    if (!(spacemap_descriptor_background = load_bitmap("spacemap_descriptor_background.png", NULL)))
      abort_on_error("Cannot find image spacemap_descriptor_background.png\nPlease check your files and try again");

    if (!(spacemap_scanner = load_bitmap("spacemap_scanner.png", NULL)))
      abort_on_error("Cannot find image spacemap_scanner.png\nPlease check your files and try again");
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
