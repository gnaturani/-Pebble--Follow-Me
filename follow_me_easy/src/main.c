#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"


#define MY_UUID { 0x24, 0x70, 0x5D, 0x56, 0x05, 0xB0, 0x4D, 0xB3, 0x9F, 0x6F, 0x09, 0x72, 0x59, 0x52, 0x78, 0x92 }
PBL_APP_INFO(MY_UUID,
             "Follow Me", "GNat",
             1, 0, /* App version */
             //DEFAULT_MENU_ICON,
			 RESOURCE_ID_IMAGE_MENU_ICON,
             APP_INFO_STANDARD_APP);

static struct FollowMeData {
  Window window;
  TextLayer info_layer1;
	TextLayer distance1;
	TextLayer um_distance1;
	BmpContainer image;
} s_data;


Window window;

void my_out_sent_handler(DictionaryIterator *sent, void *context) {
  // outgoing message was delivered
	text_layer_set_text(&s_data.info_layer1, "outgoing message was delivered");
}
void my_out_fail_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
  // outgoing message failed
	text_layer_set_text(&s_data.info_layer1, "outgoing message failed");
}
void my_in_rcv_handler(DictionaryIterator *received, void *context) {
  // incoming message received
	//text_layer_set_text(&s_data.info_layer1, "incoming message received");
	
				static char str0[60];
			static char str1[60];
				static char str2[60];
			static char str3[60];
			static char strImage[60];	
	
	Tuple *tuple = dict_read_first(received);
    while (tuple) {
        switch (tuple->key) {
            case 0:
                memcpy(str0, tuple->value->cstring, (int)tuple->length);	
				text_layer_set_text(&s_data.info_layer1,str0);
                break;
            case 1:
                memcpy(str1, tuple->value->cstring, (int)tuple->length);	
				text_layer_set_text(&s_data.distance1,str1);

				break;
            case 2:
                memcpy(str2, tuple->value->cstring, (int)tuple->length);	
				text_layer_set_text(&s_data.um_distance1,str2);			
                break;		
            case 3:
                break;		
			
            case 6:

                memcpy(strImage, tuple->value->cstring, (int)tuple->length);	
			
				layer_remove_from_parent(&s_data.image.layer.layer);
				bmp_deinit_container(&s_data.image);	
			
				if ( strcmp (strImage, "ARROW_LEFT") == 0){
					bmp_init_container(RESOURCE_ID_IMAGE_ARROW_LEFT_BLACK, &s_data.image);					
				}
				if ( strcmp (strImage, "ARROW_RIGHT") == 0){
					bmp_init_container(RESOURCE_ID_IMAGE_ARROW_RIGHT_BLACK, &s_data.image);					
				}
				if ( strcmp (strImage, "ARROW_ROUND_FIRST") == 0){
					bmp_init_container(RESOURCE_ID_IMAGE_ARROW_ROUND_FIRST_BLACK, &s_data.image);					
				}			
				if ( strcmp (strImage, "ARROW_ROUND_SECOND") == 0){
					bmp_init_container(RESOURCE_ID_IMAGE_ARROW_ROUND_SECOND_BLACK, &s_data.image);					
				}			
				if ( strcmp (strImage, "ARROW_ROUND_THIRD") == 0){
					bmp_init_container(RESOURCE_ID_IMAGE_ARROW_ROUND_THIRD_BLACK, &s_data.image);					
				}			
				if ( strcmp (strImage, "ARROW_NOT_FOUND") == 0){
					bmp_init_container(RESOURCE_ID_IMAGE_ARROW_NOT_FOUND_BLACK, &s_data.image);					
				}				
			
				layer_set_frame(&s_data.image.layer.layer, GRect(0, 100, 70, 70));
				layer_add_child(&s_data.window.layer, &s_data.image.layer.layer);			
			
			
				break;				
			
			default:
                return;
        }
		return;
        tuple = dict_read_next(received);
    }	
	 
	
}
void my_in_drp_handler(void *context, AppMessageResult reason) {
  // incoming message dropped
	text_layer_set_text(&s_data.info_layer1, "incoming message dropped");
}





void handle_init(AppContextRef ctx) {  
	
  Window* window = &s_data.window;	
	
  window_init(window, "Window Name");	
	
resource_init_current_app(&APP_RESOURCES);	
	
	const int width = 144;
	const int h_distance = 25;
	const int h_info1 = 50;
	const int h_infon = 75;
	int count_info = 0;
	
	//window_set_background_color(window, GColorBlack);
	
	text_layer_init(&s_data.info_layer1, GRect(0, 0, 144, 100));
	text_layer_set_text_alignment(&s_data.info_layer1, GTextAlignmentCenter);
	text_layer_set_text(&s_data.info_layer1, "Info 1");
	text_layer_set_font(&s_data.info_layer1, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	layer_add_child(&window->layer, &s_data.info_layer1.layer);	
	
	text_layer_init(&s_data.distance1, GRect(70, 100, 70, 50));
	text_layer_set_text_alignment(&s_data.distance1, GTextAlignmentCenter);
	text_layer_set_text(&s_data.distance1, "9999");
	text_layer_set_font(&s_data.distance1, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	layer_add_child(&window->layer, &s_data.distance1.layer);	
	
	bmp_init_container(RESOURCE_ID_IMAGE_ARROW_UP1_BLACK, &s_data.image);
	layer_set_frame(&s_data.image.layer.layer, GRect(0, 90, 70, 70));
	layer_add_child(&window->layer, &s_data.image.layer.layer);
	
	window_stack_push(window, true /* Animated */);
}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .messaging_info = {		
		
		.default_callbacks.callbacks = {
        .out_sent = my_out_sent_handler,
        .out_failed = my_out_fail_handler,
        .in_received = my_in_rcv_handler,
        .in_dropped = my_in_drp_handler,
      },
		
      .buffer_sizes = {
        .inbound = 64,
        .outbound = 16,
      }
    }			
  };
  app_event_loop(params, &handlers);
}

