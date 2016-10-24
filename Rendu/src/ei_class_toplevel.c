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
#include "ei_class_toplevel.h"
#include <time.h>
#include "ei_callback.h"

void* toplevel_allocfunc() {
    ei_class_toplevel_t *toplevel = calloc(1, sizeof(ei_class_toplevel_t));
    return toplevel;
}

void toplevel_releasefunc(struct ei_widget_t* widget) {
    if (widget) {
	ei_class_toplevel_t *toplevel = (ei_class_toplevel_t*)widget;
	if (toplevel->title
 != NULL) {
	    free(toplevel->title);
	    toplevel->title = NULL;
	}
	if (toplevel->min_size != NULL) {
	    free(toplevel->min_size);
	    toplevel->min_size = NULL;
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

void toplevel_drawfunc(struct ei_widget_t*	widget,
		       ei_surface_t		surface,
		       ei_surface_t		pick_surface,
		       ei_rect_t*		clipper) {
    ei_class_toplevel_t *toplevel = (ei_class_toplevel_t*)widget;
    //Tracé du rectangle intérieur, et de la bordure (avec espace pour le titre)
    int title_height = ei_font_default_size + 5; // Valeur arbitraire, à changer ?
    ei_linked_point_t* border = top_rounded_frame(&widget->screen_location, title_height);
    ei_color_t color_surface = toplevel->color;
    ei_color_t border_color = darker_color(color_surface);
    ei_rect_t inside_rect = widget->screen_location;
    
    inside_rect.size.width -= toplevel->border_width * 2;
    inside_rect.size.height -= toplevel->border_width + title_height;
    inside_rect.top_left.x += toplevel->border_width;
    inside_rect.top_left.y += title_height;
    ei_linked_point_t* inside = linked_point_from_rect(inside_rect);
    ei_draw_polygon(surface, border, border_color, clipper);
    ei_draw_polygon(surface, inside, color_surface, clipper);

    refresh_clipper(surface, clipper);

    //Tracé du widget dans l'offscreen
    draw_widget_in_offscreen(pick_surface, widget, clipper);

    //Tracé de l'offscreen pour déplacer le toplevel
    //TODO

    //Ecriture du titre
    if (toplevel->title) {
	ei_point_t topleft = ei_point_add(toplevel->screen_location.top_left, ei_point(title_height,0));
	ei_color_t color_text;
	color_text.red = (255 - color_surface.red);
	color_text.blue = (255 - color_surface.blue);
	color_text.green = (255 - color_surface.green);
	ei_draw_text(surface, &topleft, toplevel->title, ei_default_font, &color_text, clipper);
	refresh_clipper(surface,clipper);
    }
    //Tracé de la zone cliquable pour le redimensionnement
    if (toplevel->resizable != 0) {
	int resize_button_size = 10;
	ei_rect_t resize_rect = widget->screen_location;
	resize_rect.size.width = resize_button_size;
	resize_rect.size.height = resize_button_size;
	resize_rect.top_left.x += (widget->screen_location.size.width - resize_button_size);
	resize_rect.top_left.y += (widget->screen_location.size.height - resize_button_size);
	ei_linked_point_t* resize_button = linked_point_from_rect(resize_rect);
	ei_draw_polygon(surface, resize_button, border_color, clipper);
	refresh_clipper(surface, clipper);
    }

    //Pour faire du carré rouge de fermeture un bouton
    /* if (toplevel->closable){ */
    /* 	//(*toplevel->children_tail->wclass->drawfunc)(toplevel->children_tail, surface, pick_surface, clipper); */
    /* 	//(*toplevel->children_tail->geom_params->manager->runfunc)(widget->children_tail); */
    /* } */

    if (toplevel->closable) {
	//devrait être dans widget.toplevel_configure
	ei_rect_t rect_close = widget->screen_location;
	rect_close.size.width = 10;
	rect_close.size.height = 10;
	rect_close.top_left.x += 10;
	rect_close.top_left.y += 10;
	ei_color_t red = {0xf0, 0x10, 0x10, 0xff};
	ei_linked_point_t *close_button = rounded_frame(&rect_close, 0, EI_TRUE, EI_TRUE);
	ei_draw_polygon(surface, close_button, red, clipper); 
	refresh_clipper(surface, clipper);
    }
}

void toplevel_geomnotifyfunc (struct ei_widget_t*	widget,
                           ei_rect_t		rect){
    NULL;
}

void toplevel_setdefaults(struct ei_widget_t* widget) {
    ei_class_toplevel_t *toplevel = (ei_class_toplevel_t*)widget;

    srand(time(NULL));
    ei_color_t black = {0x00, 0x00, 0x00, 0xff};
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
    ei_size_t default_requested_size = {320,240};
    widget->requested_size = default_requested_size;
    ei_point_t origin = {0,0};
    ei_size_t size = {0,0};
    ei_rect_t default_screen_location = {origin, size};
    widget->screen_location = default_screen_location;
    widget->content_rect = &(widget->screen_location);

    toplevel->color = (ei_color_t)ei_default_background_color;
    int default_border_width = 4;
    toplevel->border_width = default_border_width;
    char* default_title = "Toplevel";
    toplevel->title = default_title;
    toplevel->closable = EI_TRUE;
    toplevel->resizable = ei_axis_both;
    ei_size_t default_min_size = {160, 120};
    toplevel->min_size = &default_min_size;
}

    
