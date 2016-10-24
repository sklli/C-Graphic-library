/**
 * @file	relief.h
 *
 * @brief 	Useful functions to create relief.
 *
 */

#ifndef EI_ARC_H
#define EI_ARC_H
#include <math.h>
#include "ei_widgetclass.h"
#include "ei_utils.h"
#include <stdlib.h>
#include <stdio.h>
#include "ei_widget.h"
#include <string.h>
#include "ei_new_types.h"
#include "ei_class_frame.h"

/**
 * @brief 	Creates linked points to draw an arc.
 *
 * @param 	center 		Number of horizontal and vertical pixels.
 * @param 	radius  	Radius of the arc.
 * @param       angle_begin     The angle at which the arc begins (orthogonal sense for x and y).
 * @param       angle_end       The angle at which the arc ends.
 *
 * @return 			Linked points that will be used to draw an arc.
 */
extern ei_linked_point_t *arc (ei_point_t  center,
                               uint32_t    radius,
                               uint16_t    angle_begin,
                               uint16_t    angle_end);

/**
 * @brief 	Adds an arc to an existing list of linked points.
 *
 * @param 	center 		Number of horizontal and vertical pixels.
 * @param 	radius  	Radius of the arc.
 * @param       angle_begin     The angle at which the arc begins (orthogonal sense for x and y).
 * @param       angle_end       The angle at which the arc ends.
 *
 * @return 			Linked points that contain the new arc in the end.
 */
ei_linked_point_t *add_arc (ei_linked_point_t *current,
                            uint32_t          radius,
                            uint32_t          center_x,
                            uint32_t          center_y,
                            uint16_t          angle_begin,
                            uint16_t          angle_end);

/**
 * @brief 	Creates linked points to draw a form with round corners.
 *
 * @param 	rect 		The rectangle of the form.
 * @param 	radius  	Radius of the corners that make the form.
 * @param       high            If True, the top part of the form is created.
 * @param       low             If True, the bottom part of the form is created.
 *
 * @return 			Linked points that will be used to draw the rounded form.
 */
extern ei_linked_point_t *rounded_frame (ei_rect_t *rect,
                                         uint32_t  radius,
                                         ei_bool_t high,
                                         ei_bool_t low);

/**
 * @brief 	Creates linked points to draw a form with round corners at the top only.
 *
 * @param 	center 		Number of horizontal and vertical pixels.
 * @param 	radius  	Radius of the corners that make the form.
 *
 * @return 			Linked points that will be used to draw the form.
 */
ei_linked_point_t *top_rounded_frame(ei_rect_t *rect, uint32_t radius);

/**
 * @brief 	Return a darker color.
 *
 * @param 	color 		The color which will be made darker.
 *
 * @return 			The new darker color.
 */
extern ei_color_t darker_color(ei_color_t color);

/**
 * @brief 	Return a lighter color.
 *
 * @param 	color 		The color which will be made lighter.
 *
 * @return 			The new lighter color.
 */ 
extern ei_color_t lighter_color(ei_color_t color);

/**
 * @brief 	Draws a frame or button according to the given parameters.
 *
 * @param 	widget          The widget that will be drawn.
 * @param 	surface         The surface on which the form will be drawn.
 * @param       clipper         The clipper where the form can be drawn.
 * @param       actionned       If True, the form will be drawn with a relief opposite to what is
 *                              specified in the widget.
 */
extern void draw_frame_or_button_relief (ei_widget_t   *widget,
                                         ei_surface_t  surface,
                                         ei_rect_t*    clipper,
                                         ei_bool_t     actionned);

#endif
