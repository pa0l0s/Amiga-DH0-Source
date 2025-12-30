#include <exec/types.h>
#include <exec/tasks.h>
#include <exec/lists.h>
#include <exec/execbase.h>
#include <dos/dos.h>
#include <stdio.h>

extern struct ExecBase *SysBase;

/* Zamiana typu noda na czytelny tekst */
const char *nodeTypeName(UBYTE type)
{
    switch (type)
    {
        case NT_TASK:    return "Task";
        case NT_PROCESS: return "Process";
        default:         return "Unknown";
    }
}

/* Wypisanie listy tasków */
void ListTasks(struct List *list, const char *state)
{
    struct Task *task;

    for (task = (struct Task *)list->lh_Head;
         task->tc_Node.ln_Succ != NULL;
         task = (struct Task *)task->tc_Node.ln_Succ)
    {
        printf("%-8s | %-24s | Pri: %3d | Type: %-7s\n",
               state,
               task->tc_Node.ln_Name ? task->tc_Node.ln_Name : "(noname)",
               task->tc_Node.ln_Pri,
               nodeTypeName(task->tc_Node.ln_Type));
    }
}

int main(void)
{
    printf("\n=== AmigaOS 3.1 Task List ===\n\n");

    Forbid();   /* blokujemy przełączanie tasków */

    printf("STATE    | NAME                     | DETAILS\n");
    printf("---------+--------------------------+-----------------------------\n");

    /* Taski gotowe do uruchomienia */
    ListTasks(&SysBase->TaskReady, "READY");

    /* Taski czekające */
    ListTasks(&SysBase->TaskWait, "WAITING");

    Permit();

    printf("\nDone.\n");
    return 0;
}
