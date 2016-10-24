#include "ei_event.h"
#include "ei_widget.h"
#include "ei_new_types.h"
#include <stdio.h>
#include <string.h>
#include "ei_class_button.h"

void		ei_bind			(ei_eventtype_t		eventtype,
					 ei_widget_t*		widget,
					 ei_tag_t		tag,
					 ei_callback_t		callback,
					 void*			user_param){
    if ((widget) && (tag)) {
        fprintf(stderr,"Binding Error : A callback can't be linked to both a widget and a tag");
        return;
    }

    struct event_type_list* current_event = registered_event_type[0];
    int i = 1;

    /* On parcourt la liste des event_type jusqu'à trouver celui dont il est question */
    while ((current_event->event != eventtype) && (i < 8)) {
        current_event = registered_event_type[i];
        i++;
    }

    if (i == 8) {
        fprintf(stderr,"This event type does not exist.");
        return;
    }

    /* A présent, current_event est placé sur la bonne cellule.
     * Il faut maintenant compléter les champs adéquats selon ce
     * qui a été passé en paramètre dans la fonction ei_bind */
    
    if (widget) {
        /* Liaison widget / callback / event */

        /* Ajout du widget dans la liste des widgets associés à l'event */
        if (current_event->widget_and_callback != NULL) {
            struct widget_cell *new_widget = malloc(sizeof(struct widget_cell));
            new_widget->next = current_event->widget_and_callback;
            current_event->widget_and_callback = new_widget;
        }
        else {
            current_event->widget_and_callback = malloc(sizeof(struct widget_cell));
            current_event->widget_and_callback->next = NULL;
        }
        current_event->widget_and_callback->widget = widget;
        current_event->widget_and_callback->callback = callback;
        current_event->widget_and_callback->user_param = user_param;
    }
    else {
        /* Liaison tag / callback / event */

        /* Ajout du tag dans la liste des tags associés à l'event */
        if (current_event->tag_and_callback != NULL) {
            struct tag_cell *new_tag = malloc(sizeof(struct tag_cell));
            new_tag->next = current_event->tag_and_callback;
            current_event->tag_and_callback = new_tag;
        }
        else {
            // Le malloc change des valeurs qui n'ont rien à voir
            current_event->tag_and_callback = malloc(sizeof(struct tag_cell));
            current_event->tag_and_callback->next = NULL;
        }
        current_event->tag_and_callback->tag = tag;
        current_event->tag_and_callback->callback = callback;
        current_event->tag_and_callback->user_param = user_param;
    }
}

void		ei_unbind		(ei_eventtype_t		eventtype,
                                         ei_widget_t*		widget,
                                         ei_tag_t		tag,
                                         ei_callback_t		callback,
                                         void*			user_param){

    if ((widget) && (tag)) {
        fprintf(stderr,"Unbinding Error : A callback can't be detached from both a widget and a tag");
        return;
    }

    struct event_type_list* current_event = registered_event_type[0];
    int i = 1;

    /* On parcourt la liste des event_type jusqu'à trouver celui dont il est question */
    while (current_event->event != eventtype) {
        current_event = registered_event_type[i];
        i++;
    }

    if (i == 8) {
        fprintf(stderr,"This event type does not exist.");
        return;
    }

    if (widget) {

        struct widget_cell* previous_widget;
        struct widget_cell* current_widget = current_event->widget_and_callback;

        /* On parcourt la liste des widgets associés à current_event, et on
         * supprime le bon. */
        while ((current_widget->widget != widget) && (current_widget)) {
            previous_widget = current_widget;
            current_widget = current_widget->next;
        }

        /* Si on s'arrête après le dernier widget... */
        /* Ou si le callback du widget n'est pas celui mentionné... */
        /* Ou si le user_param du widget n'est pas celui mentionné... */
        if ((current_widget == NULL) || (current_widget->callback != callback) || (current_widget->user_param != user_param)) {
            fprintf(stderr, "Unbinding Error : This association event/widget/callback does not exist");
            return;
        }

        /* Sinon, on supprime l'association event/widget/callback */
        previous_widget->next = current_widget->next;
        free(current_widget); //Ca détruit le widget ?
        current_widget = NULL;

    }
    else {

        struct tag_cell* previous_tag;
        struct tag_cell* current_tag = current_event->tag_and_callback;

        /* On parcourt la liste des tags associés à current_event, et on
         * supprime le bon. */
        while ((current_tag->tag != tag) && (current_tag)) {
            previous_tag = current_tag;
            current_tag = current_tag->next;
        }

        /* Si on s'arrête après le dernier tag... */
        /* Ou si le callback associé n'est pas celui mentionné... */
        /* Ou si le user_param du tag n'est pas celui mentionné... */
        if ((current_tag == NULL) || (current_tag->callback != callback) || (current_tag->user_param != user_param)) {
            fprintf(stderr, "Unbinding Error : This association event/tag/callback does not exist");
            return;
        }

        /* Sinon, on supprime l'association event/tag/callback */
        previous_tag->next = current_tag->next;
        free(current_tag); //Ca détruit le tag ?
        current_tag = NULL;
    }
}
