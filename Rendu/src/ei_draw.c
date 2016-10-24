#include <stdio.h>
#include <assert.h>
#include "ei_types.h"
#include "hw_interface.h"

/* Fonction triant les indices (int) des couleurs dans l'ordre croissant */

/* void		IndexSort		(int Indexes[4][2]) { */
/*   int i, j; */
/*   int index_stock; */
/*   unsigned char ColorStock; */

/*   for(i=0; i<4; i++) { */
/*     for(j=i+1; j<4; j++) { */
/*       if(Indexes[i][0]>Indexes[j][0]) { */
/*         //Echange des indices. */
/*         index_stock = Indexes[i][0]; */
/*         Indexes[i][0] = Indexes[j][0]; */
/*         Indexes[j][0] = index_stock; */
/*         //Echange des couleurs. */
/*         ColorStock = Indexes[i][1]; */
/*         Indexes[i][1] = Indexes[j][1]; */
/*         Indexes[j][1] = ColorStock; */
/*       } */
/*     } */
/*   } */
/* } */
/* ------------------------------------------------------------------ */
/* uint32_t		ei_map_rgba		(ei_surface_t surface, const ei_color_t* color) { */

/*   /\* Un petit doute subsiste : */
/*   Faut-il utiliser hw_surface_has_alpha(surface) ? *\/ */

/*   /\* On récupère tout d'abord les indices de position de R, G, B, et A *\/ */
/*   int index_red, index_green, index_blue, index_alpha; */
/*   hw_surface_get_channel_indices(surface, &index_red, &index_green, &index_blue, &index_alpha); */

/*   /\* On trie les indices dans l'ordre croissant *\/ */
/*   int Indexes[4][2] = { {index_red, color->red}, */
/*   {index_green, color->green}, */
/*   {index_blue, color->blue}, */
/*   {index_alpha, color->alpha} }; */

/*   IndexSort(Indexes); */
/*   /\* Les indices sont à présents triés dans l'ordre croissant dans */
/*   notre tableau, avec leur couleur respective juste à côté d'eux. *\/ */

/*   /\* A présent, on code la couleur sur 32 bits, en respectant l'ordre : */
/*   Le premier élément du tableau occupe les 8 premiers bits, etc *\/ */
/*   return  (uint32_t)Indexes[3][1] | */
/*   ((uint32_t)Indexes[2][1] << 8) | */
/*   ((uint32_t)Indexes[1][1] << 16) | */
/*   ((uint32_t)Indexes[0][1] << 24); */
/* } */

/* ------------------------------------------------------------------ */

/* void ei_draw_text (ei_surface_t		surface, const ei_point_t*	where, const char*		text, const ei_font_t	font, const ei_color_t*	color, const ei_rect_t*	clipper) { */

/*     /\* On considère que la surface est bloquée. *\/ */

/*     /\* Création de la surface contenant le texte à écrire. *\/ */
/*     const ei_surface_t text_surface = hw_text_create_surface(text, font, color); */

/*     const ei_rect_t* text_rect_src = &(hw_surface_get_rect(text_surface)); */
/*     const ei_rect_t* text_rect_dst = { *where, text_rect_src->size }; */

/*     /\* Il faut copier le texte sur la surface, à partir du moment */
/*      * où on ne sort pas du clipper. Donc la zone (rectangle) de */
/*      * dessin est l'intersection entre text_rect et clipper. *\/ */

/*     hw_surface_lock(text_surface); */
/*     ei_copy_surface(surface,text_rect_dst,text_surface,text_rect_src,NULL); */

/*     /\* La surface ne contenant que le texte n'est à présent */
/*      * plus nécessaire, puisque ce dernier a été copié. *\/ */
/*     hw_surface_unlock(text_surface); */
/*     hw_surface_update_rects(surface,NULL); */
/*     /\* La surface ne contenant que le texte n'est à présent */
/*      * plus nécessaire, puisque ce dernier a été copié. *\/ */
/*     hw_surface_free(text_surface); */
/* } */

/* ------------------------------------------------------------------ */

/* void ei_fill (ei_surface_t surface, const ei_color_t*	color, const ei_rect_t*	clipper) { */
/*     /\* L'objectif ici est de dessiner un rectangle rempli sur la */
/*        surface à dessiner : */

