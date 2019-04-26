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
    // createContainer(1);
    // // lsd();
    // int cid = fork();
    // if(cid==0){
    //     schedular_log_on();
    //     join_container(0);
    //     // chdir("0");

    //     int fd = c_open("File", O_CREATE | O_RDWR);
    //     write(fd, &yolo, 5);
    //     close(fd);
    //     int fd2 = c_open("File2", O_CREATE | O_RDWR);
    //     write(fd2, &yolo, 5);
    //     close(fd2);
    //     // chdir("..");
    //     // givels();
    //     // ps();
    //     destroyContainer(0);
    //     // schedular_log_off();
    // // chdir("..");
    //     exit();
    // }
    // else{
    //     // wait();
        
    //     join_container(1);
    //     // ps();
    //     // chdir("1");
    //     int fd3 = c_open("File3", O_CREATE);
    //     close(fd3);
    //     fd3 =  c_open("File3", O_RDWR);
    //     write(fd3, &yolo, 5);
    //     close(fd3);
    //     int fd4 = c_open("File42", O_CREATE);
    //     close(fd3);
    //     fd4 =  c_open("File42", O_RDWR);
    //     write(fd4, &yolo, 5);
    //     close(fd4);
    //     // chdir("..");
    //     // givels();
    //     // destroyContainer(1);
    // }
    

    
    char* myname = giveMy_pid();
    printf(1,"my pid name is : %s",myname);
    
    


    // char* a = "0";
    // char* b = "File";
    // char* res = joinPath(a,b);
    // printf(1,"res : %s\n", res);
    // printf(1, "%d", strcmp(res, "0/File"));

    exit();
}