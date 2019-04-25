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

char* pathJoin(char* aa, char* bb){
  // int alen = strlen(a);
  // int blen = strlen(b);

  // printf(1,"%d %d\n", alen, blen);
  // char danda = '/';
  // char *os;
  // os = a;
  char *a = aa;
  char *b = bb;

  // pointing to the end of the 1st string
  while(*a)   // till it doesn't point to NULL-till string is not empty
  {
      a++;    // point to the next letter of the string
  }

  while(*b)   // till second string is not empty
  {
      *a = *b;
      b++;
      a++;
  }

  // *a = danda;
  // a++;

  // *a = '\0';  // string must end with '\0'
  // a--;

  return aa;

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
  // int mycontainer = give_cont();
  
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
      char* fn = fmtname(buf);
      printf(1,"filename: %s\n",fn);
      unlink(fn);
    }
    unlink("/0/containerFile");
    unlink("0");
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

