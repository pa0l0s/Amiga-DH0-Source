#include <exec/types.h>
#include <exec/tasks.h>
#include <exec/execbase.h>
#include <exec/lists.h>
#include <devices/timer.h>
#include <proto/exec.h>
#include <proto/timer.h>
#include <stdio.h>
#include <string.h>

extern struct ExecBase *SysBase;

/* ---------------- CPU sampling ---------------- */

#define MAX_TASKS 128
#define SAMPLE_COUNT 250   /* 250 * 20ms ≈ 5s */

struct TaskStat {
    struct Task *task;
    ULONG samples;
};

struct TaskStat stats[MAX_TASKS];
ULONG statCount = 0;
ULONG totalSamples = 0;

void SampleTask(void)
{
    struct Task *t = SysBase->ThisTask;
    ULONG i;

    for (i = 0; i < statCount; i++)
    {
        if (stats[i].task == t)
        {
            stats[i].samples++;
            totalSamples++;
            return;
        }
    }

    if (statCount < MAX_TASKS)
    {
        stats[statCount].task = t;
        stats[statCount].samples = 1;
        statCount++;
        totalSamples++;
    }
}

float GetCPU(struct Task *t)
{
    ULONG i;
    for (i = 0; i < statCount; i++)
        if (stats[i].task == t)
            return (stats[i].samples * 100.0f) / totalSamples;
    return 0.0f;
}

/* ---------------- Task listing ---------------- */

void PrintTask(struct Task *t, const char *state)
{
    printf("%-22s %-7s Pri:%3ld Stack:%6lu SP:0x%08lx CPU:%5.2f%%\n",
        t->tc_Node.ln_Name ? t->tc_Node.ln_Name : "(noname)",
        state,
        t->tc_Node.ln_Pri,
        t->tc_SPUpper - t->tc_SPLower,
        (ULONG)t->tc_SPReg,
        GetCPU(t)
    );
}

void ListTasks(void)
{
    struct Task *t;

    printf("\nTASK LIST:\n");
    printf("---------------------------------------------------------------\n");

    /* Running */
    PrintTask(SysBase->ThisTask, "RUN");

    /* Ready list */
    for (t = (struct Task *)SysBase->TaskReady.lh_Head;
         t->tc_Node.ln_Succ;
         t = (struct Task *)t->tc_Node.ln_Succ)
    {
        PrintTask(t, "READY");
    }

    /* Waiting list */
    for (t = (struct Task *)SysBase->TaskWait.lh_Head;
         t->tc_Node.ln_Succ;
         t = (struct Task *)t->tc_Node.ln_Succ)
    {
        PrintTask(t, "WAIT");
    }
}

/* ---------------- main ---------------- */

int main(void)
{
    struct MsgPort *timerPort;
    struct timerequest *tr;
    ULONG i;

    timerPort = CreateMsgPort();
    if (!timerPort) return 20;

    tr = (struct timerequest *)CreateIORequest(timerPort, sizeof(struct timerequest));
    if (!tr) return 20;

    if (OpenDevice(TIMERNAME, UNIT_MICROHZ, (struct IORequest *)tr, 0))
        return 20;

    for (i = 0; i < SAMPLE_COUNT; i++)
    {
        tr->tr_node.io_Command = TR_ADDREQUEST;
        tr->tr_time.tv_secs = 0;
        tr->tr_time.tv_micro = 20000; /* 20 ms */
        DoIO((struct IORequest *)tr);
        SampleTask();
    }

    CloseDevice((struct IORequest *)tr);
    DeleteIORequest((struct IORequest *)tr);
    DeleteMsgPort(timerPort);

    ListTasks();
    return 0;
}
