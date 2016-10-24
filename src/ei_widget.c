#include "ei_widgetclass.h"
#include "ei_types.h"
#include "ei_widget.h"
#include <stdlib.h>
#include <string.h>
#include "ei_class_frame.c"
#include "ei_class_button.h"
#include "ei_widgetclass.c"
#include "ei_new_types.h"
#include "ei_draw_widget.h"
#include "ei_application.h"
#include "ei_callback.h"

/* ----------------------------------------------------------------------- */
/* ------------------ PROGRAMMES INTERMEDIAIRES -------------------------- */
/* ----------------------------------------------------------------------- */

// Widget qui sera retourné par la fonction ei_widget_pick
ei_widget_t* widget_picked = NULL;

/* Fonction qui renvoie true si le point where appartient au widget,
 * ou false sinon. */
ei_bool_t point_on_widget(ei_widget_t* widget, ei_point_t where) {
    // Variables introduites pour faciliter la lecture de la dernière ligne.
    int w_top_left_x = widget->screen_location.top_left.x;
    int w_top_left_y = widget->screen_location.top_left.y;
    int w_width = widget->screen_location.size.width;
    int w_height = widget->screen_location.size.height;

    /* Avec (x,y) le point haut gauche du widget et (x0,y0) le point where,
     * le point est placé sur le widget ssi (x <= x0 <= x + width) et
     * (y <= y0 <= y + height). */

    return ((w_top_left_x <= where.x) && (where.x <= w_top_left_x + w_width) && (w_top_left_y <= where.y) && (where.y <= w_top_left_y + w_height));
}

void which_widget(ei_widget_t* widget, ei_point_t where) {
    if (widget) {
	    if (point_on_widget(widget,where)) {
		widget_picked = widget;
		if (widget->children_head) {
		    ei_widget_t* kid = widget->children_head;
		    which_widget(kid, where);
		    while (kid->next_sibling) {
			kid = kid->next_sibling;
			which_widget(kid,where);
		    }
		}
	    }
	}
}

ei_widget_t*		ei_widget_pick			(ei_point_t*		where){
    ei_widget_t* first_widget = ei_app_root_widget();
    widget_picked = NULL;
    which_widget(first_widget, *where);
    return widget_picked;
}

/* Chaîne le widget tout récemment créé au bon endroit dans la généalogie des
 * widgets */
void placement_new_widget (ei_widget_t* new_widget, ei_widget_t* parent) {
    if (parent->children_head == NULL) {
        parent->children_head = new_widget;
        parent->children_tail = new_widget;
    } else {
        ei_widget_t *kid = parent->children_head;
	while (kid->next_sibling != NULL) {
	    kid = kid->next_sibling;
	}
	kid->next_sibling = new_widget;
        parent->children_tail = new_widget;
    }
}

/* ----------------------------------------------------------------------- */
/* ----------------------------------------------------------------------- */
/* ----------------------------------------------------------------------- */

ei_widget_t* ei_widget_create (ei_widgetclass_name_t	class_name,
			       ei_widget_t*		parent) {
    /* Recherche de la classe associée */
    ei_widgetclass_t *wclass = ei_widgetclass_from_name(class_name);

    if (wclass == NULL) {
        printf("Cette classe n'existe pas\n");
        exit(EXIT_FAILURE);
    }

    /* Création du widget : */
    ei_widget_t* new_widget = (*wclass->allocfunc)();
    new_widget->pick_color = malloc(sizeof(ei_color_t));
    new_widget->geom_params = malloc(sizeof(struct ei_geometry_param_t));
    new_widget->geom_params->manager = NULL;
    new_widget->content_rect = malloc(sizeof(ei_rect_t));

    new_widget->wclass = wclass;
    ei_placer_setdefaults(new_widget);
    (*(wclass->setdefaultsfunc))(new_widget);
    new_widget->parent = parent;
    new_widget->children_head = NULL;
    new_widget->children_tail = NULL;
    new_widget->next_sibling = NULL;
    /* Si ce n'est pas la fenêtre racine... */
    if (parent) {
            placement_new_widget(new_widget,parent);
    }
        return new_widget;
 }

