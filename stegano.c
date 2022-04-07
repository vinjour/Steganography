#include "stegano.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_image_data_offset(FILE* bmp_offset) //prend en argument un pointeur bmp_offset qui représente le flux de base de donnée
{
	fseek(bmp_offset,10,0); //définit la position de fichier du flux sur l'offset donné
	int offset;
	offset=(int)fgetc(bmp_offset); //permet de lire un caractère à la position du flux spécifié (bmp_offset dans notre cas et en fais un int).
	return offset; //renvoie la valeur pour commencer à lire le flux post offset
}

int get_message_length(FILE *fp) 
{
	fseek(fp, 0L, SEEK_END); //permet de se positionner dans le fichier image à la position de l'offset
	int size = ftell(fp); //on récupère la valeur à la position courante (taille du texte)
	fseek(fp, 0L, SEEK_SET);
	return(size);
}

int get_bit(char the_byte,int which_bit) 
{
	return((the_byte>>8-which_bit)&1);
}
