/**
 *  @file	ei_class_toplevel.h
 *
 *  @brief	Defines a toplevel.
 *
 */

#ifndef EI_CLASS_TOPLEVEL_H
#define EI_CLASS_TOPLEVEL_H

#include "ei_widgetclass.h"
#include "ei_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include "ei_widget.h"
#include <string.h>
#include "ei_new_types.h"
#include "relief.h"

/**
 * \brief	A structure that stores information about a toplevel.
 */
typedef struct ei_class_toplevel_t {
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
    //Attributs spécifiques
    ei_color_t		         color;
    int	   		         border_width;
    char* 		 	 title;
    ei_bool_t		         closable;
    ei_axis_set_t		 resizable;
    ei_size_t*	                 min_size;
}ei_class_toplevel_t;

/**
 * \brief	A function that allocates a block of memory that is big enough to store the
 *		attributes of a top_level. After allocation, the function *must*
 *		initialize the memory to 0.
 *
 * @return		A block of memory with all bytes set to 0.
 */
void* toplevel_allocfunc();

/**
 * \brief	A function that releases the memory used by a top_level before it is destroyed.
 *		The \ref ei_widget_t structure itself, passed as parameter, must *not* by freed by
 *		these functions. Can be set to NULL in \ref ei_widgetclass_t if no memory is used
 *              by the top_level class.
 *
 * @param	widget		The widget which resources are to be freed.
 */
void toplevel_releasefunc(struct ei_widget_t* widget);

/**
 * \brief	A function that draws top_level widgets.
 *
 * @param	widget		A pointer to the widget instance to draw.
 * @param	surface		Where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 */
void toplevel_drawfunc (struct ei_widget_t*	widget,
		     ei_surface_t		surface,
		     ei_surface_t		pick_surface,
		     ei_rect_t*		clipper);

/**
 * \brief	A function that sets the default values for a top_level.
 *
 * @param	widget		A pointer to the widget instance to intialize.
 */
void toplevel_setdefaults (struct ei_widget_t*	widget);

/**
 * \brief 	A function that is called to notify the widget that its geometry has been modified
 *		by its geometry manager. Can set to NULL in \ref ei_widgetclass_t.
 *
 * @param	widget		The widget instance to notify of a geometry change.
 * @param	rect		The new rectangular screen location of the widget
 *				(i.e. = widget->screen_location).
 */
void toplevel_geomnotifyfunc (struct ei_widget_t*	widget,
                               ei_rect_t		rect);

#endif
