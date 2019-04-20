#include "types.h"
#include "user.h"

int main(int argc, char **argv)
{
  if(argc > 2){
    printf(2, "usage: destroy_container container_num\n");
    exit();
  }
  
  destroy_container(0);
  
  exit();
}
