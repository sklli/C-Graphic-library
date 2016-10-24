#include <stdio.h>
#include <stdlib.h>
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_new_types.h"
#include "ei_class_frame.h"
#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_geometrymanager.h"
#include "ei_callback.h"


ei_linked_point_t *linked_point_from_rect(ei_rect_t rect) {
    ei_linked_point_t *linked = malloc(sizeof(ei_linked_point_t));
    linked->point = rect.top_left;
    linked->next = malloc(sizeof(ei_linked_point_t));
    linked->next->point = ei_point_add(rect.top_left, ei_point(rect.size.width,0));
    linked->next->next = malloc(sizeof(ei_linked_point_t));
    linked->next->next->point = ei_point_add(linked->next->point, ei_point(0, rect.size.height));
    linked->next->next->next = malloc(sizeof(ei_linked_point_t));
    linked->next->next->next->point = ei_point_add(linked->point, ei_point(0, rect.size.height));
    linked->next->next->next->next = NULL;
    return linked;
}

/* Fonction qui ajoute un élément à la liste des tags associés à l'event
 * mouse_button_down : appel du callback "bouton enfoncé" pour les widgets
 * de type button. */
void button_callback_register() {
    registered_event_type[4]->tag_and_callback = malloc(sizeof(struct tag_cell));
    registered_event_type[4]->tag_and_callback->tag = "button";
    registered_event_type[4]->tag_and_callback->callback = &button_mouse_button_down;
    registered_event_type[4]->tag_and_callback->next = NULL;
}

void toplevel_callback_register() {
    registered_event_type[4]->tag_and_callback->next = malloc(sizeof(struct tag_cell));
    registered_event_type[4]->tag_and_callback->next->tag = "toplevel";
    registered_event_type[4]->tag_and_callback->next->callback = &toplevel_processing;
    registered_event_type[4]->tag_and_callback->next->next = NULL;
}

void init_registers(){
    // Liste des classes de widgets
    registered_class = malloc(sizeof(struct class_cell));
    registered_class->wclass = NULL;
    registered_class->next = NULL;
    // Liste des gestionnaires de géométrie
    registered_geo = malloc(sizeof(struct geo_cell));
    registered_geo->manager = NULL;
    registered_geo->next = NULL;
    // Liste des couleurs des widgets (dans l'offscreen)
    registered_color = malloc(sizeof(struct color_cell));
    ei_color_t black = {0,0,0,255};
    registered_color->color = black;
    registered_color->next = NULL;

    // Liste des tags
    registered_tag = malloc(sizeof(ei_linked_tag_t));
    registered_tag->tag = "all";
    registered_tag->next = NULL;

    // Liste des event types
    int i = 0;
    for (i = 0; i < 8 ; i++) {
        registered_event_type[i] = malloc(sizeof(struct event_type_list));
        registered_event_type[i]->event = i;
        registered_event_type[i]->tag_and_callback = NULL;
        registered_event_type[i]->widget_and_callback = NULL;
    }

    // Ajout des callback présents par défaut
    button_callback_register();
    toplevel_callback_register();
}

void ei_placer_setdefaults (ei_widget_t* widget) {
    ei_placer_t* placer = malloc(sizeof(ei_placer_t));
    placer->manager = widget->geom_params->manager;
    placer->widget = widget;
    placer->anchor = ei_anc_northwest;
    placer->x = 0;
    placer->y = 0;
    placer->width = widget->requested_size.width;
    placer->height = widget->requested_size.height;
    placer->rel_x = 0.0;
    placer->rel_y = 0.0;
    placer->rel_width = 0.0;
    placer->rel_height = 0.0;
}

ei_bool_t color_is_taken(ei_color_t color) {
    struct color_cell* current = registered_color;
    while (current->next) {
	if (current->color.red == color.red && current->color.green == color.green && current->color.blue == color.blue) {
	    return EI_TRUE;
	}
	current = current->next;
    }
    return EI_FALSE;
}

void color_register(ei_color_t color) {
    struct color_cell* current = registered_color;
    while (current->next) {
	current = current->next;
    }
    current->next = malloc(sizeof(struct color_cell));
    current->next->color = color;
    current->next->next = NULL;
}