void			ei_widget_destroy		(ei_widget_t*		widget){
    if (widget) {
	ei_widget_t *brother = widget->parent->children_head;
	if (widget == widget->parent->children_head & widget == widget->parent->children_tail) {
	    widget->parent->children_head = NULL;
	    widget->parent->children_tail = NULL;
	}
	else if (widget == widget->parent->children_head) {
	    widget->parent->children_head = widget->next_sibling;
	}
	else {
	    while (brother->next_sibling != widget) {
		brother = brother->next_sibling;
	    }
	    if (widget == widget->parent->children_tail) {
		widget->parent->children_tail = brother;
		brother->next_sibling = NULL;
	    }
	    else {
		brother->next_sibling = widget->next_sibling;
	    }
	}
	    widget->wclass->releasefunc(widget);
	    free(widget);
    }
}

void			ei_frame_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 ei_relief_t*		relief,
							 char**			text,
							 ei_font_t*		text_font,
							 ei_color_t*		text_color,
							 ei_anchor_t*		text_anchor,
							 ei_surface_t*		img,
							 ei_rect_t**		img_rect,
							 ei_anchor_t*		img_anchor) {
// à changer après avoir écrit la bibliothèque geometrymanager: requested_size
    ei_class_frame_t *frame_obj = (ei_class_frame_t*)widget;

    // Gestion de la taille
    if (requested_size) {
        frame_obj->requested_size = *requested_size;
        widget->screen_location.size = *requested_size;
    }
    // Gestion de la couleur
    if (color) {
        frame_obj->color = *color;
    }
    // Gestion de la bordure
    if (border_width) {
        frame_obj->border_width = *border_width;
        /* Si le programmeur modifie la bordure et que le content_rect du widget excède
         * la nouvelle bordure en largeur ou hauteur, il faut le réduire. */
        if ((widget->content_rect->size.width) > (widget->screen_location.size.width-2*(*border_width))) {
            widget->content_rect->size.width = widget->screen_location.size.width - 2*(*border_width);
        }
        if ((widget->content_rect->size.height) > (widget->screen_location.size.height-2*(*border_width))) {
            widget->content_rect->size.height = widget->screen_location.size.height-2*(*border_width);
        }
    }

    // Gestion du relief de la bordure
    if (relief != ei_relief_none) {
        frame_obj->relief = *relief;
    }
    // Gestion de texte OU image
    if (!(text && img)) {
        // Gestion du texte
        if (text) {
            frame_obj->text = *text;
            //Gestion de la police du texte
            if (text_font) {
                frame_obj->text_font = text_font;
            }
            // Gestion de la couleur du texte
            if (text_color) {
                frame_obj->text_color = *text_color;
            }
            // Gestion de l'ancrage du texte
            if (text_anchor) {
                frame_obj->text_anchor = *text_anchor;
            }
            // Si la requested_size est à (0,0) et qu'il y a du texte, on met requested_size aux dimensions du texte
            if (frame_obj->requested_size.width == 0) {
                int text_width;
                int text_height;
                hw_text_compute_size(*text, frame_obj->text_font,&text_width,&text_height);
                frame_obj->requested_size.width = text_width;
                if (frame_obj->border_width) {
                    frame_obj->requested_size.width += frame_obj->border_width;
                }
            }
            if (frame_obj->requested_size.height == 0) {
                int text_width;
                int text_height;
                hw_text_compute_size(*text, frame_obj->text_font,&text_width,&text_height);
                frame_obj->requested_size.height = text_height;
                if (frame_obj->border_width) {
                    frame_obj->requested_size.height += frame_obj->border_width;
                }
            }
        }
        if (img) {
            frame_obj->image = *img;
            // Gestion de img_rect
            if (img_rect) {
                frame_obj->image_rect = *img_rect;
            }
            // Gestion de l'ancrage de l'image
            if (img_anchor) {
                frame_obj->image_anchor = *img_anchor;
            }
            // Si la requested_size est à (0,0) et qu'il y a une image, on met requested_size aux dimensions de l'image
            if (frame_obj->requested_size.width == 0) {
                frame_obj->requested_size.width = (*(frame_obj->image_rect)).size.width;
                if (frame_obj->border_width) {
                    frame_obj->requested_size.width += frame_obj->border_width;
                }
            }
            if (frame_obj->requested_size.height == 0) {
                frame_obj->requested_size.height = (*(frame_obj->image_rect)).size.height;
                if (frame_obj->border_width) {
                    frame_obj->requested_size.height += frame_obj->border_width;
                }
            }
        }
    }
}

