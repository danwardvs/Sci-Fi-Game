#include<allegro.h>
#include<alpng.h>

BITMAP* buffer;
BITMAP* planet;


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

    draw_sprite(buffer, planet,0,0);
    draw_sprite(screen,buffer,0,0);

}








void setup(){
    buffer=create_bitmap(1024,768);

    if (!(planet = load_bitmap("planet.png", NULL)))
      abort_on_error("Cannot find image planet.png\nPlease check your files and try again");
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


    while(!key[KEY_ESC]){
        update();
  	}

	return 0;
}
END_OF_MAIN()
