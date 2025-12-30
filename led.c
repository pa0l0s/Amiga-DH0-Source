/*
UWAGA!
Poniższy program będzie działał prawidłowo tylko na Classic Amidze.
Nie będzie działał np. na Pegasosie, czy AOne.
Program ten odwołuje się bezpośrednio do hardware`u Amigi, a mianowicie
włącza i wyłącza filtr dolnoprzepustowy Amigi.
Co to jest i do czego służy? Amigę wyposażono w filtr dolnoprzepustowy
o częstotliwości granicznej rzędu 7 kHz. W wyniku tego wysokie częstotliwości
dźwięku zostają obcięte. To w nich powstają szumy. Niestety filtr działa
jednocześnie na czterech kanałach dźwiękowych co czyni dźwięk "przytłumionym".
Włączenie filtru objawia się przyciemnieniem diody LED Amigi.
Filtr ten włącza się ustawiając bit #1 w rejestrze $bfe001.
*/

/*Oto przykład:*/


#include <stdio.h>

int main()
{
    int *led;

    led = 0xBFE001;

    printf("Naciśnij enter aby wyzerowac bit 1\n");
    getchar();
    *led = *led & 0x11111110;
/* ************************************************** */

    printf("Nacisnij enter aby ustawic bit 1\n");
    getchar();
    *led = *led | 0x00000001;
/* ************************************************** */
    printf("Koniec&#92;n");
    return(0);

}
/*
Nie będe opisywał na razie co to są wskaźniki itd. Tym zajmiemy się w następnych
odcinkach kursu. W programie podkreślone są linijki kodu w których zastosowano
operatory bitowe, a mianowicie bitową koniunkcję (AND, u nas "&amp;" oraz bitową
alternatywę (OR, u nas "|").

Program czeka na naciśnięcie klawisza enter (funkcja "getchar()"), a następnie zeruje pierwszy
bit adresu $bfe001. Proszę spojrzeć na diodę LED Amigi i zobaczyć co się stanie.
Po ponownym nacisnięciu entera pierwszy bit zostaje ustawiony (zobacz diodę LED).
Ten prosty programik prezentuje praktyczne wykorzystanie operatorów bitowych.
Na razie nie będą nam one potrzebne i nie będziemy się nimi zajmować, ale na pewno do nich
wrócimy i wtedy je szerzej opiszę.
*/
