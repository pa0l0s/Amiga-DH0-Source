#include <stdio.h> 

main() { 
char znak; 
/* Pytanie o liczbę */ 
printf("Podaj liczbę od 1 do 9: "); 
znak = getchar(); 
printf("\nOto ta liczba:"); 
/* wypisz liczbę */ 
putchar(znak); 
printf("\nTeraz wpisz coś,a program przepisze to.\n"); 
printf("Kiedy wpiszesz 'q',to nastąpi wyjście z programu\n"); 
/* pętla */ 
while(znak != 'q' ){ 
	znak = getchar(); 
	printf("\n"); 
	putchar(znak); 
	} 
	
printf("\nKoniec!!! Nacisnąłeś klawisz 'q'\n"); 
return(0); 
}

/*Źródło: https://www.ppa.pl/programy/kurs-jezyka-c-czesc-4.html
Copyright © Polski Portal Amigowy*/