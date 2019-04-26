#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

#include "container.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}




/////////// START HERE ///////////

void sys_schedular_log(int enable){
  int eb;
  argint(0,&eb);
  schedular_log(eb);
}

int sys_get_pid(){
  get_pid();
}

void sys_ps(){
  ps();
}

int sys_give_cont(){
  return give_cont();
}

int sys_get_current_proc(int cont_num){
  int c_id;
  argint(0, &c_id);
  return get_current_proc(c_id);  
}

int sys_get_max_proc(int cont_num){
  int c_id;
  argint(0, &c_id);
  return get_max_proc(c_id);  
}

int sys_get_total_proc(int cont_num){
  int c_id;
  argint(0, &c_id);
  return get_total_proc(c_id);  
}

void sys_container_init(){
  container_init();
  return;
}

void sys_set_proc_container(int id){
  int c_id;
  argint(0, &c_id);
  set_proc_container(c_id);
}


int sys_get_container_index(int id){
  int c_id;
  argint(0, &c_id);
  return get_container_index(c_id);
}

int sys_create_container(int id){
  int c_id;
  argint(0, &c_id);
  int success = create_container(c_id);
  return success;
}

void sys_join_container(int id){
  int c_id;
  argint(0, &c_id);
  join_container(c_id);
  return;
}

void sys_leave_container(int id){
  int c_id;
  argint(0, &c_id);
  leave_container(c_id);
  return;
}

void sys_destroy_container(int id){
  int c_id;
  argint(0, &c_id);
  destroy_container(c_id);
  return;
}
