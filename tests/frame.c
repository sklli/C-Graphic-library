#include <stdio.h>
#include <stdlib.h>

#include "ei_application.h"
#include "ei_event.h"
#include "hw_interface.h"
#include "ei_widget.h"
#include "ei_geometrymanager.h"
#include "ei_draw_widget.h"

ei_bool_t process_key(ei_widget_t* widget, ei_event_t* event, void* user_param)
{
    if (event->param.key.key_sym == SDLK_ESCAPE) {
        ei_app_quit_request();
        return EI_TRUE;
    }
	
    return EI_FALSE;
}

/*
 * ei_main --
 *
 *	Main function of the application.
 */
int ei_main(int argc, char** argv)
{
	ei_size_t	screen_size		= {600, 600};
	ei_color_t	root_bgcol		= {0x52, 0x7f, 0xb4, 0xff};

	ei_widget_t*	frame;
	ei_size_t	frame_size		= {300,200};
	int		frame_x			= 150;
	int		frame_y			= 200;
	ei_color_t	frame_color		= {0x88, 0x88, 0x88, 0xff};
	ei_relief_t	frame_relief		= ei_relief_raised;
	int		frame_border_width	= 6;

        ei_widget_t*    button;
        ei_size_t       button_size             = {100,50};
        float           button_x_rel            = 0.5;
        float           button_y_rel            = 0.5;
        ei_color_t      button_color            = {0,120,0,255};
        ei_relief_t     button_relief           = ei_relief_raised;
        int             button_border_width      = 3;
	/* Create the application and change the color of the background. */
	ei_app_create(&screen_size, EI_FALSE);
	ei_frame_configure(ei_app_root_widget(), NULL, &root_bgcol, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

	/* Create, configure and place the frame & button on screen. */
	frame = ei_widget_create("frame", ei_app_root_widget());
        button = ei_widget_create("button", frame);
        // Texte du frame :
        char* frame_text = "Frame";
        ei_anchor_t frame_anchor = ei_anc_north;

        // Texte du bouton :
        /* char* button_text = "Button"; */
        /* ei_anchor_t button_anchor = ei_anc_center;  */
        /* ei_anchor_t button_anchor_2 = ei_anc_northwest; */

        // Image de bouton :
        ei_surface_t *button_image = malloc(sizeof(ei_surface_t));
        char *photo = "Wendy.png";
        *button_image = hw_image_load(photo,ei_app_root_surface());
        hw_surface_lock(*button_image);
        ei_rect_t image_rect = hw_surface_get_rect(*button_image);
        hw_surface_unlock(*button_image);
        ei_rect_t* button_image_rect = &(image_rect);
        ei_anchor_t button_image_anchor = ei_anc_center;

	ei_frame_configure(frame, &frame_size, &frame_color, &frame_border_width, &frame_relief, &frame_text, NULL, NULL, &frame_anchor, NULL, NULL, NULL);

        ei_button_configure(button, &button_size, &button_color, &button_border_width, NULL, &button_relief, /*&button_text*/NULL, NULL, NULL, /*&button_anchor*/NULL, button_image/*NULL*/, &button_image_rect/*NULL*/, &button_image_anchor/*NULL*/, NULL, NULL);
	ei_place(frame, NULL, &frame_x, &frame_y, NULL, NULL, NULL, NULL, NULL, NULL );
        ei_place(button, NULL, NULL, NULL, NULL, NULL, &button_x_rel, &button_y_rel, NULL, NULL);
        ei_bind(ei_ev_keydown,		NULL, "all", process_key, NULL);
	/* Run the application's main loop. */
	ei_app_run();
	/* We just exited from the main loop. Terminate the application (cleanup). */
        ei_unbind(ei_ev_keydown,	NULL, "all", process_key, NULL);
	ei_app_free();

	return (EXIT_SUCCESS);
}
