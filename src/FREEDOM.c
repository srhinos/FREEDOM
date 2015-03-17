#include <pebble.h>
#include <math.h>

static Window *window;
static TextLayer *time_layer;
static TextLayer *pretty_layerUno;
static TextLayer *pretty_layerDos;
static TextLayer *pretty_layerTres;
static TextLayer *pretty_layerQuatro;
static BitmapLayer *flag_layer;
static int current_flag;
static Layer *timer_layer;
static AppTimer *timer;

uint32_t flag_names[5] = {RESOURCE_ID_FLAG1, RESOURCE_ID_FLAG3, RESOURCE_ID_FLAG5, RESOURCE_ID_FLAG7, RESOURCE_ID_FLAG9};
GBitmap* flags[5];

static void window_load(Window *window) {
  flag_layer = bitmap_layer_create((GRect) { .origin = { 0, 0 }, .size = { 144, 168 } });
  bitmap_layer_set_bitmap(flag_layer, gbitmap_create_with_resource(RESOURCE_ID_FLAG1));
  layer_add_child(timer_layer, bitmap_layer_get_layer(flag_layer));

  current_flag = 0;
  
  pretty_layerUno = text_layer_create(GRect(1, 54, 144, 50));
  text_layer_set_text_alignment(pretty_layerUno, GTextAlignmentCenter);
  text_layer_set_text_color(pretty_layerUno, GColorWhite);
  text_layer_set_background_color(pretty_layerUno, GColorClear);
  text_layer_set_font(pretty_layerUno, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text(pretty_layerUno, "USA");
  layer_add_child(timer_layer, text_layer_get_layer(pretty_layerUno));
  
  pretty_layerDos = text_layer_create(GRect(3, 54, 144, 50));
  text_layer_set_text_alignment(pretty_layerDos, GTextAlignmentCenter);
  text_layer_set_text_color(pretty_layerDos, GColorWhite);
  text_layer_set_background_color(pretty_layerDos, GColorClear);
  text_layer_set_font(pretty_layerDos, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text(pretty_layerDos, "USA");
  layer_add_child(timer_layer, text_layer_get_layer(pretty_layerDos));
  
  pretty_layerTres = text_layer_create(GRect(1, 57, 144, 50));
  text_layer_set_text_alignment(pretty_layerTres, GTextAlignmentCenter);
  text_layer_set_text_color(pretty_layerTres, GColorWhite);
  text_layer_set_background_color(pretty_layerTres, GColorClear);
  text_layer_set_font(pretty_layerTres, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text(pretty_layerTres, "USA");
  layer_add_child(timer_layer, text_layer_get_layer(pretty_layerTres));
  
  pretty_layerQuatro = text_layer_create(GRect(3, 57, 144, 50));
  text_layer_set_text_alignment(pretty_layerQuatro, GTextAlignmentCenter);
  text_layer_set_text_color(pretty_layerQuatro, GColorWhite);
  text_layer_set_background_color(pretty_layerQuatro, GColorClear);
  text_layer_set_font(pretty_layerQuatro, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text(pretty_layerQuatro, "USA");
  layer_add_child(timer_layer, text_layer_get_layer(pretty_layerQuatro));
  
  time_layer = text_layer_create(GRect(2, 55, 144, 50));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);
  text_layer_set_text_color(time_layer, GColorBlack);
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text(time_layer, "USA");
  layer_add_child(timer_layer, text_layer_get_layer(time_layer));
}
void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  static char time_text[] = " 0:00";
  char *time_format;
  if (!clock_is_24h_style()){ 
    time_format = "%l:%M";
  }else{
    time_format = "%H:%M";
  }

  strftime(time_text, sizeof(time_text), time_format, tick_time);
  text_layer_set_text(time_layer, time_text);
  text_layer_set_text(pretty_layerUno, time_text);
  text_layer_set_text(pretty_layerDos, time_text);
  text_layer_set_text(pretty_layerTres, time_text);
  text_layer_set_text(pretty_layerQuatro, time_text);
}

static void timer_callback(void *data) {
  bitmap_layer_set_bitmap(flag_layer, flags[current_flag]);
	current_flag++;
	if(current_flag > 4){
		current_flag = 0;
	}
  timer = app_timer_register(100, timer_callback, NULL);
}

static void window_unload(Window *window) {
  text_layer_destroy(time_layer);
  text_layer_destroy(pretty_layerUno);
  text_layer_destroy(pretty_layerDos);
  text_layer_destroy(pretty_layerTres);
  text_layer_destroy(pretty_layerQuatro);
  bitmap_layer_destroy(flag_layer);
}

static void init(void) {
  window = window_create();
  timer_layer = window_get_root_layer(window);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(window, true);
  

  for(int i = 0; i < 5; i++){
      flags[i] = gbitmap_create_with_resource(flag_names[i]);
      APP_LOG(APP_LOG_LEVEL_DEBUG, "loaded bitmap %u", i);
  }

  timer = app_timer_register(100, timer_callback, NULL);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "init end");
}
static void deinit(void) {
  tick_timer_service_unsubscribe();
  layer_destroy(timer_layer);
  //window_destroy(window);
}
int main(void) {
  init();
  app_event_loop();
  deinit();
}