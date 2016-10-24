#include "ei_callback.h"
#include "ei_class_toplevel.h"
#include "ei_new_types.h"

/* --------------------------------------------------------------------- */
/* 3 petites fonctions intermédiaires utiles qui serviront pour la suite */
/* --------------------------------------------------------------------- */
int min(int x, int y) {
    if (x <= y) {
        return x;
    }
    else {
        return y;
    }
}

int max(int x, int y) {
    if (x <= y) {
        return y;
    }
    else {
        return x;
    }
}

int abs(int x) {
    if (x <= 0) {
        return -x;
    }
    else {
        return x;
    }
}

/* --------------------------------------------------------------------- */

/* Callback rattaché au tag "button", permettant l'animation du bouton enfoncé, puis l'appel
 * au Callback propre du bouton lorsque l'on clique dessus. */
ei_bool_t button_mouse_button_down(ei_widget_t* widget, ei_event_t* event, void* user_param) {

    /* Si on utilise le clic droit ou la molette -> return EI_FALSE; */
    if (event->param.mouse.button_number != 1) {
        return EI_FALSE;
    }

    /* Le widget manipulé ici est celui retourné par ei_widget_pick. */

    /* Mise du bouton sous forme enfoncée */
    hw_surface_lock(ei_app_root_surface());
    draw_frame_or_button_relief(widget,ei_app_root_surface(),NULL,EI_TRUE);
    redraw_text_or_image(widget,ei_app_root_surface(),NULL,EI_TRUE);
    hw_surface_unlock(ei_app_root_surface());
    ei_app_invalidate_rect(&(widget->screen_location));

    ei_event_t* event_button = malloc(sizeof(ei_event_t));
    ei_point_t pixel_picked;

    /* La suite de l'événement survient uniquement si on relâche le bouton de la souris */
    while (EI_TRUE) {
        hw_event_wait_next(event_button);
        pixel_picked = event_button->param.mouse.where;
        switch (event_button->type) {

        case 5:
            /* Mise du bouton sous forme relief */
            hw_surface_lock(ei_app_root_surface());
            draw_frame_or_button_relief(widget,ei_app_root_surface(),NULL,EI_FALSE);
            redraw_text_or_image(widget,ei_app_root_surface(),NULL,EI_FALSE);
            hw_surface_unlock(ei_app_root_surface());
            ei_app_invalidate_rect(&(widget->screen_location));

            /* Appel du Callback du button si on relâche le bouton sur le widget */
            if (widget == ei_widget_pick(&event_button->param.mouse.where)) {
                ei_class_button_t* button = (ei_class_button_t*)widget;
		if (button->callback){
		    return (*(button->callback))(widget,event,user_param);
		}
            }

            /* Si on relâche le clic en dehors du widget, l'événement associé à ce
             * dernier ne se produit pas. */
            return EI_FALSE;

        case 6:
            /* Si le curseur quitte les frontières du bouton, il revient sous forme relief */
            if ((pixel_picked.x < widget->screen_location.top_left.x) || (pixel_picked.x > widget->screen_location.top_left.x + widget->screen_location.size.width) || (pixel_picked.y < widget->screen_location.top_left.y) || (pixel_picked.y > widget->screen_location.top_left.y + widget->screen_location.size.height)) {
                /* Mise du bouton sous forme relief */
                hw_surface_lock(ei_app_root_surface());
                draw_frame_or_button_relief(widget,ei_app_root_surface(),NULL,EI_FALSE);
                redraw_text_or_image(widget,ei_app_root_surface(),NULL,EI_FALSE);
                hw_surface_unlock(ei_app_root_surface());
                ei_app_invalidate_rect(&(widget->screen_location));

            }
            break;

        default:
            break;
        }
    }
}
/* --------------------------------------------------------------------- */

/* Fonction qui redessine un widget et ses enfants, qui s'apparente à la fonction recursive_draw
 * de ei_application.c, à ceci près qu'elle a un paramètre en plus, le clipper, permettant
 * d'optimiser le déplacement ou le redimensionnement d'un toplevel en ne redessinant que ce
 * qui est nécessaire. */

void recursive_draw_optimal(ei_widget_t *widget, const ei_surface_t surface, const ei_surface_t offscreen, ei_rect_t* clipper) {
    // On redessine le widget
    (*(widget->wclass->drawfunc))(widget, surface, offscreen, clipper);

    // Et on fait de même pour ses enfants
    if (widget->children_head) {
        ei_widget_t *current = widget->children_head;
        recursive_draw_optimal(current, surface, offscreen, clipper);
        while (current->next_sibling){
            current = current->next_sibling;
            recursive_draw_optimal(current, surface, offscreen, clipper);
        }
    }
}
/* --------------------------------------------------------------------- */

/* Fonction permettant de faire subir aux enfants d'un toplevel le même mouvement
 * que ce dernier, lors du déplacement */
