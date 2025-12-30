#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

//#include <proto/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>

/* Define socklen_t if missing */
#if !defined(socklen_t)
typedef unsigned int socklen_t;
#endif

struct SocketBase *SocketBase;

int main(){

struct List *netiflist=NULL;
struct Node *node=NULL;
struct sockaddr addr;
unsigned int res=0;
struct SocketIFace ISocket;

  SocketBase=(struct Library *)OpenLibrary("bsdsocket.library",0);
  if (SocketBase!=NULL)
  {
   ISocket=(struct SocketIFace *)IExec->GetInterface(SocketBase,"main",1,NULL);
   if (ISocket!=NULL)
   {
    netiflist=ISocket->ObtainInterfaceList();
    if (netiflist!=NULL)
    {
     node=IExec->GetHead(netiflist);
     if (node!=NULL)
     {
      while (node!=NULL)
      {
       if (node->ln_Name!=NULL)
       {
        printf("Network interface %s:\\n",node->ln_Name);
        // QueryInterfaceTags returns 0 for success, -1 for error
        res=ISocket->QueryInterfaceTags(node->ln_Name,
                                        IFQ_Address,&addr,
                                        TAG_DONE);
        if (res==0)
        {
 printf("IP: %u.%u.%u.%u\\n",(uint8)addr.sa_data[2],(uint8)addr.sa_data[3],(uint8)addr.sa_data[4],(uint8)addr.sa_data[5]);
        }
        else
        { printf("- Error: Can't query information (%ld)\\n",res); }
       }

       node=IExec->GetSucc(node);
      }
     }
     else
     {
      printf("No network interfaces found\\n");
     }

     ISocket->ReleaseInterfaceList(netiflist);
    }
    IExec->DropInterface((struct Interface *)ISocket);
   }
   IExec->CloseLibrary(SocketBase);
   //if (SocketBase) CloseLibrary((struct Library*)SocketBase);
  }
  return 0;
}
