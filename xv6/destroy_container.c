#include "types.h"
#include "user.h"

int main(int argc, char **argv)
{
  if(argc != 2){
    printf(2, "usage: create_container\n");
    exit();
  }
  int num_container = atoi(argv[1]);
  create_container(num_container);
  
  exit();
  
  return 0;
}
