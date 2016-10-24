#include <math.h>
#include "ei_widgetclass.h"
#include "ei_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include "ei_widget.h"
#include <string.h>
#include "ei_new_types.h"
#include "ei_class_frame.h"
#include "ei_class_button.h"

ei_linked_point_t *arc(ei_point_t center, uint32_t radius, uint16_t angle_begin, uint16_t angle_end) {
    ei_linked_point_t *linked_point = malloc(sizeof(ei_linked_point_t));
    ei_point_t point;
    angle_begin %= 360;
    double pi = 4*atan(1);
    uint16_t angle = angle_begin;
    float angle_rad = (float)((float)angle)*pi/180;
    point = ei_point(center.x + radius*cos(angle_rad), center.y + radius*sin(angle_rad));
    linked_point->point = point;
    ei_linked_point_t *current = linked_point;
    int x = point.x;
    int y = point.y;
    while (angle < angle_end & angle <= 360) {
	angle += 1;
	angle_rad = (float)((float)angle)*pi/180.0;
	x = center.x + radius * cos(angle_rad);
	y = center.y + radius * sin(angle_rad);
	current->next = malloc(sizeof(ei_linked_point_t));
	current = current->next;
	current->point = ei_point(x,y);
    }
    current->next = NULL;
    return linked_point;
}

ei_linked_point_t *tail_linked_point(ei_linked_point_t *linked_point) {
    ei_linked_point_t *tail_linked = linked_point;
    while (tail_linked->next){
	tail_linked = tail_linked->next;
    }
    return tail_linked;
}

ei_linked_point_t *add_arc(ei_linked_point_t *current, uint32_t radius, uint32_t center_x, uint32_t center_y, uint16_t angle_begin, uint16_t angle_end) {
    ei_point_t center = ei_point(center_x, center_y);
    ei_linked_point_t *arc_obj = arc(center, radius, angle_begin, angle_end);
    current->next = arc_obj;
    current = tail_linked_point(arc_obj);
    return current;
}

ei_linked_point_t *add_point(ei_linked_point_t *current, int x, int y) {
    current->next = malloc(sizeof(ei_linked_point_t));
    current = current->next;
    current->point = ei_point(x,y);
    return current;
}

ei_linked_point_t *add_straight(ei_linked_point_t *current, int x1, int y1, int x2, int y2){
    current = add_point(current, x1, y1);
    current = add_point(current, x2, y2);
    return current;
}

ei_linked_point_t *rounded_frame(ei_rect_t *rect, uint32_t radius,ei_bool_t high, ei_bool_t low) {
    ei_linked_point_t *linked_point = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t *current = linked_point;

    //Recherche du point au milieu de l'arc en bas à gauche
    int x = rect->top_left.x + radius;
    int y = rect->top_left.y + rect->size.height;
    int a,b;
    ei_point_t center = ei_point(x, y-radius);
    ei_linked_point_t *arc_obj = arc(center, radius, 90, 135);
    linked_point->point = tail_linked_point(arc_obj)->point;

    if (high) {
	arc_obj = arc(center,radius, 135, 180);
	current->next = arc_obj;
	current = tail_linked_point(arc_obj);

	a = rect->top_left.x;
	b = rect->top_left.y + radius;

	current = add_arc(current, radius, a+radius, b, 180, 270);

	a = rect->top_left.x + rect->size.width - radius;
	b = rect->top_left.y;

	current = add_arc(current, radius, a, b+radius, 270, 315);

	if (!low) {
	    x = rect->top_left.x + rect->size.width - rect->size.height/2;
	    y = rect->top_left.y + rect->size.height/2;
	    a = rect->top_left.x + rect->size.height/2;
	    b = rect->top_left.y + rect->size.height/2;
	    current = add_straight(current, x, y, a, b);

	    current = add_point(current, linked_point->point.x, linked_point->point.y);
	}
	else {
	    current = add_arc(current, radius, a, b+radius, 315, 360);

	    a = rect->top_left.x + rect->size.width;
	    b = rect->top_left.y + rect->size.height - radius;

	    current = add_arc(current, radius, a-radius, b, 0, 90);

	    a = rect->top_left.x + radius;
	    b = rect->top_left.y + rect->size.height;

	    current = add_arc(current, radius, a, b-radius, 90, 135);

	}
    }
    else if (low) {
	x = rect->top_left.x + rect->size.height/2;
	y = rect->top_left.y + rect->size.height/2;
	a = rect->top_left.x + rect->size.width - rect->size.height/2;
	b = rect->top_left.y + rect->size.height/2;
	current = add_straight(current, x, y, a, b);

	//Recherche du point au milieu de l'arc en haut à gauche
	x = rect->top_left.x + rect->size.width - radius;
	y = rect->top_left.y;
	center = ei_point(x, y+radius);
	arc_obj = arc(center, radius, 270, 315);
	current = add_point(current, tail_linked_point(arc_obj)->point.x, tail_linked_point(arc_obj)->point.y);

	current = add_arc(current, radius, x, y+radius, 315, 360);

	a = rect->top_left.x + rect->size.width;
	b = rect->top_left.y + rect->size.height - radius;

	current = add_arc(current, radius, a-radius, b, 0, 90);

	a = rect->top_left.x + radius;
	b = rect->top_left.y + rect->size.height;

	current = add_arc(current, radius, a, b-radius, 90, 135);
    }
    current->next = NULL;
    return linked_point;
}

