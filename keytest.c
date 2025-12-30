#include <stdio.h>

int main(){
	char znak;
	char string[80];
	int ichars; /*number_of chars read*/
	int i=0;
	printf("\nTeraz wpisz coś,a program przepisze to.\n"); 
	printf("Kiedy wpiszesz 'q',to nastąpi wyjście z programu\n"); 
	/* pętla */ 
	/*
	while(znak != 'q' ){ 
		znak = getchar(); 
		printf("\n"); 
		

		putchar(znak); 
	} */
	
	while(ichars==65535|ichars==0){
		i++;	
		ichars = scanf("%s",string);
	};
	
	printf("%s - %i - %i",string,ichars,i);
	
	printf("\nKoniec!!! Nacisnąłeś klawisz 'q'\n"); 
	return 0;
}