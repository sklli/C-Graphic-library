#include "ei_widget.h"
#include "ei_types.h"
#include "ei_new_types.h"
#include <stdio.h>
#include <string.h>

void	placer_runfunc	(struct ei_widget_t*	widget) {

    if (widget) {
    

        ei_placer_t* placer_widget = (ei_placer_t*)widget->geom_params;
        ei_widget_t *parent = widget->parent;
        int x;
        int y;
        int parent_height = widget->parent->content_rect->size.height;
        int parent_width = widget->parent->content_rect->size.width;


        // Recherche de la longueur et largeur affichées du widget
        if (placer_widget->rel_width == 0.0) {
            widget->screen_location.size.width = placer_widget->width;
        }
        else {
            widget->screen_location.size.width = (int)((placer_widget->rel_width) * parent_width + placer_widget->width);
        }
        if (widget->screen_location.size.width == 0) {

        }
        if (placer_widget->height == 0.0) {
            widget->screen_location.size.height = placer_widget->height;
        }
        else {
            widget->screen_location.size.height = (int)((placer_widget->rel_height) * parent_height + placer_widget->height);
        }
        if ( widget->screen_location.size.height == 0) {

        }
        int widget_height = widget->screen_location.size.height;
        int widget_width = widget->screen_location.size.width;


        // Calcul du x et y du top_left du widget selon l'ancre du placeur
        switch (placer_widget->anchor) {

        case ei_anc_none:
        case ei_anc_northwest:

            // Calcul de x
            // Prise en compte du débordement à gauche
            if ((int)((placer_widget->rel_x) * parent_width) < widget_width) {
                x = (int)((placer_widget->rel_x) * parent_width);
                if (placer_widget->x > 0) {
                    x += placer_widget->x;
                }
            }
            // Prise en compte du débordement à droite
            else if ((parent_width - (int)((placer_widget->rel_x) * parent_width) < widget_width)) {
                x = parent_width - widget_width;
                if (placer_widget->x < 0) {
                    x += placer_widget->x;
                }
            }
            else {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x) - widget_width/2;
                // Le terme -widget_width/2 est là pour bien placer le x
                // S'il n'y est pas, x prend la valeur du centre du widget plutôt que celle du top_left
            }

            // Calcul de y
            // Prise en compte du débordement à gauche
            if ((int)((placer_widget->rel_y) * parent_height) < widget_height) {
                y = (int)((placer_widget->rel_y) * parent_height);
                if (placer_widget->y > 0) {
                    y += placer_widget->y;
                }
            }
            // Prise en compte du débordement à droite
            else if ((parent_height - (int)((placer_widget->rel_y) * parent_height) < widget_height)) {
                y = parent_height - widget_height;
                if (placer_widget->y < 0) {
                    y += placer_widget->y;
                }
            }
            else {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y) - widget_height/2;
                // Le terme -widget_height/2 est là pour bien placer le y
                // S'il n'y est pas, y prend la valeur du centre du widget plutôt que celle du top_left
            }
            break;

        case ei_anc_west:

            if ((int)((placer_widget->rel_x) * parent_width) < widget_width) {
                x = (int)((placer_widget->rel_x) * parent_width);
                if (placer_widget->x > 0) {
                    x += placer_widget->x;
                }
            }
            else if ((parent_width - (int)((placer_widget->rel_x) * parent_width) < widget_width)) {
                x = parent_width - widget_width;
                if (placer_widget->x < 0) {
                    x += placer_widget->x;
                }
            }
            else {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x) - widget_width/2;
            }

            if ((int)((placer_widget->rel_y) * parent_height) < widget_height) {
                y = (int)((placer_widget->rel_y) * parent_height);
                if (placer_widget->y > 0) {
                    y += placer_widget->y - widget_height/2;
                }
            }
            else if ((parent_height - (int)((placer_widget->rel_y) * parent_height) < widget_height)) {
                y = parent_height - widget_height;
                if (placer_widget->y < 0) {
                    y += placer_widget->y - widget_height/2;
                }
            }
            else {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y) - widget_height/2;
            }
            break;

        case ei_anc_southwest:

            if ((int)((placer_widget->rel_x) * parent_width) < widget_width) {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x);
                if (placer_widget->x > 0) {
                    x += placer_widget->x;
                }
            }
            else if ((parent_width - (int)((placer_widget->rel_x) * parent_width) < widget_width)) {
                x = parent_width - widget_width;
                if (placer_widget->x < 0) {
                    x += placer_widget->x;
                }
            }
            else {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x) - widget_width/2;
            }

            if ((int)((placer_widget->rel_y) * parent_height) < widget_height) {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y);
                if (placer_widget->y > 0) {
                    y += placer_widget->y - widget_height;
                }
            }
            else if ((parent_height - (int)((placer_widget->rel_y) * parent_height) < widget_height)) {
                y = parent_height - widget_height;
                if (placer_widget->y < 0) {
                    y += placer_widget->y - widget_height;
                }
            }
            else {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y) - widget_height/2;
            }
            break;

        case ei_anc_south:

            if ((int)((placer_widget->rel_x) * parent_width) < widget_width) {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x);
                if (placer_widget->x > 0) {
                    x += placer_widget->x - widget_width/2;
                }
            }
            else if ((parent_width - (int)((placer_widget->rel_x) * parent_width) < widget_width)) {
                x = parent_width - widget_width;
                if (placer_widget->x < 0) {
                    x += placer_widget->x - widget_width/2;
                }
            }
            else {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x) - widget_width/2;
            }

            if ((int)((placer_widget->rel_y) * parent_height) < widget_height) {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y);
                if (placer_widget->y > 0) {
                    y += placer_widget->y - widget_height;
                } 
            }
            else if ((parent_height - (int)((placer_widget->rel_y) * parent_height) < widget_height)) {
                y = parent_height - widget_height;
                if (placer_widget->y < 0) {
                    y += placer_widget->y - widget_height;
                }
            }
            else {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y) - widget_height/2;
            }
            break;

    case ei_anc_southeast:

        if (placer_widget->rel_x == 0.0) {
            x = placer_widget->x - widget_width;
        }
        else {
            if ((int)((placer_widget->rel_x) * parent_width) < widget_width) {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x);
                if (placer_widget->x > 0) {
                    x += placer_widget->x - widget_width;
                }
            }
            else if ((parent_width - (int)((placer_widget->rel_x) * parent_width) < widget_width)) {
                x = parent_width - widget_width;
                if (placer_widget->x < 0) {
                    x += placer_widget->x - widget_width;
                }
            }
            else {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x) - widget_width/2;
            }
        }

        if (placer_widget->rel_y == 0.0) {
            y = placer_widget->y - widget_height;
        }
        else {
            if ((int)((placer_widget->rel_y) * parent_height) < widget_height) {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y);
                if (placer_widget->y > 0) {
                    y += placer_widget->y - widget_height;
                }
            }
            else if ((parent_height - (int)((placer_widget->rel_y) * parent_height) < widget_height)) {
                y = parent_height - widget_height;
                if (placer_widget->y < 0) {
                    y += placer_widget->y - widget_height;
                }
            }
            else {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y) - widget_height/2;
            }
            break;
        }

        case ei_anc_east:

            if ((int)((placer_widget->rel_x) * parent_width) < widget_width) {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x);
                if (placer_widget->x > 0) {
                    x += placer_widget->x - widget_width;
                }
            }
            else if ((parent_width - (int)((placer_widget->rel_x) * parent_width) < widget_width)) {
                x = parent_width - widget_width;
                if (placer_widget->x < 0) {
                    x += placer_widget->x - widget_width;
                }
            }
            else {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x) - widget_width/2;
            }


            if ((int)((placer_widget->rel_y) * parent_height) < widget_height) {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y);
                if (placer_widget->y > 0) {
                    y += placer_widget->y - widget_height/2;
                }
            }
            else if ((parent_height - (int)((placer_widget->rel_y) * parent_height) < widget_height)) {
                y = parent_height - widget_height;
                if (placer_widget->y < 0) {
                    y += placer_widget->y - widget_height/2;
                }
            }
            else {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y) - widget_height/2;
            }
            break;

        case ei_anc_northeast:

            if ((int)((placer_widget->rel_x) * parent_width) < widget_width) {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x);
                if (placer_widget->x > 0) {
                    x += placer_widget->x - widget_width;
                }
            }
            else if ((parent_width - (int)((placer_widget->rel_x) * parent_width) < widget_width)) {
                x = parent_width - widget_width;
                if (placer_widget->x < 0) {
                    x += placer_widget->x - widget_width;
                }
            }
            else {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x) - widget_width/2;
            }

            if ((int)((placer_widget->rel_y) * parent_height) < widget_height) {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y);
                if (placer_widget->y > 0) {
                    y += placer_widget->y;
                }
            }
            else if ((parent_height - (int)((placer_widget->rel_y) * parent_height) < widget_height)) {
                y = parent_height - widget_height;
                if (placer_widget->y < 0) {
                    y += placer_widget->y;
                }
            }
            else {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y) - widget_height/2;
            }
            break;

        case ei_anc_north:

            if ((int)((placer_widget->rel_x) * parent_width) < widget_width) {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x);
                if (placer_widget->x > 0) {
                    x += placer_widget->x - widget_width/2;
                }
            }
            else if ((parent_width - (int)((placer_widget->rel_x) * parent_width) < widget_width)) {
                x = parent_width - widget_width;
                if (placer_widget->x < 0) {
                    x += placer_widget->x - widget_width/2;
                }
            }
            else {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x) - widget_width/2;
            }

            if ((int)((placer_widget->rel_y) * parent_height) < widget_height) {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y);
                if (placer_widget->y > 0) {
                    y += placer_widget->y;
                }
            }
            else if ((parent_height - (int)((placer_widget->rel_y) * parent_height) < widget_height)) {
                y = parent_height - widget_height;
                if (placer_widget->y < 0) {
                    y += placer_widget->y;
                }
            }
            else {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y) - widget_height/2;
            }
            break;

        case ei_anc_center:

            if ((int)((placer_widget->rel_x) * parent_width) < widget_width) {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x);
                if (placer_widget->x > 0) {
                    x += placer_widget->x - widget_width/2;
                }
            }
            else if ((parent_width - (int)((placer_widget->rel_x) * parent_width) < widget_width)) {
                x = parent_width - widget_width;
                if (placer_widget->x < 0) {
                    x += placer_widget->x - widget_width/2;
                }
            }
            else {
                x = (int)((placer_widget->rel_x) * parent_width + placer_widget->x) - widget_width/2;
            }

            if ((int)((placer_widget->rel_y) * parent_height) < widget_height) {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y);
                if (placer_widget->y > 0) {
                    y += placer_widget->y - widget_height/2;
                }
            }
            else if ((parent_height - (int)((placer_widget->rel_y) * parent_height) < widget_height)) {
                y = parent_height - widget_height;
                if (placer_widget->y < 0) {
                    y += placer_widget->y - widget_height/2;
                }
            }
            else {
                y = (int)((placer_widget->rel_y) * parent_height + placer_widget->y) - widget_height/2;
            }
            break;

        default:
            break;
        }


        // Prise en compte de la bordure
        ei_class_frame_t* frame_parent = (ei_class_frame_t*)parent;
        if (frame_parent->border_width) {
            int border = frame_parent->border_width;
            if (x < border) {
                x = border;
            }
            else if (x > (parent_width - border - widget_width)) {
                x = parent_width - border - widget_width;
            }
            if (y < border) {
                y = border;
            }
            else if (y > (parent_height - border - widget_height)) {
                y = parent_height - border - widget_height;
            }
        }

        x += parent->content_rect->top_left.x;
        y += parent->content_rect->top_left.y;
        widget->screen_location.top_left.x = x;
        widget->screen_location.top_left.y = y;
        widget->content_rect->top_left.x = x;
        widget->content_rect->top_left.y = y;
    }
}