ei_linked_point_t *top_rounded_frame(ei_rect_t *rect, uint32_t radius) {
    ei_linked_point_t *linked_point = malloc(sizeof(ei_linked_point_t));
    ei_linked_point_t *current = linked_point;
    //Recherche du point au milieu de l'arc en bas à gauche
    int x = rect->top_left.x;
    int y = rect->top_left.y + rect->size.height;
    linked_point->point = ei_point(x,y);
    int a = rect->top_left.x;
    int b = rect->top_left.y + radius;
    current = add_straight(current, x, y, a, b);

    current = add_arc(current, radius, a+radius, b, 180, 270);

    x = rect->top_left.x + radius;
    y = rect->top_left.y;
    a = rect->top_left.x + rect->size.width - radius;
    b = rect->top_left.y;
    current = add_straight(current, x, y, a, b);

    current = add_arc(current, radius, a, b+radius, 270, 360);

    x = rect->top_left.x + rect->size.width;
    y = rect->top_left.y + radius;
    a = rect->top_left.x + rect->size.width;
    b = rect->top_left.y + rect->size.height;
    current = add_straight(current, x, y, a, b);

    x = rect->top_left.x;
    y = rect->top_left.y + rect->size.height;
    current = add_straight(current, a, b, x, y);

    current->next = NULL;
    return linked_point;
}

ei_color_t darker_color(ei_color_t color) {
    ei_color_t new_color = color;
    new_color.red  = new_color.red < 30 ? 0 : new_color.red - 30;
    new_color.green  = new_color.green < 30 ? 0 : new_color.green - 30;
    new_color.blue  = new_color.blue < 30 ? 0 : new_color.blue - 30;
    return new_color;
}

ei_color_t lighter_color(ei_color_t color) {
    ei_color_t new_color = color;
    new_color.red  = new_color.red > 225 ? 255 : new_color.red + 30;
    new_color.green  = new_color.green > 255 ? 255 : new_color.green + 30;
    new_color.blue  = new_color.blue > 225 ? 255 : new_color.blue + 30;
    return new_color;
}

void draw_frame_or_button_relief(ei_widget_t *widget, ei_surface_t surface, ei_rect_t* clipper, ei_bool_t actionned) {

    ei_class_button_t *object = (ei_class_button_t*)widget;
    int corner_radius = 0;
    if (!strcmp(ei_widgetclass_stringname(widget->wclass->name),"button")) {
    	corner_radius = object->corner_radius;
    }
    ei_linked_point_t* linked_surface = rounded_frame(&widget->screen_location, corner_radius, EI_TRUE, EI_TRUE);
    ei_color_t color_surface = object->color;
    hw_surface_lock(surface);

    if (object->relief == ei_relief_raised || object->relief == ei_relief_sunken) {
	ei_linked_point_t* border_top = rounded_frame(&widget->screen_location, corner_radius, EI_TRUE, EI_FALSE);
	ei_linked_point_t *border_bottom = rounded_frame(&widget->screen_location, corner_radius, EI_FALSE, EI_TRUE);
	ei_rect_t rect_small = widget->screen_location;
	rect_small.size.width -= object->border_width * 2;
	rect_small.size.height -= object->border_width * 2;
	rect_small.top_left.x += object->border_width;
	rect_small.top_left.y += object->border_width;
	ei_linked_point_t *linked_surface_small = rounded_frame(&rect_small, corner_radius, EI_TRUE, EI_TRUE);
	ei_color_t color_border_top;
	ei_color_t color_border_bottom ;
    	if (((object->relief == ei_relief_sunken) & !actionned) | ((object->relief == ei_relief_raised) & actionned)) {
	    color_border_top = darker_color(color_surface);
	    color_border_bottom = lighter_color(color_surface);
	}
	else {
	    color_border_bottom = darker_color(color_surface);
	    color_border_top = lighter_color(color_surface);
	}
	ei_draw_polygon(surface, border_top, color_border_top, clipper);
	ei_draw_polygon(surface, border_bottom, color_border_bottom, clipper);
	ei_draw_polygon(surface, linked_surface_small, color_surface, clipper);
    }

    else {
	ei_draw_polygon(surface, linked_surface, color_surface, clipper);
    }
}
