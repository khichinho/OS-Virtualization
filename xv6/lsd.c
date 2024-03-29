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

  ///////////////////////////////////////////
  ///////////////////////////////////////////
  //// baht pareshaan kiya hai isne ->   ////
  ////      while(*a){ a++;}             ////
  ///////////////////////////////////////////
  ///////////////////////////////////////////
  
  a++;
  strcpy(a,danda);

  a = aa;
  while(*a){ a++;}
  strcpy(a,b);

  return aa;
}

char* joinStrfname(char* aa, char* bb){

  char *a = aa;
  char *b = bb;
  char *end = "\0";
  int i=0;
  while(*a && i<5){ a++;i++;}
  strcpy(a,b);
  while(*a && i<7){ a++;i++;}
  strcpy(a,end);
  return aa;
}

char* joinStr(char* aa, char* bb){

  char *a = aa;
  char *b = bb;
  // char *end = "\0";
  
  while(*a){ a++;}
  strcpy(a,b);

  // a = aa;
  // // while(*a){ a++;}
  // // strcpy(a,end);

  return aa;
}

char* joinFolder(char* aa){

  char *a = aa;
  char *b = "/";

  a++;
  strcpy(a,b);

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
  // printf(1,"num passed to itoa : %d\n",num);
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
  // printf(1,"itoa returning : %s\n",out);

  return out;
}

void
lsd(char *inpath)
{
  char* one_dot = ".";
  char* two_dot = "..";
  // int mycontainer = give_cont();
  // printf(2,"container id from proc %d\n",mycontainer);
  char* path = inpath;
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
      if(*inpath == *one_dot){
        printf(1, "%s %d %d %d\n", fn, st.type, st.ino, st.size);
      }
      else{
        if(*fn == *one_dot || *fn == *two_dot){}
        else{ 
          printf(1, "%s %d %d %d\n", fn, st.type, st.ino, st.size);
        }
      }
    }
    break;
  }
  close(fd);
}


void
rmFiles(char* folder)
{
  char* one_dot = ".";
  char* two_dot = "..";

  char* path = folder;
  printf(1,"removing files of folder %s\n",folder);
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
      
      // int cont_id = give_cont();
      char* fn2 = fmtname(buf);
      char* fn = trimwhitespace(fn2);
      // printf(1,"fileName: %s\n", fn);


      if(*fn == *one_dot || *fn == *two_dot){}
      else{

        char *contStr = path;
        // printf(1,"contID: %d\n", cont_id);
        // printf(1,"filePath: %s\n", contStr);

        char *completePath = joinPath(contStr, fn);
        // printf(1,"Complete Path: %s\n",completePath);

        if(unlink(completePath) < 0){
          printf(1, "rm: %s failed to delete file\n", completePath);
        }
        else{
            // printf(1, "rm: deleted file %s\n", completePath);
        }
      }
    }

    break;
  }
  close(fd);
}

void
rmdir2(int id){
    // printf(1,"removing container %d's folder \n",id);
    char *cont_num = itoa(id);
    if(unlink(joinFolder(cont_num)) < 0){
      printf(1, "rm: failed to remove container %s's folder \n", cont_num);
    }
    else{
      
      printf(1, "removed container : %d\n", id);
    }
    
}

void givels(){
    int cont = give_cont();
    if(cont<0){
        lsd(".");
    }
    else{
        lsd(".");
        char* folder = itoa(cont);

        lsd(joinFolder(folder));

    }
}

void schedular_log_on(){
  schedular_log(1);
}
void schedular_log_off(){
  schedular_log(0);
}

void give_string(int p, char* out){
  // char out[2];
  int one = p/10;
  int two = p%10;

  out[0] = one + '0';
  out[1] = two + '0';
  // char* 
  // return out;
}

void giveMy_pid(char* name){
  int p_id  = get_pid();
  // char name[2];
  give_string(p_id,name);
  // return name
}
int createContainer(int id){
  printf(1,"creating container %d\n", id);
  char dt[2];
  give_string(id, dt);
  // mkdir(itoa(id));
  mkdir(dt);
  return create_container(id); 
}

void leaveContainer(int id){
  leave_container(id);
}

int destroyContainer(int id){
  
  printf(1,"removing container %d\n", id);
  printf(1,".\n");
  printf(1,"...\n");
  // char* myfolder = itoa(id);
  char myfolder[2];
  give_string(id, myfolder);
  printf(1,"passing folder's id : %d\n",id);
  printf(1,"passing folder name : %s\n",myfolder);
  rmFiles(myfolder);
  rmdir2(id);
  
  return destroy_container(id) ;
}

int c_open(char* name, int option){
  int c_num = give_cont();
  // printf(1,"c_open - container folder : %d\n",c_num);
  if(c_num<0 || c_num>9){
    int fd = open(name, option);
    chdir("..");
    return fd;
  }else{
    // char* fn = itoa(c_num);
    // char* fname = joinFolder(fn);
    char* fn = "";
    give_string(c_num,fn);  
    
    printf(1,"what is fn: %s\n",fn);
    char* fname = joinFolder(fn);
    chdir(fname);
    int fd = open(name, option);
    chdir("..");
    return fd;
  }
}

// int main(int argc, char** argv){
//   lsd();
//   return 0;
// }