void	placer_releasefunc	(struct ei_widget_t*	widget) {
    NULL;
    /* if (widget) { */
    /*     ei_widget_t *sibling = widget->parent->children_head; */
    /*     ei_widget_t *parent = widget->parent; */
    /*     // On midifie la géométrie des frères */
    /*     while (sibling != NULL) { */
    /*         ei_placer_t* placer_sibling = (ei_placer_t*)sibling->geom_params; */
    /*         placer_sibling->widget = sibling; */
    /*         (*(placer_sibling->manager->runfunc))(sibling);// Appel à ei_place à un moment? */
    /*         sibling = sibling->next_sibling; */
    /*     } */
    /*     // On modifie la géométrie des parents */
    /*     while (parent != NULL) { */
    /*         ei_placer_t* placer_parent = (ei_placer_t*)parent->geom_params; */
    /*         placer_parent->widget = parent; */
    /*         (*(placer_parent->manager->runfunc))(parent); */
    /*         parent = parent->parent; */
    /*     } */
    /* } */
}

void	ei_geometrymanager_register	(ei_geometrymanager_t* geometrymanager) {
    if (registered_geo->manager != NULL) {
        struct geo_cell *new = malloc(sizeof(struct geo_cell));
        new->next = registered_geo;
        registered_geo = new;
    }
    registered_geo->manager = geometrymanager;
}

