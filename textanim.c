#include<cstring>
#include<iostream>
void p(const char *str){
        write(1,str,strlen(str));
        sleep(1);
}
int main(){
        p("you "); p("can't "); p("see "); p("me");
        write(1,"\33[2K\r",5); //erase line and carriage return
        write(1,"\33[1A",4); // move 1 line up
        write(1,"\33[2K\r",5); //erase line and carriage return
        p("Ohh .. yeah!!\n");
        return 0;
}
