/**
 * @file	ei_class_button.h
 *
 * @brief 	Defines a button.
 *
 */

#ifndef EI_CLASS_BUTTON_H
#define EI_CLASS_BUTTON_H

#include "ei_widgetclass.h"
#include "ei_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include "ei_widget.h"
#include <string.h>
#include "ei_new_types.h"
#include "relief.h"

/**
 * \brief	A structure that stores information about a button.
 */
typedef struct ei_class_button_t {
    ei_widget_t*                 widget;
    uint32_t                     pick_id;
    ei_color_t*                  pick_color;
    struct ei_widget_t*          parent;
    struct ei_widget_t*          children_head;
    struct ei_widget_t*          children_tail;
    struct ei_widget_t*          next_sibling;
    struct ei_geometry_param_t*  geom_params;
    ei_size_t                    requested_size;
    ei_rect_t                    screen_location;
    ei_rect_t*                   content_rect;
    /* Spécificités widget frame (également présents dans button) */
    ei_color_t	                 color;
    int 			 border_width;
    ei_relief_t 		 relief;
    char*			 text;
    ei_font_t		         text_font;
    ei_color_t		         text_color;
    ei_anchor_t 		 text_anchor;
    ei_surface_t		 image;
    ei_rect_t*		         image_rect;
    ei_anchor_t 		 image_anchor;
    /* Spécificités widget button */
    int                          corner_radius;
    ei_callback_t                callback;
    void*                        user_param;
}ei_class_button_t;

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a button. After allocation, the function *must*
 *		initialize the memory to 0.
 *
 * @return		A block of memory with all bytes set to 0.
 */
void* button_allocfunc();

/**
 * \brief	A function that releases the memory used by a widget before it is destroyed.
 *		The \ref ei_widget_t structure itself, passed as parameter, must *not* by freed by
 *		these functions. Can be set to NULL in \ref ei_widgetclass_t if no memory is used
 *              by the button class.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void button_releasefunc(struct ei_widget_t* widget);

/**
 * \brief	A function that draws a button.
 *
 * @param	widget		A pointer to the widget instance to draw.
 * @param	surface		Where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void button_drawfunc (struct ei_widget_t*	widget,
		     ei_surface_t		surface,
		     ei_surface_t		pick_surface,
		     ei_rect_t*		clipper);

/**
 * \brief	A function that sets the default values for a button.
 *
 * @param	widget		A pointer to the widget instance to intialize.
 */
void button_setdefaults (struct ei_widget_t*	widget);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 * @param	rect		The new rectangular screen location of the widget
 *				(i.e. = widget->screen_location).
 */
void button_geomnotifyfunc (struct ei_widget_t*	widget,
                           ei_rect_t		rect);

#endif
