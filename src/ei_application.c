#include "ei_types.h"
#include "ei_widget.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "ei_new_types.h"
#include <assert.h>
#include "ei_event.h"

ei_widget_t *widget_root;
ei_surface_t main_surface;
ei_surface_t main_offscreen;
ei_bool_t exit_program;

struct class_cell *registered_class;
struct geo_cell *registered_geo;
struct color_cell *registered_color;
ei_linked_tag_t *registered_tag;
struct event_type_list *registered_event_type[8];

void ei_app_create(ei_size_t* main_window_size, ei_bool_t fullscreen) {

    hw_init();
    init_registers();

    main_surface = hw_create_window(main_window_size,fullscreen);
    main_offscreen = hw_surface_create(main_surface, main_window_size, EI_FALSE);

    /* Enregistrement des classes de widgets */
    ei_frame_register_class();
    ei_button_register_class();
    ei_toplevel_register_class();
    /* Enregistrement des managers de géométrie */
    ei_register_placer_manager();

    /* Création de la fenêtre racine (root) */
    widget_root = ei_widget_create("frame",NULL);
    ei_size_t root_size = {500,500};
    ei_color_t blue = { 0x00, 0x7f, 0xff, 0xff };
    ei_frame_configure(widget_root, &root_size, &blue, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
    widget_root->screen_location = hw_surface_get_rect(main_surface);
}

void ei_app_free() {
    (*widget_root->wclass->releasefunc)(widget_root);
    hw_quit();
}

void recursive_draw(ei_widget_t *widget, const ei_surface_t surface, const ei_surface_t offscreen){
    (*(widget->wclass->drawfunc))(widget, surface, offscreen, widget->content_rect);
    if (widget->children_head) {
	ei_widget_t *current = widget->children_head;
	recursive_draw(current, surface, offscreen);
        while (current->next_sibling){
            current = current->next_sibling;
            recursive_draw(current, surface, offscreen);
        }
    }
}

void ei_app_quit_request(){
    exit(0);
}

ei_surface_t ei_app_root_widget() {
    return widget_root;
}

ei_surface_t ei_app_root_surface() {
    return main_surface;
}

ei_surface_t ei_app_root_offscreen() {
    return main_offscreen;
}

void ei_app_invalidate_rect(ei_rect_t* rect) {
    ei_linked_rect_t *linked_rect = (ei_linked_rect_t *)malloc(sizeof(ei_linked_rect_t));
    linked_rect->rect = *rect;
    linked_rect->next = NULL;
    hw_surface_update_rects(ei_app_root_surface(), linked_rect);
}

void mouse_processing(ei_event_t* event, ei_eventtype_t i) {

    ei_point_t pixel_picked = event->param.mouse.where;
    ei_widget_t* chosen_widget = ei_widget_pick(&pixel_picked);

    struct tag_cell* current_tag;
    struct widget_cell* current_widget;
    ei_tag_t chosen_widget_class_tag;

    /* Si on clique sur un widget... */
    if (chosen_widget) {

        // Gestion des tags liés à l'événement
        chosen_widget_class_tag = (ei_tag_t)chosen_widget->wclass->name;
        current_tag = registered_event_type[i]->tag_and_callback;
        while (current_tag) {
            if (!strcmp(current_tag->tag,chosen_widget_class_tag) || !strcmp(current_tag->tag,"all")) {
                (*(current_tag->callback))(chosen_widget,event,NULL);
            }
            current_tag = current_tag->next;
        }

        // Gestion des widgets liés à l'événement
        current_widget = registered_event_type[i]->widget_and_callback;
        while (current_widget) {
            if (current_widget->widget == chosen_widget) {
                (*(current_widget->callback))(chosen_widget,event,NULL);
            }
            current_widget = current_widget->next;
        }
    }
}

void keyboard_processing(ei_event_t* event, ei_eventtype_t i) {

    struct tag_cell* current_tag;
    struct widget_cell* current_widget;

    // Gestion des tags liés à l'événement
    current_tag = registered_event_type[i]->tag_and_callback;

    while (current_tag) {
        (*(current_tag->callback))(NULL,event,NULL);
        current_tag = current_tag->next;
    }

    current_widget = registered_event_type[i]->widget_and_callback;

    while (current_widget) {
        (*(current_widget->callback))(registered_event_type[2]->widget_and_callback->widget,event,NULL);
        current_widget = current_widget->next;
    }
}

void ei_app_run() {

    hw_surface_lock(ei_app_root_surface());
    recursive_draw(widget_root, main_surface, main_offscreen);
    hw_surface_unlock(ei_app_root_surface());

    struct ei_event_t* event = malloc(sizeof(ei_event_t));

    // Boucle principale.
    while (EI_TRUE) {

        hw_event_wait_next(event);

        switch (event->type) {

        case 2:
            /* ------------------------------- */
            /* Appui sur une touche du clavier */
            /* ------------------------------- */

            keyboard_processing(event, 2);
            break;

        case 3:
            /* ----------------------------------- */
            /* Relâchement d'une touche du clavier */
            /* ----------------------------------- */

            keyboard_processing(event, 3);
            break;

        case 4:
            /* ----------------- */
            /* Clic de la souris */
            /* ----------------- */

            mouse_processing(event, 4);
            break;

        case 5:
            /* ---------------------------------- */
            /* Relâchement d'un clic de la souris */
            /* ---------------------------------- */

            mouse_processing(event, 5);
            break;

        case 6:
            /* ----------------------------------- */
            /* Déplacement du curseur de la souris */
            /* ----------------------------------- */
            
            mouse_processing(event, 6);
            break;

        default:
            break;
        }
    }
}
