#include "ei_widgetclass.h"
#include "ei_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include "ei_widget.h"
#include <string.h>
#include "ei_new_types.h"
#include "relief.h"
#include "ei_class_button.h"

void refresh_clipper(ei_surface_t surface, ei_rect_t* clipper) {
    ei_linked_rect_t *linked_rect = (ei_linked_rect_t *)malloc(sizeof(ei_linked_rect_t));
    linked_rect->rect = *clipper;
    linked_rect->next = NULL;
    hw_surface_unlock(surface);
    hw_surface_update_rects(surface, linked_rect);
}

void draw_widget_in_offscreen(ei_surface_t pick_surface, ei_widget_t* widget, ei_rect_t* clipper){
    ei_linked_point_t *linked_offscreen = linked_point_from_rect(widget->screen_location);
    ei_color_t color_offscreen = *(widget->pick_color);
    hw_surface_lock(pick_surface);
    ei_draw_polygon(pick_surface, linked_offscreen, color_offscreen, clipper);
    hw_surface_unlock(pick_surface);
}

ei_point_t placement_text_image(ei_widget_t* widget) {
    int text_img_width;
    int text_img_height;
    ei_anchor_t text_img_anchor;
    ei_class_frame_t* widgetclass = (ei_class_frame_t*)widget;

    // Cas où on traite un texte
    if (widgetclass->text) {
        hw_text_compute_size(widgetclass->text, widgetclass->text_font,&text_img_width,&text_img_height);
        text_img_anchor = widgetclass->text_anchor;
    }
    // Cas où on traite une image
    else {
        text_img_width = widgetclass->image_rect->size.width;
        text_img_height = widgetclass->image_rect->size.height;
        text_img_anchor = widgetclass->image_anchor;
    }

    ei_point_t text_img_top_left;
    int widget_width = widget->screen_location.size.width;
    int widget_height = widget->screen_location.size.height;
    int border = widgetclass->border_width;
    ei_point_t widget_top_left = widget->screen_location.top_left;

    // Calcul du top_left de l'image / du texte
    switch (text_img_anchor) {

    case 0:

    case 1:
        text_img_top_left.x = widget_top_left.x + widget_width/2 - text_img_width/2;
        text_img_top_left.y = widget_top_left.y + widget_height/2 - text_img_height/2;
        break;

    case 2:
        text_img_top_left.x = widget_top_left.x + widget_width/2 - text_img_width/2;
        text_img_top_left.y = widget_top_left.y + border;
        break;

    case 3:
        text_img_top_left.x = widget_top_left.x + widget_width - text_img_width - border;
        text_img_top_left.y = widget_top_left.y + border;
        break;

    case 4:
        text_img_top_left.x = widget_top_left.x + widget_width - text_img_width - border;
        text_img_top_left.y =  widget_top_left.y + widget_height/2 - text_img_height/2;
        break;

    case 5:
        text_img_top_left.x = widget_top_left.x + widget_width - text_img_width - border;
        text_img_top_left.y = widget_top_left.y + widget_height - text_img_height - border;
        break;

    case 6:
        text_img_top_left.x = widget_top_left.x + widget_width/2 - text_img_width/2;
        text_img_top_left.y = widget_top_left.y + widget_height - text_img_height - border;
        break;

    case 7:
        text_img_top_left.x = widget_top_left.x +border;
        text_img_top_left.y = widget_top_left.y + widget_height - text_img_height - border;
        break;

    case 8:
        text_img_top_left.x = widget_top_left.x + border;
        text_img_top_left.y =  widget_top_left.y + widget_height/2 - text_img_height/2;
        break;

    case 9:
        text_img_top_left.x = widget_top_left.x + border;
        text_img_top_left.y = widget_top_left.y + border;
        break;

    default:
        break;
    }

    return text_img_top_left;
}

void redraw_text_or_image(ei_widget_t* widget, ei_surface_t surface, ei_rect_t* clipper, ei_bool_t shift) {

    ei_class_button_t* button = (ei_class_button_t*)widget;
    if (button->text) {

        //Recherche du top_left du texte
        ei_point_t text_top_left = placement_text_image(widget);
        if (shift) {
            text_top_left.x += button->border_width - 2;
            text_top_left.y += button->border_width - 2;
        }
        ei_draw_text(surface,&text_top_left,button->text, button->text_font, &button->text_color,clipper);
    }

    // Chargement de l'image dans le widget (si nécessaire)
    else if (button->image) {

        //affichage de l'image
        hw_surface_lock(button->image);
        ei_rect_t image_on_surface;
        image_on_surface.top_left = placement_text_image(widget);
        if (shift) {
            image_on_surface.top_left.x += button->border_width - 2;
            image_on_surface.top_left.y += button->border_width - 2;
        }
        image_on_surface.size = (*(button->image_rect)).size;
        ei_copy_surface(surface,&image_on_surface, button->image, button->image_rect,EI_FALSE);
        hw_surface_unlock(button->image);
    }
}
