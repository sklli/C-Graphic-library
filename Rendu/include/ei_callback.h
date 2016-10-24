/**
 * @file	ei_callback.h
 *
 * @brief	Functions to treat the callbacks.
 *
 */

#ifndef EI_CALLBACK_H
#define EI_CALLBACK_H

#include "ei_widget.h"
#include "ei_widgetclass.h"
#include "ei_event.h"
#include "relief.h"
#include "ei_application.h"
#include "ei_class_button.h"
#include "ei_draw_widget.h"

/**
 * @brief 	The callback that is called when there is a mouse_buttondown event on a button.
 *
 * @param 	widget 	        The widget for which the event was generated.
 * @param 	event   	The event containing all its parameters (type, etc.).
 * @param       user_param      The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return 			A boolean telling if the event was consumed by the callback or not.
 *				If TRUE, the library does not try to call other callbacks for this
 *				event. If FALSE, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				FALSE, or return TRUE without having done anything.
 */
ei_bool_t button_mouse_button_down(ei_widget_t* widget, ei_event_t* event, void* user_param);

/**
 * @brief	The callback that is called when a toplevel is moved or resized. It is linked to the
 *              tag "toplevel".
 *
 * @param	widget		The widget for which the event was generated.
 * @param	event		The event containing all its parameters (type, etc.)
 * @param	user_param	The user parameters that was provided by the caller when registering
 *				this callback.
 *
 * @return			A boolean telling if the event was consumed by the callback or not.
 *				If TRUE, the library does not try to call other callbacks for this
 *				event. If FALSE, the library will call the next callback registered
 *				for this event, if any.
 *				Note: The callback may execute many operations and still return
 *				FALSE, or return TRUE without having done anything.
 */
ei_bool_t toplevel_processing(ei_widget_t* widget, ei_event_t* event, void* user_param);

#endif
