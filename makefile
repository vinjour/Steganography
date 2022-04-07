CC=gcc
FLAGS=-Wall  -g

all	:	prog e d

prog	:	stegano.o main_stegano.o
			$(CC) stegano.o main_stegano.o -o prog

stegano.o	:	stegano.c stegano.h
				$(CC) -c stegano.c $(FLAGS) -o stegano.o

main_stegano.o	:	main_stegano.c stegano.h
						$(CC) -c main_stegano.c $(FLAGS) -o main_stegano.o

clean	:	
			rm -f *.o
			rm -f prog

e : addText

addText : 	prog imageEnsea.bmp imageEnsea.bmp message.txt
			./prog -e imageEnsea.bmp imageEnsea2.bmp message.txt

d : getText

getText :	prog imageEnsea2.bmp imageEnsea3.bmp message_recu.txt
			./prog -d imageEnsea2.bmp imageEnsea3.bmp message_recu.txt