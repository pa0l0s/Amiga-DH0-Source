#include <stdio.h>
#include <exec/types.h>

LONG MemCmp(UBYTE *s1, UBYTE *s2, ULONG size)
{
	LONG diff=0;

	while (size > 0) {
		size--;
		if (diff = *s1++ - *s2++)
			break;
	}
			
	return(diff);
}

main()
{
	static UBYTE tab[]={1, 2, 3, 4}, tab2[]={1, 2, 4, 3};
	LONG wynik;

	wynik = MemCmp(tab, tab2, sizeof(tab));
	printf("%ld\n", wynik);

	return 0;
}