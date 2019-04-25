#include "types.h"
#include "stat.h"
#include "user.h"

#include "container.h"
char* gharbaar;

int
main(int argc, char *argv[])
{

  if(argc < 2){
    printf(2, "Usage: chdir directory...\n");
    exit();
  }

  if(chdir(argv[1]) < 0){
    printf(2, "chdir: %s failed to enter\n", argv[1]);
    exit();
  }

  gharbaar = argv[1];
  printf(2, "In DIR: %s \n", argv[1]);

  exit();
}
