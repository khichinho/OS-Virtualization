// #include "types.h"
// #include "user.h"
#include "container.h"
#include "lsd.c"

#include "fcntl.h"
int main(int argc, char* argv[]){
    printf(1,"TEST\n");
    // int cid = fork();
    // int cid2;

    // if(cid == 0){
    //     create_container(0);
    //     join_container(0);
    // }
    // else{
    //     cid2 = fork();
    //     if(cid2==0){
    //         create_container(1);
    //         join_container(1);
    //     }
    //     else{
    //         ps();
    //     }
    // }
    // wait();

    // if(cid!=0 && cid2!=0){
    //     printf(1,"Destroying Container\n");
    //     destroy_container(0);
    //     destroy_container(1);
    //     ps();
    // }

    // char yolo[5] = "yolo\n";

    // createContainer(0);
    // join_container(0);

    // chdir("0");

    // int fd = open("containerFile", O_CREATE | O_RDWR);
    // write(fd, &yolo, 5);
    // close(fd);

    // lsd();
    // chdir("..");

    // destroyContainer(0);
    // lsd();

    char* a = "000";
    char* b = "////";
    char *c = "222";
    char* res = pathJoin(a,b);
    printf(1,"res : %s\n", res);

    char* res2 = pathJoin(res,c);
    printf(1,"res : %s\n", res2);

    exit();
}