void move_children(ei_widget_t *widget, int delta_abs, int delta_ord) {

    ei_widget_t *current = widget->children_head;

    while (current) {
        current->screen_location.top_left.x += delta_abs;
        current->screen_location.top_left.y += delta_ord;
        move_children(current, delta_abs, delta_ord);
        current = current->next_sibling;
    }
}
/* --------------------------------------------------------------------- */

/* Fonction permettant de replacer les enfants d'un toplevel lorsque l'on
 * redimensionne ce dernier. */
void positioning_children(ei_widget_t* widget) {

    ei_widget_t* current = widget->children_head;
    
    while (current) {
        (*(current->geom_params->manager->runfunc))(current);
        positioning_children(current);
        current = current->next_sibling;
    }
}
/* --------------------------------------------------------------------- */

/* Fonction calculant le rectangle de clipper idéal à redessiner lors du déplacement d'un toplevel */
ei_rect_t ideal_shifting_clipper(ei_rect_t current_position, ei_point_t previous_position, ei_rect_t parent_position, int delta_abs, int delta_ord) {

    ei_rect_t clipper;
    /* Construction du rectangle de clipping, pour ne redessiner que ce qui est
     * nécessaire :

     * - L'origine est toujours le point (x,y) tel que x et y sont les coordonnées
     * les plus petites entre celles du rectangle de départ et d'arrivée.
     * - La largeur est celle du widget + le déplacement élémentaire en y effectué (en valeur absolue)
     * - La hauteur est celle du widget + le déplacement élémentaire en x effecuté (en valeur absolue) */

    clipper.top_left.x = min(previous_position.x, current_position.top_left.x);
    clipper.top_left.y = min(previous_position.y, current_position.top_left.y);
    clipper.size.width = current_position.size.width + abs(delta_abs)+1;
    clipper.size.height = current_position.size.height + abs(delta_ord)+1;

    /* Si le widget dépasse les limites du widget parent ou du root, il faut rogner le clipper. */
    if (current_position.top_left.x + current_position.size.width > parent_position.top_left.x + parent_position.size.width) {
        clipper.size.width = clipper.size.width - ((current_position.top_left.x + current_position.size.width) - (parent_position.top_left.x + parent_position.size.width));
    }

    if (current_position.top_left.y + current_position.size.height > parent_position.top_left.y + parent_position.size.height) {
        clipper.size.height = clipper.size.height - ((current_position.top_left.y + current_position.size.height) - (parent_position.top_left.y + parent_position.size.height)) - 1;
    }

    if (current_position.top_left.x < parent_position.top_left.x) {
        clipper.top_left.x = parent_position.top_left.x;
        clipper.size.width = clipper.size.width - (parent_position.top_left.x - current_position.top_left.x);
    }

    /* --------------- */

    return clipper;
}

/* --------------------------------------------------------------------- */

/* Fonction permettant de redimensionner un toplevel, appelée lorsque l'on
 * clique sur le petit carré de redimensionnement en bas à droite; */
ei_bool_t toplevel_resizing(ei_widget_t* widget, ei_event_t* event) {
    
    ei_point_t pixel_picked = event->param.mouse.where;
    ei_size_t save_size;
    ei_rect_t clipper;
    ei_event_t* event_toplevel = malloc(sizeof(ei_event_t));


    while (EI_TRUE) {

        hw_event_wait_next(event_toplevel);
        save_size = widget->screen_location.size;

        switch (event_toplevel->type) {

        case 5:
            /* On arrête le redimensionnement */
            return EI_TRUE;
        case 6:
            /* Tant que le curseur ne dépasse pas les limites du widget parent,
             * et que nous n'atteignons pas la min size, on redimensionne */

            if ((pixel_picked.x < widget->parent->screen_location.top_left.x + widget->parent->screen_location.size.width)) {
                widget->screen_location.size.width += (event_toplevel->param.mouse.where.x - pixel_picked.x);
                positioning_children(widget);
            }


            if ((pixel_picked.y < widget->parent->screen_location.top_left.y + widget->parent->screen_location.size.height)) {
                widget->screen_location.size.height += (event_toplevel->param.mouse.where.y - pixel_picked.y);
                positioning_children(widget);
            }
 
            pixel_picked = event_toplevel->param.mouse.where;
            clipper.top_left = widget->screen_location.top_left;
            clipper.size.width = max(save_size.width, widget->screen_location.size.width)+1;
            clipper.size.height = max(save_size.height, widget->screen_location.size.height)+1;
            recursive_draw_optimal(widget->parent, ei_app_root_surface(), ei_app_root_offscreen(), &clipper);
            break;

        default:
            break;
        }
    }
}
/* --------------------------------------------------------------------- */

/* Fonction permettant de déplacer un toplevel, appelée lorsque l'on clique
 * sur la zone du titre. */
