/* Listing #3 */



#include <exec/execbase.h>



#include <proto/exec.h>



#include <stdio.h>



extern struct ExecBase *SysBase;



int main(int argc, char *argv[])

{

	struct Task *naszproces=SysBase->ThisTask;

	short licznik;

	printf("Jestem procesem o nazwie \"%s\" i priorytecie %d\n",

		naszproces->tc_Node.ln_Name, naszproces->tc_Node.ln_Pri);

	for (licznik=AFB_68040;; licznik--)

		if (licznik==-1 || (SysBase->AttnFlags & (1<<licznik)))

		{

			printf("Jestem uruchomiony na AMIDZE z procesorem 680%d0\n", licznik+1);

			break;

		}

	return 0;

}