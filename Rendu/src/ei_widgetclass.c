#include <string.h>
#include "ei_types.h"
#include "ei_widget.h"
#include <stdio.h>
#include <stdlib.h>
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_new_types.h"
#include "ei_class_frame.h"
#include "ei_class_button.h"
#include "ei_class_toplevel.h"

void ei_widgetclass_register(ei_widgetclass_t* widgetclass) {
    // Ajout de la classe dans la liste des classe
    //struct class_cell *registered_class;
    if (registered_class->wclass != NULL) {
	struct class_cell *new = malloc(sizeof(struct class_cell));
	new->next = registered_class;
	registered_class = new;
    }
    registered_class->wclass = widgetclass;

    // Ajout du tag correspondant dans la liste des tags
    if (registered_tag->tag != NULL) {
        ei_linked_tag_t *new_tag = malloc(sizeof(ei_linked_tag_t));
        new_tag->next = registered_tag;
        registered_tag = new_tag;
    }
    registered_tag->tag = (ei_tag_t)(widgetclass->name);
}


ei_widgetclass_t* ei_widgetclass_from_name (ei_widgetclass_name_t name){
    struct class_cell *current = registered_class;
    while (current != NULL) {
	if (strncmp(current->wclass->name, name, strlen(name))==0) {
	    return current->wclass;
	}
	current = current->next;
    }
    return NULL;
}

void ei_frame_register_class() {
    ei_widgetclass_t *wclass = (ei_widgetclass_t*)malloc(sizeof(ei_widgetclass_t));
    char* name = "frame";
    strncpy(wclass->name, name, strlen(name)+1);
    wclass->allocfunc = &frame_allocfunc;
    wclass->releasefunc = &frame_releasefunc;
    wclass->drawfunc = &frame_drawfunc;
    wclass->setdefaultsfunc = &frame_setdefaults;
    wclass->geomnotifyfunc = &frame_geomnotifyfunc;
    wclass->next = NULL;
    ei_widgetclass_register(wclass);
}

void ei_button_register_class() {
    ei_widgetclass_t *wclass = (ei_widgetclass_t*)malloc(sizeof(ei_widgetclass_t));
    char* name = "button";
    strncpy(wclass->name, name, strlen(name)+1);
    wclass->allocfunc = &button_allocfunc;
    wclass->releasefunc = &button_releasefunc;
    wclass->drawfunc = &button_drawfunc;
    wclass->setdefaultsfunc = &button_setdefaults;
    wclass->geomnotifyfunc = &button_geomnotifyfunc;
    wclass->next = NULL;
    ei_widgetclass_register(wclass);
}

void ei_toplevel_register_class(){
    ei_widgetclass_t *wclass = (ei_widgetclass_t*)malloc(sizeof(ei_widgetclass_t));
    char* name = "toplevel";
    strncpy(wclass->name, name, strlen(name)+1);
    wclass->allocfunc = &toplevel_allocfunc;
    wclass->releasefunc = &toplevel_releasefunc;
    wclass->drawfunc = &toplevel_drawfunc;
    wclass->setdefaultsfunc = &toplevel_setdefaults;
    wclass->geomnotifyfunc = &toplevel_geomnotifyfunc;
    wclass->next = NULL;
    ei_widgetclass_register(wclass);
}
