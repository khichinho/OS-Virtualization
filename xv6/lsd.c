#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"


char isspace (unsigned char c) 
{
  if ( c == ' '
    || c == '\f'
    || c == '\n'
    || c == '\r'
    || c == '\t'
    || c == '\v' ) 
      return 1;

  return 0;
}
void*
lsdmemmove(void *dst, const void *src, uint n)
{
  const char *s;
  char *d;

  s = src;
  d = dst;
  if(s < d && s + n > d){
    s += n;
    d += n;
    while(n-- > 0)
      *--d = *--s;
  } else
    while(n-- > 0)
      *d++ = *s++;

  return dst;
}

char* trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

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
  char *end = "\0";
  
  while(*a){ a++;}
  strcpy(a,danda);

  a = aa;
  while(*a){ a++;}
  strcpy(a,b);

  a = aa;
  while(*a){ a++;}
  strcpy(a,end);

  return aa;
}

char* joinStr(char* aa, char* bb){

  char *a = aa;
  char *b = bb;
  char *end = "\0";
  
  while(*a){ a++;}
  strcpy(a,b);

  a = aa;
  while(*a){ a++;}
  strcpy(a,end);

  return aa;
}

void correctStr(char* aa, char* bb){

  char *a = aa;
  char *end = "\0";
  
  strcpy(aa,bb);
  a = aa;
  while(*a){ a++;}
  strcpy(a,end);

}

char* itoa(int num)
{
  char* out = malloc(1);
  printf(1,"address given to char* : %d\n",out);
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
  printf(1,"value of char* : %s\n",out);
  return out;
}

char* itoafolder(int num)
{
  char* out = malloc(1);
  printf(1,"address given to char* : %d\n",out);
  char* one = malloc(1);
  char* two = malloc(1);
  char* three = malloc(1);
  char* four = malloc(1);
  char* five = malloc(1);
  char* six = malloc(1);
  char* seven = malloc(1);
  char* eight = malloc(1);
  char* nine = malloc(1);
  char* zero = malloc(1);
  
  one   = "1";
  two   = "2";
  three = "3";
  four  = "4";
  five  = "5";
  six   = "6";
  seven = "7";
  eight = "8";
  nine  = "9";
  zero  = "0";


  if(num == 0){ out = zero;}
  else if(num == 1){ out = one;}
  else if(num == 2){ out = two;}
  else if(num == 3){ out = three;}
  else if(num == 4){ out = four;}
  else if(num == 5){ out = five;}
  else if(num == 6){ out = six;}
  else if(num == 7){ out = seven;}
  else if(num == 8){ out = eight;}
  else{ out = nine;}
  printf(1,"value of char* : %s\n",out);
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
rmFiles(int folder)
{ 
  char* one_dot = ".";
  char* two_dot = "..";

  char* patht = itoa(folder);
  char* path = malloc(1);
  lsdmemmove(path,patht,1);
  // free(patht);
  
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
      
      int cont_id = give_cont();
      char* fn2 = fmtname(buf);
      char* fn = trimwhitespace(fn2);
      printf(1,"fileName: %s\n", fn);


      if(*fn == *one_dot || *fn == *two_dot){}
      else{

        char *contStr = itoa(cont_id);
        printf(1,"contID: %d\n", cont_id);
        printf(1,"filePath: %s\n", contStr);

        char *completePath = joinPath(contStr, fn);
        // printf(1,"Complete Path: %s\n",completePath);

        if(unlink(completePath) < 0){
          printf(1, "rm: %s failed to delete\n", completePath);
        }
        else{
            printf(1, "rm: deleted file %s\n", completePath);
        }

        // if(strcmp(fn, "File") == 0){ printf(1,"%s == File\n",fn);}
        // else{ printf(1,"%s != File\n",fn);}
        // free(contStr);
      }
    }

    break;
  }
  close(fd);
}


void
rmdir(int num){
    printf(1,"rm : deleting folder %d\n",num);
    if(num == 0){ unlink("/0/");}
    else if(num == 1){ unlink("/1/");}
    else if(num == 2){ unlink("/2/");}
    else if(num == 3){ unlink("/3/");}
    else if(num == 4){ unlink("/4/");}
    else if(num == 5){ unlink("/5/");}
    else if(num == 6){ unlink("/6/");}
    else if(num == 7){ unlink("/7/");}
    else if(num == 8){ unlink("/8/");}
    else{ unlink("/9/");}
    // for(int i=0;i<100;i++){
    //   char *folder_num = itoafolder(id);
    //   // printf(1, "Folder to delete %s\n", folder_num);
      
    //   if(unlink(folder_num) < 0){
    //     // printf(1, "rm: %s/ failed to delete\n", folder_num);
    //   }
    //   else{
    //     printf(1, "rm: deleted file %s\n", folder_num);
    //     return;
    //   }
    // }
    
}

int createContainer(int id){
  printf(1,"creating container %d\n", id);
  mkdir(itoa(id));
  return create_container(id); 
}

int destroyContainer(int id){
  
  printf(1,"removing container %d\n", id);
  rmFiles(id);
  rmdir(id);
  return destroy_container(id) ;
}