ei_geometrymanager_t*	ei_geometrymanager_from_name	(ei_geometrymanager_name_t name) {
    struct geo_cell *current = registered_geo;
    while (current != NULL) {
        if (strncmp(current->manager->name, name, strlen(name)) == 0) {
            return current->manager;
        }
        current = current->next;
    }
    return NULL;
}

void	ei_geometrymanager_unmap	(ei_widget_t*		widget) {
    if (widget) {
        if (widget->geom_params != NULL) {
            /* ei_placer_t* placer = (ei_placer_t*)widget->geom_params; */
            widget->screen_location.top_left.x = 0;
            widget->screen_location.top_left.y = 0;
            widget->screen_location.size.height = 0;
            widget->screen_location.size.width = 0;
            /* placer->manager->releasefunc(widget); */
            widget->geom_params = NULL;
        }
    }
}

void 	ei_register_placer_manager 	() {
    ei_geometrymanager_t *manager = (ei_geometrymanager_t*)malloc(sizeof(ei_geometrymanager_t));
    char* name = "placer";
    strncpy(manager->name,name, strlen(name)+1);
    manager->runfunc = &placer_runfunc;
    manager->releasefunc = &placer_releasefunc;
    manager->next = NULL;
    ei_geometrymanager_register(manager);
}


