#include "ei_widgetclass.h"
#include "ei_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include "ei_widget.h"
#include <string.h>
#include "ei_new_types.h"
#include "ei_class_frame.h"
#include "relief.h"
#include "ei_draw_widget.h"
#include "ei_class_button.h"
#include <time.h>

void* button_allocfunc() {
    ei_class_button_t *button = calloc(1, sizeof(ei_class_button_t));
    return button;
}

void button_releasefunc(struct ei_widget_t* widget) {
    if (widget) {
	ei_class_button_t *button = (ei_class_button_t*)widget;
	if (button->text) {
	    free(button->text);
	    button->text = NULL;
	}
	if (button->image_rect) {
	    free(button->image_rect);
	    button->image_rect = NULL;
	}
	if (button->user_param) {
	    free(button->user_param);
	    button->user_param = NULL;
	}
	ei_widget_t *current = widget->children_head;
	ei_widget_t *prev = widget->children_head;
	while (current) {
	    current = current->next_sibling;
	    ei_widget_destroy(prev);
	    prev = current;
	}

    }
}

void button_drawfunc (struct ei_widget_t*	widget,
		     ei_surface_t		surface,
		     ei_surface_t		pick_surface,
		     ei_rect_t*		        clipper) {
 
    ei_class_button_t *button = (ei_class_button_t*)widget;

    // Tracé du cadre du widget dans la fenêtre graphique.
    draw_frame_or_button_relief(widget, surface, clipper, EI_FALSE);
    
    refresh_clipper(surface,clipper);
    
    // Tracé du widget dans l'offscreen
    draw_widget_in_offscreen(pick_surface,widget,clipper);

    // Ecriture du texte dans le widget (si nécessaire)
    if (button->text) {
       
        //Recherche du top_left du texte
        ei_point_t text_top_left = placement_text_image(widget);

        ei_draw_text(surface,&text_top_left,button->text, button->text_font, &button->text_color,clipper);

        refresh_clipper(surface,clipper);
    }

    // Chargement de l'image dans le widget (si nécessaire)
    else if (button->image) {

        hw_surface_lock(button->image);
        hw_surface_lock(surface);
        ei_rect_t image_on_surface;
        image_on_surface.top_left = placement_text_image(widget);
        image_on_surface.size = button->image_rect->size;
        ei_copy_surface(surface,&image_on_surface,button->image,button->image_rect,EI_FALSE);
        hw_surface_unlock(button->image);
        hw_surface_unlock(surface);
        refresh_clipper(surface,clipper);
    }
}

void button_geomnotifyfunc (struct ei_widget_t*	widget,
                           ei_rect_t		rect){
    NULL;
}

void button_setdefaults (struct ei_widget_t*	widget){

    
    ei_class_button_t *button = (ei_class_button_t*)widget;
    srand(time(NULL));

    ei_color_t black = { 0x00, 0x00, 0x00, 0xff };
    widget->pick_color = &black;
    
    unsigned char random_red;
    unsigned char random_green;
    unsigned char random_blue;
    
    while (color_is_taken(*widget->pick_color)) {
        random_red = rand() % 256;
        random_green = rand() % 256;
        random_blue = rand() % 256;

        widget->pick_color->red = random_red;
        widget->pick_color->green = random_green;
        widget->pick_color->blue = random_blue;
    }
    
    color_register(*widget->pick_color);

    // Ordre des couleurs à prendre en compte avec ei_map_rgba ?
    widget->pick_id = ( widget->pick_color->red << 24  |
                        widget->pick_color->green << 16 |
                        widget->pick_color->blue << 8 | 
                        255
                      );

    /* Par défaut, il n'y a ni image ni texte, donc pas de
     * taille requise */
    ei_size_t size = {0,0};
    widget->requested_size = size;
    ei_point_t origin = {0,0};
    ei_rect_t default_screen_location = {origin, size};
    widget->screen_location = default_screen_location;
    widget->content_rect = &(widget->screen_location);
    
    button->color = ei_default_background_color;
    button->text = NULL;
    button->text_font = ei_default_font;
    button->text_color = ei_font_default_color;
    button->text_anchor = ei_anc_center;
    button->image = NULL;
    button->image_rect = NULL;
    button->image_anchor = ei_anc_center;
    button->border_width = k_default_button_border_width;
    button->relief = ei_relief_raised;
    button->corner_radius = k_default_button_corner_radius;
    button->callback = NULL;
    button->user_param = NULL;
}
