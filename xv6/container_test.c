// #include "types.h"
// #include "user.h"
#include "container.h"
#include "lsd.c"

#include "fcntl.h"
int main(int argc, char* argv[]){
    
    // printf(1,"TEST\n");
    char* message = "Modified by : ";

    createContainer(1);
    createContainer(2);
    
    create_container(3);
    char kmyname[2];
    giveMy_pid(kmyname);
    char* kfirstname = "file_";
    char* kfname = joinStr(kfirstname,&kmyname);
    int kfd = c_open(kfname, O_CREATE);
    close(kfd);
    kfd =  c_open(kfname, O_RDWR);
    write(kfd, message, 15);
    
    write(kfd, &kmyname,2);
    close(kfd);

    int cid1 = fork();
    if(cid1 == 0){
        join_container(1);
        char myname[2];
        giveMy_pid(myname);
        char* firstname = "file_";
        char* fname = joinStr(firstname,&myname);
        int fd = c_open(fname, O_CREATE);
        close(fd);
        fd =  c_open(fname, O_RDWR);
        write(fd, message, 15);
        
        write(fd, &myname,2);
        close(fd);
        leaveContainer(1);
    }
    int cid2 = fork();
    if(cid2 == 0){
        join_container(1);
        char myname[2];
        giveMy_pid(myname);
        char* firstname = "file_";
        char* fname = joinStr(firstname,&myname);
        int fd = c_open(fname, O_CREATE);
        close(fd);
        fd =  c_open(fname, O_RDWR);
        write(fd, message, 15);
        
        write(fd, &myname,2);
        close(fd);
        leaveContainer(1);
    }
    int cid3 = fork();
    if(cid3 == 0){
        join_container(1);
        ps();
        char myname[2];
        giveMy_pid(myname);
        char* firstname = "file_";
        char* fname = joinStr(firstname,&myname);
        int fd = c_open(fname, O_CREATE);
        close(fd);
        fd =  c_open(fname, O_RDWR);
        write(fd, message, 15);
        
        write(fd, &myname,2);
        close(fd);
        leaveContainer(1);
    }

    int cid4 = fork();
    if(cid4 == 0){
        join_container(2);
        ps();
        char myname[2];
        giveMy_pid(myname);
        char* firstname = "file_";
        char* fname = joinStr(firstname,&myname);
        int fd = c_open(fname, O_CREATE);
        close(fd);
        fd =  c_open(fname, O_RDWR);
        write(fd, message, 15);
        
        write(fd, &myname,2);
        close(fd);
        leaveContainer(2);
    }

    int cid5 = fork();
    if(cid5 == 0){
        join_container(3);
        ps();
        char myname[2];
        giveMy_pid(myname);
        char* firstname = "file_";
        char* fname = joinStr(firstname,&myname);
        int fd = c_open(fname, O_CREATE);
        close(fd);
        fd =  c_open(fname, O_RDWR);
        write(fd, message, 15);
        
        write(fd, &myname,2);
        close(fd);
        leaveContainer(2);
    }

    schedular_log_on();

    schedular_log_off();
    
    givels();

    destroyContainer(1);
    destroyContainer(2);
    destroyContainer(3);
    exit();
}