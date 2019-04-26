#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

char* joinPath(char* aa, char* bb){

  char *a = aa;
  char *b = bb;
  char *danda = "/";
  
  while(*a){ a++;}
  strcpy(a,danda);

  a = aa;
  while(*a){ a++;}
  strcpy(a,b);

  return aa;
}

char* itoa(int num)
{
  char* out;
  if(num == 0){ out = "0";}
  else if(num == 1){ out = "1";}
  else if(num == 2){ out = "2";}
  else if(num == 3){ out = "3";}
  else if(num == 4){ out = "4";}
  else if(num == 5){ out = "5";}
  else if(num == 6){ out = "6";}
  else if(num == 7){ out = "7";}
  else if(num == 8){ out = "8";}
  else{ out = "9";}

  return out;
}

void
lsd()
{
  char* one_dot = ".";
  char* two_dot = "..";
  int mycontainer = give_cont();
  printf(2,"container id from proc %d\n",mycontainer);
  char* path = ".";
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      char* fn = fmtname(buf);
      if(fn == one_dot || fn == two_dot){}
      else{ 
        printf(1, "%s %d %d %d\n", fn, st.type, st.ino, st.size);
      }
    }
    break;
  }
  close(fd);
}


void
rmdir(char* folder)
{
  // char* one_dot = ".";
  // char* two_dot = "..";
  int cont_id = give_cont();

  printf(2,"removing folder %s\n",folder);
  
  char* path = folder;
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    // printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      
      char* myContainer =  itoa(cont_id);
      char* fn = fmtname(buf);
      printf(1,"container name : %s\n",myContainer);
      printf(1,"filename : %s\n",fn);
      char* completePath = joinPath(myContainer, fn);
      printf(1,"complete path : %s\n",completePath);
      unlink(completePath);
    }
    // unlink(itoa(myContainer));
    break;
  }
  close(fd);
}


int createContainer(int id){
    mkdir("0");
    return create_container(id); 
}

int destroyContainer(int id){
  
  rmdir("0");
  return destroy_container(id) ;
}

