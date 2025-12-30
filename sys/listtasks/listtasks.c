#include <exec/types.h>
#include <exec/tasks.h>
#include <exec/lists.h>
#include <exec/execbase.h>
#include <stdio.h>

extern struct ExecBase *SysBase;

int main(void) {
    struct Task *task;

    printf("Listing all tasks in the system:\n\n");

    Forbid();  // blokujemy task switching

    for (task = (struct Task *) SysBase->TaskReady.lh_Head;
         task->tc_Node.ln_Succ != NULL;
         task = (struct Task *) task->tc_Node.ln_Succ)
    {
        printf("Task: %-16s  Pri: %3d  NodeType: %d\n",
               task->tc_Node.ln_Name,
               task->tc_Node.ln_Pri,
               task->tc_Node.ln_Type);
    }

    Permit();

    return 0;
}
