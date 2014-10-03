#include<allegro.h>
#include<alpng.h>

BITMAP* buffer;

void update(){
    draw_sprite(screen,buffer,0,0);

}








void setup(){
    buffer=create_bitmap(1024,768);
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
