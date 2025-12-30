#include <proto/exec.h>
#include <proto/dos.h>

struct DosLibrary *DOSBase;

int main() {
  if (DOSBase=(struct DosLibrary *)OpenLibrary("dos.library",0)) {
    Write(Output(),"Hello world!\n",14);
    CloseLibrary((struct Library *)DOSBase);
  }
  return 0;
}