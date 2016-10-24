/**
 *  @file	ei_draw_widget.h
 *
 *  @brief	Fonctions to help creating and drawing widgets.
 *
 */

#ifndef EI_DRAW_WIDGET_H
#define EI_DRAW_WIDGET_H

#include "ei_widgetclass.h"
#include "ei_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include "ei_widget.h"
#include <string.h>
#include "ei_new_types.h"
#include "relief.h"
#include "ei_class_button.h"

/**
 * \brief	Draws a filled polygon.
 *
 * @param	surface 	The surface to refresh.
 * @param	clipper		The drawing is restricted within this rectangle.
 */
void refresh_clipper(ei_surface_t surface, ei_rect_t* clipper);

/**
 * \brief	Draws a widget in the offscreen and on the screen.
 *
 * @param	surface		The surface on which the widget will be drawn.
 * @param	widget		The widget to draw.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle.
 */
void draw_widget_in_offscreen(ei_surface_t pick_surface, ei_widget_t* widget, ei_rect_t* clipper);

/**
 * \brief	Calulates the coordinates of the future top_left of a text or an image.
 *
 * @param	widget		A pointer to the widget in which we want to add a text or an image.
 *
 * @return 			A point which will be the top_left of a text or of an image.
 */
ei_point_t placement_text_image(ei_widget_t* widget);

/**
 * \brief	A function that redraws texts or images of a widget.
 *
 * @param	widget		A pointer to the widget instance to draw.
 * @param	surface		Where to draw the widget. The actual location of the widget in the
 *				surface is stored in its "screen_location" field.
 * @param	clipper		If not NULL, the drawing is restricted within this rectangle
 *				(expressed in the surface reference frame).
 * @param       shift           A boolean which will be true if the text or the image has to be
 *                              shifted due to a pressed aspect, and will be false in the other case.
 */
void redraw_text_or_image(ei_widget_t* widget,
                          ei_surface_t surface,
                          ei_rect_t*   clipper,
                          ei_bool_t    shift);

#endif