void			ei_button_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 const ei_color_t*	color,
							 int*			border_width,
							 int*			corner_radius,
							 ei_relief_t*		relief,
							 char**			text,
							 ei_font_t*		text_font,
							 ei_color_t*		text_color,
							 ei_anchor_t*		text_anchor,
							 ei_surface_t*		img,
							 ei_rect_t**		img_rect,
							 ei_anchor_t*		img_anchor,
							 ei_callback_t*		callback,
							 void**			user_param) {

    ei_frame_configure(widget,requested_size,color,border_width,relief,text,text_font,text_color,text_anchor,img,img_rect,img_anchor);

    ei_class_button_t* button_obj = (ei_class_button_t*)widget;

    if (corner_radius) {
        button_obj->corner_radius = *corner_radius;
    }

    // Est-ce vraiment nécessaire de faire le malloc ici si c'est NULL ?
    // Faut essayer de le faire dans setdefaults plutôt.
    if (callback) {
        if (button_obj->callback == NULL) {
            button_obj->callback = malloc(sizeof(ei_callback_t));
        }
        button_obj->callback = *callback;

        if (user_param) {
            if (button_obj->user_param == NULL) {
                button_obj->user_param = (void**)malloc(sizeof(void*));
            }
            button_obj->user_param = user_param;
            //Normalement géré par le tag button
            //ei_bind(ei_ev_mouse_buttondown,widget,NULL,callback,user_param);
        }
        else {
            //Idem
            //ei_bind(ei_ev_mouse_buttondown,widget,NULL,callback,NULL);
        }
    }

}

void			ei_toplevel_configure		(ei_widget_t*		widget,
							 ei_size_t*		requested_size,
							 ei_color_t*		color,
							 int*			border_width,
							 char**			title,
							 ei_bool_t*		closable,
							 ei_axis_set_t*		resizable,
						 	 ei_size_t**		min_size) {
    ei_class_toplevel_t *toplevel_obj = (ei_class_toplevel_t*)widget;

    // Gestion de la taille
    if (requested_size) {
	toplevel_obj->requested_size = *requested_size;
	widget->screen_location.size = *requested_size;
}

// Gestion de la bordure
    if (border_width) {
        toplevel_obj->border_width = *border_width;
        /* Si le programmeur modifie la bordure et que le content_rect du widget excède
         * la nouvelle bordure en largeur ou hauteur, il faut le réduire. */
        if ((widget->content_rect->size.width) > (widget->screen_location.size.width-2*(*border_width))) {
            widget->content_rect->size.width = widget->screen_location.size.width - 2*(*border_width);
        }
        if ((widget->content_rect->size.height) > (widget->screen_location.size.height-2*(*border_width))) {
            widget->content_rect->size.height = widget->screen_location.size.height-2*(*border_width);
        }
    }
    if (title) {
	toplevel_obj->title = *title;
    }
    if (closable) {
	toplevel_obj->closable = *closable;
        // Pour changer le carré rouge de fermeture en bouton (non fonctionnel).
	/* ei_widget_t *close_button = ei_widget_create("button", widget); */
        /* ei_color_t	button_color	= {0xff, 0x20, 0x20, 0xff}; */
        /* ei_color_t      text_color      = {0x00, 0x00, 0x00, 0xff}; */
        /* ei_relief_t     relief          = ei_relief_raised; */
        /* int             button_border_width    = 2; */
	/* int             button_corner_radius = 2; */
        /* char*           button_title    = "X"; */

	/* ei_anchor_t	button_anchor	= ei_anc_northwest; */
        /* float           button_rel_x    = 0; */
        /* float           button_rel_y    = 0; */
        /* int		button_x	= 0; */
        /* int		button_y	= 0; */
        /* float           button_rel_size_x = 0; */
	/* int button_size = 10; */
	/* ei_callback_t callback = close_toplevel; */
	/* ei_widget_t *close_button = ei_widget_create("button", widget); */
	/* ei_button_configure(close_button, NULL, &button_color, &button_border_width, &button_corner_radius, &relief, &button_title, NULL, &text_color, NULL,  NULL, NULL, NULL, &callback, NULL); */
	/* ei_place(close_button, NULL, &button_x, &button_y, &button_size, &button_size, NULL, NULL, NULL, NULL); */
    }
    if (resizable != 0) {
	toplevel_obj->resizable = *resizable;
	if (min_size) {
	    toplevel_obj->min_size = *min_size;
	}
    }

}
