/**
 *  @file	ei_new_types.h
 *
 *  @brief	New types and fonctions created to register classes, geometry managers, tags, and to
 *	        draw something.
 *
 */

#ifndef EI_NEW_TYPES_H
#define EI_NEW_TYPES_H

#include "ei_widget.h"
#include "ei_widgetclass.h"
#include <stdio.h>
#include <stdlib.h>
#include "hw_interface.h"
#include "ei_draw.h"
#include "ei_new_types.h"
#include "ei_class_frame.h"
#include "ei_geometrymanager.h"
#include "ei_event.h"

/**
 * \brief	A function that returns the linkedpoints which describes the rect_t passed in
 *              parameter.
 *
 * @param	rect	The rect from which we want to have the linked points.
 *
 * @return		Returns the linked points that describes the rect.
 */
ei_linked_point_t *linked_point_from_rect(ei_rect_t rect);

/**
 * \brief	A structure that stores a class.
 */
struct class_cell {
    ei_widgetclass_t* wclass;
    struct class_cell* next;
};

/**
 * \brief	A structure that stores a geometry manager.
 */
struct geo_cell {
    ei_geometrymanager_t* manager;
    struct geo_cell* next;
};

/**
 * \brief	A structure that stores a color.
 */
struct color_cell {
    ei_color_t   color;
    struct color_cell*  next;
};

/**
 * \brief	A structure that stores a tag, its callback and its user parameters.
 */
struct tag_cell {
    ei_tag_t tag;
    ei_callback_t callback;
    void* user_param;
    struct tag_cell* next;
};

/**
 * \brief	A structure that stores a widget, its callback and its user parameters.
 */
struct widget_cell {
    ei_widget_t* widget;
    ei_callback_t callback;
    void* user_param;
    struct widget_cell* next;
};

/**
 * \brief	A structure that stores an event type.
 */
// Liste des types d'event
struct event_type_list {
    // Event en question
    ei_eventtype_t event;
    // Liste des tags associés à cet event, avec leur callback
    struct tag_cell* tag_and_callback;
    // Liste des widgets associés à cet event
    struct widget_cell* widget_and_callback;
};

/**
 * \brief	A structure that stores the registered classes.
 */
extern struct class_cell *registered_class;

/**
 * \brief	A structure that stores the registered geometry managers.
 */
extern struct geo_cell *registered_geo;

/**
 * \brief	A structure that stores the registered colors.
 */
extern struct color_cell *registered_color;

/**
 * \brief	A structure that stores the registered tags.
 */
extern ei_linked_tag_t *registered_tag;

/**
 * \brief	A structure that stores the registered event types.
 */
extern struct event_type_list *registered_event_type[8];

/**
 * \brief	A function that registers by default registered_class, registered_geo,
 *              registered_color, registered_tag, registered_event_type and the default callbacks.
 */
extern void init_registers();

/**
 * \brief	A structure that stores information about a placer.
 */
typedef struct ei_placer_t {
    ei_geometrymanager_t* manager;
    ei_widget_t*	  widget;
    ei_anchor_t	          anchor;
    int		          x;
    int		          y;
    int		          width;
    int		          height;
    float		  rel_x;
    float		  rel_y;
    float		  rel_width;
    float		  rel_height;
}ei_placer_t;

/**
 * \brief	A function that sets the default values for a placer.
 *
 * @param	widget	A pointer to the widget from which the placer has to be initialized.
 */
void ei_placer_setdefaults (ei_widget_t* widget);

/**
 * \brief	A function that tests if a color is already taken by a widget or not.
 *
 * @param	color	The color to test.
 *
 * @return		Returns True if the color is already taken, False if not.
 */
extern ei_bool_t color_is_taken(ei_color_t color);

/**
 * @brief	Registers a color to the program so that this color will be used only once.
 *
 * @param	color	The color used in the offscreen.
 */
extern void color_register(ei_color_t color);

/**
 * \brief	A function that returns the offscreen of the main surface. Written in
 *              \ref ei_application.c.
 *
 * @return		Returns the offscreen of the main surface.
 */
ei_surface_t ei_app_root_offscreen();

/**
 * \brief	A function that draws widgets of a class.
 *
 * @param	widget		A pointer to the widget instance to draw. Written in
 *                              \ref ei_application.c.
 * @param	surface		Where to draw the widget, its siblings and descendants. The actual
 *                              location of the widget in the surface is stored in its
 *                              "screen_location" field.
 * @param	offscreen	The offscreen on which the widget, its siblings and descendants will
 *                              be drawn.
 */
void recursive_draw(ei_widget_t *widget, const ei_surface_t surface, const ei_surface_t offscreen);

#endif
