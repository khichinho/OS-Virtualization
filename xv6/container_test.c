// #include "types.h"
// #include "user.h"
#include "container.h"
#include "lsd.c"

#include "fcntl.h"
int main(int argc, char* argv[]){
    
    // printf(1,"TEST\n");
    
    createContainer(1);
    createContainer(2);
    
    createContainer(3);
    // givels();
    char* kmessage = "Modified by : ";
    
    char kmyname[2];
    giveMy_pid(kmyname);
    char* kfirstname = "file_";
    char* kfname = joinStr(kfirstname,kmyname);
    int kfd = c_open(kfname, O_CREATE);
    close(kfd);
    kfd =  c_open(kfname, O_RDWR);
    // printf(1,"message to be written %s\n",kmessage);
    write(kfd, kmessage, 15);
    // write(kfd, kmyname,2);
    close(kfd);
    printf(1,"\n");
    // givels();
    printf(1,"\n");

    int cid1 = fork();
    if(cid1 == 0){
        join_container(1);
        char* message1 = "Modified by : ";
        char myname1[2];
        giveMy_pid(myname1);
        // printf(1,"myname %s\n",myname1);
        char* firstname1 = "file_";
        // printf(1,"firstname :%s, myname %s\n",firstname1,myname1);
        char* fname1 = joinStrfname(firstname1,myname1);
        // printf(1,"firstname :%s, fname :%s, myname %s\n",firstname1,fname1,myname1);
        int fd = c_open(fname1, O_CREATE);
        // int fd = c_open(firstname1, O_CREATE);
        close(fd);
        fd =  c_open(fname1, O_RDWR);
        // fd =  c_open(firstname1, O_RDWR);
        write(fd, message1, 15);
        
        write(fd, myname1,2);
        // write(fd, firstname1,2);
        close(fd);
        // sleep(5);
        // ps();
        leaveContainer(1);
        
        // exit();
    }
    // sleep(1);
    int cid2 = fork();
    if(cid2 == 0){
        join_container(1);
        char* message2 = "Modified by : ";
        char myname2[2];
        giveMy_pid(myname2);
        // printf(1,"myname %s\n",myname2);
        char* firstname2 = "file_";
        // printf(1,"firstname :%s, myname %s\n",firstname2,myname2);
        char* fname2 = joinStrfname(firstname2,myname2);
        // printf(1,"firstname :%s, fname :%s, myname %s\n",firstname1,fname1,myname1);
        int fd = c_open(fname2, O_CREATE);
        // int fd = c_open(firstname1, O_CREATE);
        close(fd);
        fd =  c_open(fname2, O_RDWR);
        // fd =  c_open(firstname1, O_RDWR);
        write(fd, message2, 15);
        
        write(fd, myname2,2);
        // write(fd, firstname1,2);
        close(fd);
        // sleep(1);
        leaveContainer(1);
        // sleep(4);
        // ps();
        // exit();
    }
    // sleep(1);
    int cid3 = fork();
    if(cid3 == 0){
        join_container(1);
        char* message3 = "Modified by : ";
        char myname3[2];
        giveMy_pid(myname3);
        // printf(1,"myname %s\n",myname3);
        char* firstname3 = "file_";
        // printf(1,"firstname :%s, myname %s\n",firstname3,myname3);
        char* fname3 = joinStrfname(firstname3,myname3);
        // printf(1,"firstname :%s, fname :%s, myname %s\n",firstname1,fname1,myname1);
        int fd = c_open(fname3, O_CREATE);
        // int fd = c_open(firstname1, O_CREATE);
        close(fd);
        fd =  c_open(fname3, O_RDWR);
        // fd =  c_open(firstname1, O_RDWR);
        write(fd, message3, 15);
        
        write(fd, myname3,2);
        // write(fd, firstname1,2);
        close(fd);
        // sleep(1);
        leaveContainer(1);
        // ps();   
        sleep(4);
        // exit();
    }

    int cid4 = fork();
    if(cid4 == 0){
        join_container(2);
        char* message4 = "Modified by : ";
        char myname4[2];
        giveMy_pid(myname4);
        // printf(1,"myname %s\n",myname4);
        char* firstname4 = "file_";
        // printf(1,"firstname :%s, myname %s\n",firstname4,myname4);
        char* fname4 = joinStrfname(firstname4,myname4);
        // printf(1,"firstname :%s, fname :%s, myname %s\n",firstname1,fname1,myname1);
        int fd = c_open(fname4, O_CREATE);
        // int fd = c_open(firstname1, O_CREATE);
        close(fd);
        fd =  c_open(fname4, O_RDWR);
        // fd =  c_open(firstname1, O_RDWR);
        write(fd, message4, 15);
        
        write(fd, myname4,2);
        // write(fd, firstname1,2);
        close(fd);
        leaveContainer(2);
        // givels();
        // ps();
        // exit();
    }
    // wait();

    int cid5 = fork();
    if(cid5 == 0){
        join_container(3);
        char* message5 = "Modified by : ";
        char myname5[2];
        giveMy_pid(myname5);
        // printf(1,"myname %s\n",myname5);
        char* firstname5 = "file_";
        // printf(1,"firstname :%s, myname %s\n",firstname5,myname5);
        char* fname5 = joinStrfname(firstname5,myname5);
        // printf(1,"firstname :%s, fname :%s, myname %s\n",firstname1,fname1,myname1);
        int fd = c_open(fname5, O_CREATE);
        // int fd = c_open(firstname1, O_CREATE);
        close(fd);
        fd =  c_open(fname5, O_RDWR);
        // fd =  c_open(firstname1, O_RDWR);
        write(fd, message5, 15);
        
        write(fd, myname5,2);
        // write(fd, firstname1,2);
        close(fd);
        // ps();    
        leaveContainer(3);
        // givels();
        
        // exit();
    }
    
    // schedular_log_on();
    // sleep(5);
    // schedular_log_off();
    // ps();
    wait();
    // givels();
    
    // destroyContainer(1);
    // givels();
    // destroyContainer(2);
    // destroyContainer(3);
    exit();
}