#http://makefiletutorial.com/
#File
objects = P1G145_877546_877485_vm.o P1G145_877546_877485_lettura_file.o P1G145_877546_877485_controllo_programma.o P1G145_877546_877485_stampa.o P1G145_877546_877485_istruction_set_stampa.o P1G145_877546_877485_esegui.o P1G145_877546_877485_istruction_set.o
#Nome file eseguibile
eseguibile = vm
#Specifiche compilazione
cflags = -g3 -fsanitize=address -fsanitize=undefined -std=gnu89 -pedantic-errors -Wall -Wextra
libs = -lm

#Compilazione
$(eseguibile) : $(objects)
	gcc $(cflags) -o $(eseguibile) $(objects) $(libs)

#Rimozione file generati
clean:
	rm $(eseguibile)  $(objects)

#regole implicite
.c.o :
	gcc -c $(cflags) $< 
.h.c :
	touch $<
