#include "types.h"
#include "stat.h"
#include "user.h"
#include "container.h"

int main(int argc, char* argv[]){
    printf(1,"first ps print\n");
    int cid = fork();
    int cid2;

    if(cid == 0){
        create_container(0);
        join_container(0);
    }
    else{
        cid2 = fork();
        if(cid2==0){
            create_container(1);
            join_container(1);
        }
        else{
            ps();
        }
    }
    wait();

    if(cid!=0 && cid2!=0){
        printf(1,"Destroying Container\n");
        destroy_container(0);
        destroy_container(1);
        
        
        ps();
    }
    exit();
}