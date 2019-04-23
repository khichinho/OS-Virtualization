#include "types.h"
#include "stat.h"
#include "user.h"
#include "container.h"

int main(int argc, char* argv[]){
    printf(1,"first ps print\n");
    int cid = fork();
    if(cid == 0){
        create_container(0);
        join_container(0);
    }
    else{
        int cid2 = fork();
        if(cid2==0){
            create_container(1);
            join_container(1);
            ps();
        }
    }
    wait();
    exit();
}