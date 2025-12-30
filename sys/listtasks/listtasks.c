#include <exec/types.h>
#include <exec/tasks.h>
#include <exec/lists.h>
#include <exec/execbase.h>
#include <stdio.h>

extern struct ExecBase *SysBase;  // wskaźnik do ExecBase

int main(void) {
    struct Task *task;
    struct Node *node;

    // Zablokuj task switch, żeby nie zmieniały się taski w trakcie przeglądania
    Forbid();

    // SysBase->TaskReady to lista tasków gotowych
    for (task = (struct Task *) SysBase->TaskReady.lh_Head;
         task->tc_Node.ln_Succ != NULL;
         task = (struct Task *) task->tc_Node.ln_Succ)
    {
        printf("Task: %-16s  Priority: %d\n", task->tc_Node.ln_Name, task->tc_Node.ln_Pri);
    }

    Permit();
    return 0;
}