void			ei_place			(ei_widget_t*		widget,
							 ei_anchor_t*		anchor,
							 int*			x,
							 int*			y,
							 int*			width,
							 int*			height,
							 float*			rel_x,
							 float*			rel_y,
							 float*			rel_width,
							 float*			rel_height) {

    ei_geometrymanager_t* widget_manager = (ei_geometrymanager_t*)widget->geom_params;
    // Cas où geom_params est du type placeur
    if (strncmp(widget_manager->name,"placer",strlen("placer")) == 0) {
        ei_placer_t* placer = (ei_placer_t*)widget->geom_params;
        placer->widget = widget;
        placer->manager = ei_geometrymanager_from_name("placer");
        if (anchor) {
            placer->anchor = *anchor;
        }
        if (x) {
            placer->x = *x;
        }
        if (y) {
            placer->y = *y;
        }
        if (width) {
            placer->width = *width;
        }
        if (height) {
            placer->height = *height;
        }
        if (rel_x) {
            placer->rel_x = *rel_x;
        }
        if (rel_y) {
            placer->rel_y = *rel_y;
        }
        if (rel_width) {
            placer->rel_width = *rel_width;
        }
        if (rel_height) {
            placer->rel_height = *rel_height;
        }
        (*(placer->manager->runfunc))(widget);
    }
    // Cas où geom_params est nul ou est un manager de géométrie autre qu'un placeur
    else {
        if (widget->geom_params != NULL) {
            ei_geometrymanager_unmap(widget);
        }
        ei_placer_t* placer = malloc(sizeof(ei_placer_t));
        widget->geom_params = (ei_geometry_param_t*)placer;
        ei_placer_setdefaults(widget);
        placer->widget = widget;
        placer->manager = ei_geometrymanager_from_name("placer");
        if (anchor) {
            placer->anchor = *anchor;
        }
        else {
            ei_anchor_t default_anchor = ei_anc_northwest;
            placer->anchor = default_anchor;
        }
        if (x) {
            placer->x = *x;
        }
        else {
            placer->x = 0;
        }
        if (y) {
            placer->y = *y;
        }
        else {
            placer->y = 0;
        }
        if (width) {
            placer->width = *width;
        }
        else {
            int* widget_width = &widget->requested_size.width;
            placer->width = *widget_width;
        }
        if (height) {
            placer->height = *height;
        }
        else {
            int* widget_height = &widget->requested_size.height;
            placer->height = *widget_height;
        }
        if (rel_x) {
            placer->rel_x = *rel_x;
        }
        else {
            float zero = 0.0;
            placer->rel_x = zero;
        }
        if (rel_y) {
            placer->rel_y = *rel_y;
        }
        else {
            float zero = 0.0;
            placer->rel_y = zero;
        }
        if (rel_width) {
            placer->rel_width = *rel_width;
        }
        else {
            float zero = 0.0;
            placer->rel_width = zero;
        }
        if (rel_height) {
            placer->rel_height = *rel_height;
        }
        else {
            float zero = 0.0;
            placer->rel_height = zero;
        }
        (*(placer->manager->runfunc))(widget);
    }
}
