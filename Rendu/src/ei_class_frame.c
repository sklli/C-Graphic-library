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
#include <time.h>

struct color_cell *registered_color;


void* frame_allocfunc()  {
    ei_class_frame_t *frame = calloc(1, sizeof(ei_class_frame_t));
    return frame;
}

void frame_releasefunc(struct ei_widget_t* widget){
    if (widget) {
	ei_class_frame_t *frame = (ei_class_frame_t*)widget;
	if (frame->text) {
	    free(frame->text);
	}
	if (frame->image_rect) {
	    free(frame->image_rect);
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

void frame_drawfunc (struct ei_widget_t*	widget,
		     ei_surface_t		surface,
		     ei_surface_t		pick_surface,
		     ei_rect_t*		clipper){

    ei_class_frame_t *frame = (ei_class_frame_t*)widget;

    draw_frame_or_button_relief(widget, surface, clipper, EI_FALSE);
    
    /* La fonction suivante prend une surface bloquée en paramètre pour rafraîchir
     * l'affichage sur son clipper */
    refresh_clipper(surface,clipper);

    // Tracé du cadre du widget dans l'offscreen
    draw_widget_in_offscreen(pick_surface,widget,clipper);

    // Ecriture du texte dans le widget (si nécessaire)
    if (frame->text) {
        
        // Recherche du top_left du texte
        ei_point_t text_top_left = placement_text_image(widget);

        ei_draw_text(surface, &text_top_left, frame->text, frame->text_font, &frame->text_color, clipper);

        refresh_clipper(surface,clipper);
    }
    
    // Chargement de l'image dans le widget (si nécessaire)
    else if (frame->image) {

        //affichage de l'image
        hw_surface_lock(frame->image);
        ei_rect_t image_on_surface;
        image_on_surface.top_left = placement_text_image(widget);
        image_on_surface.size = frame->image_rect->size;
        ei_copy_surface(surface,&image_on_surface, frame->image,frame->image_rect,EI_FALSE);
        hw_surface_unlock(frame->image);
        refresh_clipper(surface,clipper);
    }
}



void frame_geomnotifyfunc (struct ei_widget_t*	widget,
                           ei_rect_t		rect){
    NULL;
}


void frame_setdefaults (struct ei_widget_t*	widget){
	
    /* if (!widget) { */
    /* 	widget = basic_widget(); */
    /* } */

    ei_class_frame_t *frame = (ei_class_frame_t*)widget;
    // Ordre des couleurs à prendre en compte avec ei_map_rgba ?
    /* Pour la couleur, on va en choisir une au hasard et vérifier
     * qu'elle n'est pas déjà prise. */
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

    widget->children_head = NULL;
    widget->children_tail = NULL;
    widget->parent = NULL;
    /* Par défaut, il n'y a ni image ni texte, donc pas de
     * taille requise */
    ei_size_t size = {0,0};
    widget->requested_size = size;
    ei_point_t origin = {0,0};
    ei_rect_t default_screen_location = {origin, size};
    widget->screen_location = default_screen_location;
    widget->content_rect = &(widget->screen_location);
    
    frame->color = ei_default_background_color;
    frame->border_width = 0;
    frame->relief = ei_relief_none;
    frame->text = NULL;
    frame->text_font = ei_default_font;
    frame->text_color = ei_font_default_color;
    frame->text_anchor = ei_anc_center;
    frame->image = NULL;
    frame->image_rect = NULL;
    frame->image_anchor = ei_anc_center;
}
