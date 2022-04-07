#include "stegano.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc,char** argv) 
{
	FILE *file_handle;
	FILE *message_handle;
	FILE *hidden_message_handle;

	if(argc!=5) {
		printf("Utilisation: %s [-e][-d] <source file> <destination file> <text file>\n-e : Ajoute le texte dans l'image\n-d : Recupere le texte de l'image\n",argv[0]);
		exit(1);
	}

	int mode;
	
	if(!strcmp(argv[1],"-e"))
		mode=1;
		
	else if(!strcmp(argv[1],"-d"))
		mode=0;
		
	else 
	{
		printf("Utilisation: %s <mode> <source file> <destination file> <text file>\nMode - e = crypter \n d= decrypter \n",argv[0]);
		exit(1);
	}

	/* Traitement de l'ouverture du fichier et des erreurs */
	file_handle=fopen(argv[2],"r");
	if (file_handle == NULL) 
	{
		fprintf(stderr, "Impossible d'ouvrir le fichier d'entrée %s\n",argv[2]);
		exit(1);
	}

	hidden_message_handle=fopen(argv[3],"w");
	if (hidden_message_handle== NULL) 
	{
		fprintf(stderr, "Impossible de créer le fichier de sortie %s\n",argv[3]);
		exit(1);
	}
	

	int c=0;

	/* Génère un fichier avec le même header. Copie les 120 premiers octets */
	char tmp_sig_cpy;
	int offset=get_image_data_offset(file_handle);

	rewind(file_handle);

	for(int i=0;i<offset;i++) 
	{
		tmp_sig_cpy=fgetc(file_handle);
		fputc(tmp_sig_cpy,hidden_message_handle);
		c++;
	}
	/* Met le fichier en .bmp */

	char file_buffer; 			// Variable tampon pour un octet du fichier
	char message_buffer;		// Buffer tampon pour un octet du message

	if(mode) 
	{
		message_handle=fopen(argv[4],"r");
		if (message_handle== NULL) 
		{
			fprintf(stderr, "Impossible d'ouvrir le fichier d'entrée de texte %s\n",argv[4]);
			exit(1);
		}
		int hidden_message_length=get_message_length(message_handle);


	/* 
	Apres que offset a été lu et que le fichier header a été écrit sur l'image vierge,
	la longueur du message caché est écrite sur le premier octet. 
	Cette longueur est ensuite utilisé pendant le decryptage du texte depuis l'image. 
	*/
		fputc(hidden_message_length,hidden_message_handle);
		c++;
		do {
			int bit_of_message;
			if(!feof(message_handle)) {		
				message_buffer=fgetc(message_handle);
				for(int i=1;i<=8;i++) {  //Effectue cela pour chaque bit dans chaque octet de l'image vierge

					file_buffer=fgetc(file_handle);
					c++;
					int file_byte_lsb = file_buffer & 1; // AND avec 1 pour avoir la valeur du LSB. AND renvoi 0 si LSB est 0 ou 1 si c'est 1

					bit_of_message=get_bit(message_buffer,i);

					if(file_byte_lsb==bit_of_message) {
						fputc(file_buffer,hidden_message_handle);
					}
					else {
						if(file_byte_lsb==0)
							file_buffer = (file_buffer | 1);
						else
							file_buffer = (file_buffer & ~1);
						//  Changement logique du LSB du file_buffer et le met dans un fichier avec putc()
						fputc(file_buffer,hidden_message_handle);
					}
				}
			}
			else {
				tmp_sig_cpy=fgetc(file_handle);
				fputc(tmp_sig_cpy,hidden_message_handle);
				c++;
			}
		} while(!feof(file_handle));	
		fclose(message_handle);	
	}
	else {
		message_handle=fopen(argv[4],"w");
		if (message_handle== NULL) {
			fprintf(stderr, "Impossible d'ouvrir le fichier %s\n",argv[4]);
			exit(1);
		}
	
		/* Saisie les LSB de tous les octets pour une longueur spécifié à fgetc */
		int message_length=fgetc(file_handle);
		for(int i=0;i<message_length;i++) {
			char temp_ch='\0';
			for( int j=0;j<8;j++) {
				temp_ch=temp_ch<<1;
				file_buffer=fgetc(file_handle);
				int file_byte_lsb = file_buffer & 1; 
				temp_ch|=file_byte_lsb;
			}
			fputc(temp_ch,message_handle);
		}
		fclose(message_handle);	
	}

	/* Nettoie avant de quitter */
	fclose(file_handle);
	fclose(hidden_message_handle);
}