ei_bool_t toplevel_shifting(ei_widget_t* widget, ei_event_t* event) {

    ei_point_t save_position = widget->screen_location.top_left;
    ei_point_t pixel_picked = event->param.mouse.where;
    int delta_x = widget->screen_location.top_left.x - (event->param.mouse.where.x);
    int delta_y = widget->screen_location.top_left.y - (event->param.mouse.where.y);
    int delta_abs;
    int delta_ord;
    ei_rect_t clipper;
    ei_event_t* event_toplevel = malloc(sizeof(ei_event_t));
    ei_rect_t parent_position = widget->parent->screen_location;

    while (EI_TRUE) {
        hw_event_wait_next(event_toplevel);
        pixel_picked = event_toplevel->param.mouse.where;
        switch (event_toplevel->type) {
        case 5:
            /* On arrête le déplacement */
            return EI_TRUE;
        case 6:

            save_position = widget->screen_location.top_left;

            if ((delta_x + (event_toplevel->param.mouse.where.x) > (parent_position.top_left.x) - (widget->screen_location.size.width)/1.5) && (delta_x + (event_toplevel->param.mouse.where.x) + widget->screen_location.size.width < (parent_position.top_left.x + parent_position.size.width) + (widget->screen_location.size.width)/1.5)) {
                widget->screen_location.top_left.x = delta_x + (event_toplevel->param.mouse.where.x);
            }

            if ((delta_y + (event_toplevel->param.mouse.where.y) > parent_position.top_left.y) && (delta_y + (event_toplevel->param.mouse.where.y) + widget->screen_location.size.height < parent_position.top_left.y + parent_position.size.height + (widget->screen_location.size.height)/1.5)) {

                widget->screen_location.top_left.y = delta_y + (event_toplevel->param.mouse.where.y);
            }

            delta_x = widget->screen_location.top_left.x - event_toplevel->param.mouse.where.x;
            delta_y = widget->screen_location.top_left.y - event_toplevel->param.mouse.where.y;

            delta_abs = widget->screen_location.top_left.x - save_position.x;
            delta_ord = widget->screen_location.top_left.y - save_position.y;
            move_children(widget, delta_abs, delta_ord);

            clipper = ideal_shifting_clipper(widget->screen_location, save_position, parent_position, delta_abs, delta_ord);

            hw_surface_lock(ei_app_root_surface());
            recursive_draw_optimal(widget->parent, ei_app_root_surface(), ei_app_root_offscreen(), &clipper);
            hw_surface_unlock(ei_app_root_surface());
            ei_app_invalidate_rect(&clipper);
            break;

        default:
            break;
        }
    }
}

/* --------------------------------------------------------------------- */

/* Fonction permettant de fermer le toplevel. */
ei_bool_t toplevel_closing(ei_widget_t* widget) {
    ei_widget_destroy(widget);
    return EI_TRUE;
}

/* --------------------------------------------------------------------- */

/* Fonction Callback rattachée au tag "toplevel", qui mène aux deux fonctions ci-dessus,
 * permettant le redimensionnement ou le déplacement d'un toplevel. */
ei_bool_t toplevel_processing(ei_widget_t* widget, ei_event_t* event, void* user_param) {

    /* Si on utilise le clic droit ou la molette -> return EI_FALSE; */
    if (event->param.mouse.button_number != 1) {
        return EI_FALSE;
    }

    ei_point_t save_position = widget->screen_location.top_left;
    ei_point_t pixel_picked = event->param.mouse.where;

    /* Si on clique sur la zone de resize, on redirige vers la fonction appropriée */
    if ((pixel_picked.x > save_position.x + widget->screen_location.size.width - 10) && (pixel_picked.x < save_position.x + widget->screen_location.size.width) && (pixel_picked.y > save_position.y + widget->screen_location.size.height - 10) && (pixel_picked.y < save_position.y + widget->screen_location.size.height)) {
        return toplevel_resizing(widget,event);
    }

    /* Si on clique sur la zone de fermeture du toplevel */
    if ((pixel_picked.x > save_position.x + 10 ) && (pixel_picked.x < save_position.x + 20) && (pixel_picked.y > save_position.y + 10 ) && (pixel_picked.y < save_position.y + 20)) {
        return toplevel_closing(widget);
    }

    /* Si le curseur est sur la bande du haut, il s'agit d'un déplacement */ 
    if ((pixel_picked.x > widget->screen_location.top_left.x) && (pixel_picked.x < widget->screen_location.top_left.x + widget->screen_location.size.width) && (pixel_picked.y > widget->screen_location.top_left.y) && (pixel_picked.y < widget->screen_location.top_left.y + ei_font_default_size + 5)) {
        return toplevel_shifting(widget,event);
    }

    return EI_FALSE;
}
/* --------------------------------------------------------------------- */