/*        - On a d'abord besoin d'un ei_linked_point_t chaînant les 4 */
/*        points suivants du rectangle. *\/ */
/*     ei_point_t* top_left; */
/*     ei_point_t* top_right; */
/*     ei_point_t* low_right; */
/*     ei_point_t* low_left; */

/*     /\* Chaînage des points *\/ */
/*     ei_linked_point_t* filling_color = (ei_linked_point*)malloc(sizeof(ei_linked_point_t)); */
/*     filling_color->point = *top_left; */
/*     filling_color->next = (ei_linked_point*)malloc(sizeof(ei_linked_point_t)); */
/*     filling_color->next->point = *top_right; */
/*     filling_color->next->next = (ei_linked_point*)malloc(sizeof(ei_linked_point_t)); */
/*     filling_color->next->next->point = *low_right; */
/*     filling_color->next->next->next = (ei_linked_point*)malloc(sizeof(ei_linked_point_t)); */
/*     filling_color->next->next->next->point = *low_left; */
/*     filling_color->next->next->next->next = filling_color; */

/*     /\* Plaçons les points aux bonnes coordonnées : *\/ */
/*     /\* hw_surface_get_buffer retourne un pointeur vers l'adresse */
/*        du pixel en coordonnées (0,0), donc le contenu de ce pointeur */
/*        est lui-même un pointeur vers le point en (0,0)*\/ */
/*     top_left = *(hw_surface_get_buffer(surface)); */
/*     top_right->x = (hw_surface_get_size(surface)).width; */
/*     top_right->y = top_left->y; */
/*     low_right->x = top_right->x; */
/*     low_right->y = (hw_surface_get_size(surface)).height; */
/*     low_left->x = top_left->x; */
/*     low_left->y = low_right->y; */

/*     /\* Nous pouvons désormais tracer le rectangle. *\/ */
/*     if (color) { */
/*         ei_draw_polygon(surface,filling_color,*color,clipper); */
/*     } */
/*     /\* Si la couleur n'est pas précisée, on remplit en noir. *\/ */
/*     else { */
/*         ei_color_t black = {0x00, 0x00, 0x00, 0xff}; */
/*         ei_draw_polygon(surface,filling_color,black,clipper); */
/*     } */
/* } */


/* ------------------------------------------------------------------ */

/* Fonction retournant la "valeur" (c'est-à-dire le codage couleur) du pixel en position (i,j) */

/* uint32_t pixel_i_j(ei_surface_t surface, int i, int j) { */
/*     uint32_t* cursor = (uint32_t*)hw_surface_get_buffer(surface); */
/*     /\* Si A est l'adresse du pixel (0,0), alors l'adresse du pixel (i,j) est A + (i*width) + j. *\/ */
/*     return *(cursor + (i*hw_surface_get_size(surface).width) + j); */
/* } */

/* ------------------------------------------------------------------ */

/* int	ei_copy_surface (ei_surface_t	destination, const ei_rect_t*	dst_rect, const ei_surface_t	source, const ei_rect_t*	src_rect, const ei_bool_t	alpha) { */
/*     // s'il y a un canal alpha sur la source, il faut recalculer les couleurs pour destination. */
/*     if (dst_rect->size != src_rect->size) { */
/*         return 1; */
/*     } */

/*     /\* Si la surface source a un canal alpha, il faut recalculer les couleurs des pixels de la surface destination *\/ */
/*     if (hw_surface_has_alpha(source)) { */
/*         int index_red, index_green, index_blue, index_alpha; */
/*         hw_surface_get_channel_indices(destination,&index_red,&index_green,&index_blue,&index_alpha); */
/*         /\* ZUUUUUUUUUUUUUUUUUUUUUUUUUT *\/ */
/*         NULL; */
/*     } */
/*     /\* Sinon, les pixels de la surface destination sont une exacte copie de ceux de la surface source *\/ */
/*     else { */
/*         for (int i = 0 ; i < (src_rect->size.width) ; i++) { */
/*             for (int j = 0 ; j < (src_rect->size.height) ; j++) { */
/*                 pixel_i_j(destination, dst_rect->topleft.x + i, dst_rect->topleft.y + j) = pixel_i_j(source, src_rect->topleft.x + i, src_rect->topleft.y + j); */
/*             } */
/*         } */
/*     } */

/*     return 0; */
/* } */
