#include "types.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "x86.h"
#include "proc.h"
#include "spinlock.h"

#include "container.h"
int schedular_log_status=0;
struct {
  struct spinlock lock;
  struct proc proc[NPROC];
} ptable;

static struct proc *initproc;

int nextpid = 1;
extern void forkret(void);
extern void trapret(void);

static void wakeup1(void *chan);

void
pinit(void)
{
  initlock(&ptable.lock, "ptable");
}

// Must be called with interrupts disabled
int
cpuid() {
  return mycpu()-cpus;
}

// Must be called with interrupts disabled to avoid the caller being
// rescheduled between reading lapicid and running through the loop.
struct cpu*
mycpu(void)
{
  int apicid, i;
  
  if(readeflags()&FL_IF)
    panic("mycpu called with interrupts enabled\n");
  
  apicid = lapicid();
  // APIC IDs are not guaranteed to be contiguous. Maybe we should have
  // a reverse map, or reserve a register to store &cpus[i].
  for (i = 0; i < ncpu; ++i) {
    if (cpus[i].apicid == apicid)
      return &cpus[i];
  }
  panic("unknown apicid\n");
}

// Disable interrupts so that we are not rescheduled
// while reading proc from the cpu structure
struct proc*
myproc(void) {
  struct cpu *c;
  struct proc *p;
  pushcli();
  c = mycpu();
  p = c->proc;
  popcli();
  return p;
}

//PAGEBREAK: 32
// Look in the process table for an UNUSED proc.
// If found, change state to EMBRYO and initialize
// state required to run in the kernel.
// Otherwise return 0.
static struct proc*
allocproc(void)
{
  struct proc *p;
  char *sp;

  acquire(&ptable.lock);

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == UNUSED)
      goto found;

  release(&ptable.lock);
  return 0;

found:
  p->state = EMBRYO;
  p->pid = nextpid++;

  release(&ptable.lock);

  // Allocate kernel stack.
  if((p->kstack = kalloc()) == 0){
    p->state = UNUSED;
    return 0;
  }
  sp = p->kstack + KSTACKSIZE;

  // Leave room for trap frame.
  sp -= sizeof *p->tf;
  p->tf = (struct trapframe*)sp;

  // Set up new context to start executing at forkret,
  // which returns to trapret.
  sp -= 4;
  *(uint*)sp = (uint)trapret;

  sp -= sizeof *p->context;
  p->context = (struct context*)sp;
  memset(p->context, 0, sizeof *p->context);
  p->context->eip = (uint)forkret;

  //edited
  p->in_kernal=1;

  return p;
}

//PAGEBREAK: 32
// Set up first user process.
void
userinit(void)
{
  struct proc *p;
  extern char _binary_initcode_start[], _binary_initcode_size[];

  p = allocproc();
  
  initproc = p;
  if((p->pgdir = setupkvm()) == 0)
    panic("userinit: out of memory?");
  inituvm(p->pgdir, _binary_initcode_start, (int)_binary_initcode_size);
  p->sz = PGSIZE;
  memset(p->tf, 0, sizeof(*p->tf));
  p->tf->cs = (SEG_UCODE << 3) | DPL_USER;
  p->tf->ds = (SEG_UDATA << 3) | DPL_USER;
  p->tf->es = p->tf->ds;
  p->tf->ss = p->tf->ds;
  p->tf->eflags = FL_IF;
  p->tf->esp = PGSIZE;
  p->tf->eip = 0;  // beginning of initcode.S

  safestrcpy(p->name, "initcode", sizeof(p->name));
  p->cwd = namei("/");

  // this assignment to p->state lets other cores
  // run this process. the acquire forces the above
  // writes to be visible, and the lock is also needed
  // because the assignment might not be atomic.
  acquire(&ptable.lock);

  p->state = RUNNABLE;

  release(&ptable.lock);
}

// Grow current process's memory by n bytes.
// Return 0 on success, -1 on failure.
int
growproc(int n)
{
  uint sz;
  struct proc *curproc = myproc();

  sz = curproc->sz;
  if(n > 0){
    if((sz = allocuvm(curproc->pgdir, sz, sz + n)) == 0)
      return -1;
  } else if(n < 0){
    if((sz = deallocuvm(curproc->pgdir, sz, sz + n)) == 0)
      return -1;
  }
  curproc->sz = sz;
  switchuvm(curproc);
  return 0;
}

// Create a new process copying p as the parent.
// Sets up stack to return as if from system call.
// Caller must set state of returned proc to RUNNABLE.
int
fork(void)
{
  int i, pid;
  struct proc *np;
  struct proc *curproc = myproc();

  // Allocate process.
  if((np = allocproc()) == 0){
    return -1;
  }

  // Copy process state from proc.
  if((np->pgdir = copyuvm(curproc->pgdir, curproc->sz)) == 0){
    kfree(np->kstack);
    np->kstack = 0;
    np->state = UNUSED;
    return -1;
  }
  np->sz = curproc->sz;
  np->parent = curproc;
  *np->tf = *curproc->tf;

  // Clear %eax so that fork returns 0 in the child.
  np->tf->eax = 0;

  for(i = 0; i < NOFILE; i++)
    if(curproc->ofile[i])
      np->ofile[i] = filedup(curproc->ofile[i]);
  np->cwd = idup(curproc->cwd);

  safestrcpy(np->name, curproc->name, sizeof(curproc->name));

  pid = np->pid;

  acquire(&ptable.lock);

  np->state = RUNNABLE;

  release(&ptable.lock);

  return pid;
}

// Exit the current process.  Does not return.
// An exited process remains in the zombie state
// until its parent calls wait() to find out it exited.
void
exit(void)
{
  struct proc *curproc = myproc();
  struct proc *p;
  int fd;

  if(curproc == initproc)
    panic("init exiting");

  // Close all open files.
  for(fd = 0; fd < NOFILE; fd++){
    if(curproc->ofile[fd]){
      fileclose(curproc->ofile[fd]);
      curproc->ofile[fd] = 0;
    }
  }

  begin_op();
  iput(curproc->cwd);
  end_op();
  curproc->cwd = 0;

  acquire(&ptable.lock);

  // Parent might be sleeping in wait().
  wakeup1(curproc->parent);

  // Pass abandoned children to init.
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->parent == curproc){
      p->parent = initproc;
      if(p->state == ZOMBIE)
        wakeup1(initproc);
    }
  }

  // Jump into the scheduler, never to return.
  curproc->state = ZOMBIE;
  sched();
  panic("zombie exit");
}

// Wait for a child process to exit and return its pid.
// Return -1 if this process has no children.
int
wait(void)
{
  struct proc *p;
  int havekids, pid;
  struct proc *curproc = myproc();
  
  acquire(&ptable.lock);
  for(;;){
    // Scan through table looking for exited children.
    havekids = 0;
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->parent != curproc)
        continue;
      havekids = 1;
      if(p->state == ZOMBIE){
        // Found one.
        pid = p->pid;
        kfree(p->kstack);
        p->kstack = 0;
        freevm(p->pgdir);
        p->pid = 0;
        p->parent = 0;
        p->name[0] = 0;
        p->killed = 0;
        p->state = UNUSED;
        release(&ptable.lock);
        return pid;
      }
    }

    // No point waiting if we don't have any children.
    if(!havekids || curproc->killed){
      release(&ptable.lock);
      return -1;
    }

    // Wait for children to exit.  (See wakeup1 call in proc_exit.)
    sleep(curproc, &ptable.lock);  //DOC: wait-sleep
  }
}

//PAGEBREAK: 42
// Per-CPU process scheduler.
// Each CPU calls scheduler() after setting itself up.
// Scheduler never returns.  It loops, doing:
//  - choose a process to run
//  - swtch to start running that process
//  - eventually that process transfers control
//      via swtch back to the scheduler.
void
scheduler(void)
{
  struct proc *p;
  struct cpu *c = mycpu();
  c->proc = 0;
  
  for(;;){
    // Enable interrupts on this processor.
    sti();

    // Loop over process table looking for process to run.
    acquire(&ptable.lock);
    for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      if(p->state != RUNNABLE)
        continue;

      // Switch to chosen process.  It is the process's job
      // to release ptable.lock and then reacquire it
      // before jumping back to us.
      if(schedular_log_status ==1 ){
        if(p->in_kernal !=1){
          // struct *my_p = myproc();
          int c_id = p->p_container->id;
          int c_p_id = p->pid;
          cprintf("Container :) %d : schduling process :) %d \n",c_id, c_p_id);
        }
      }
      c->proc = p;
      switchuvm(p);
      p->state = RUNNING;

      swtch(&(c->scheduler), p->context);
      switchkvm();

      // Process is done running for now.
      // It should have changed its p->state before coming back.
      c->proc = 0;
    }
    release(&ptable.lock);

  }
}

// Enter scheduler.  Must hold only ptable.lock
// and have changed proc->state. Saves and restores
// intena because intena is a property of this
// kernel thread, not this CPU. It should
// be proc->intena and proc->ncli, but that would
// break in the few places where a lock is held but
// there's no process.
void
sched(void)
{
  int intena;
  struct proc *p = myproc();

  if(!holding(&ptable.lock))
    panic("sched ptable.lock");
  if(mycpu()->ncli != 1)
    panic("sched locks");
  if(p->state == RUNNING)
    panic("sched running");
  if(readeflags()&FL_IF)
    panic("sched interruptible");
  intena = mycpu()->intena;
  swtch(&p->context, mycpu()->scheduler);
  mycpu()->intena = intena;
}

// Give up the CPU for one scheduling round.
void
yield(void)
{
  acquire(&ptable.lock);  //DOC: yieldlock
  myproc()->state = RUNNABLE;
  sched();
  release(&ptable.lock);
}

// A fork child's very first scheduling by scheduler()
// will swtch here.  "Return" to user space.
void
forkret(void)
{
  static int first = 1;
  // Still holding ptable.lock from scheduler.
  release(&ptable.lock);

  if (first) {
    // Some initialization functions must be run in the context
    // of a regular process (e.g., they call sleep), and thus cannot
    // be run from main().
    first = 0;
    iinit(ROOTDEV);
    initlog(ROOTDEV);
  }

  // Return to "caller", actually trapret (see allocproc).
}

// Atomically release lock and sleep on chan.
// Reacquires lock when awakened.
void
sleep(void *chan, struct spinlock *lk)
{
  struct proc *p = myproc();
  
  if(p == 0)
    panic("sleep");

  if(lk == 0)
    panic("sleep without lk");

  // Must acquire ptable.lock in order to
  // change p->state and then call sched.
  // Once we hold ptable.lock, we can be
  // guaranteed that we won't miss any wakeup
  // (wakeup runs with ptable.lock locked),
  // so it's okay to release lk.
  if(lk != &ptable.lock){  //DOC: sleeplock0
    acquire(&ptable.lock);  //DOC: sleeplock1
    release(lk);
  }
  // Go to sleep.
  p->chan = chan;
  p->state = SLEEPING;

  sched();

  // Tidy up.
  p->chan = 0;

  // Reacquire original lock.
  if(lk != &ptable.lock){  //DOC: sleeplock2
    release(&ptable.lock);
    acquire(lk);
  }
}

//PAGEBREAK!
// Wake up all processes sleeping on chan.
// The ptable lock must be held.
static void
wakeup1(void *chan)
{
  struct proc *p;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++)
    if(p->state == SLEEPING && p->chan == chan)
      p->state = RUNNABLE;
}

// Wake up all processes sleeping on chan.
void
wakeup(void *chan)
{
  acquire(&ptable.lock);
  wakeup1(chan);
  release(&ptable.lock);
}

// Kill the process with the given pid.
// Process won't exit until it returns
// to user space (see trap in trap.c).
int
kill(int pid)
{
  struct proc *p;

  acquire(&ptable.lock);
  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->pid == pid){
      p->killed = 1;
      // Wake process from sleep if necessary.
      if(p->state == SLEEPING)
        p->state = RUNNABLE;
      release(&ptable.lock);
      return 0;
    }
  }
  release(&ptable.lock);
  return -1;
}

//PAGEBREAK: 36
// Print a process listing to console.  For debugging.
// Runs when user types ^P on console.
// No lock to avoid wedging a stuck machine further.
void
procdump(void)
{
  static char *states[] = {
  [UNUSED]    "unused",
  [EMBRYO]    "embryo",
  [SLEEPING]  "sleep ",
  [RUNNABLE]  "runble",
  [RUNNING]   "run   ",
  [ZOMBIE]    "zombie"
  };
  int i;
  struct proc *p;
  char *state;
  uint pc[10];

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
    if(p->state == UNUSED)
      continue;
    if(p->state >= 0 && p->state < NELEM(states) && states[p->state])
      state = states[p->state];
    else
      state = "???";
    cprintf("%d %s %s", p->pid, state, p->name);
    if(p->state == SLEEPING){
      getcallerpcs((uint*)p->context->ebp+2, pc);
      for(i=0; i<10 && pc[i] != 0; i++)
        cprintf(" %p", pc[i]);
    }
    cprintf("\n");
  }
}

















///// START HERE /////


void schedular_log(int enable){
  cprintf("current schedular_log_status : %d\n",schedular_log_status);
  if(enable == 1){
    schedular_log_status = 1;
  }
  else{
    schedular_log_status = 0;
  }
  cprintf("setting schedular_log_status to: %d\n",schedular_log_status);
}

int get_pid(){
  return myproc()->pid;
}

void ps(){
  struct proc *p;

  acquire(&ptable.lock);
  int in_k = myproc()->in_kernal;
  int my_cid = myproc()->p_container->id;

  for(p = ptable.proc; p < &ptable.proc[NPROC]; p++){
      // Wake process from sleep if necessary.
      if(p->state!=UNUSED && p->killed!=1){
        if(in_k == 1){
          if(p->in_kernal==1){
            cprintf("Kernal Process, pid : %d\n",p->pid);
          }
        }
        else{
          if(in_k!=1){
            // int c_index = get_container_index((p->p_container)->id);
            if(p->p_container->id == my_cid){
              cprintf("Container %d Process, pid : %d\n",(p->p_container)->id, p->pid);
            }
          }
        }
      }
    }
  release(&ptable.lock);
}
int give_cont(){
  struct proc *p= myproc();
  return p->p_container->id;
}

// char*
// fmtname(char *path)
// {
//   static char buf[DIRSIZ+1];
//   char *p;

//   // Find first character after last slash.
//   for(p=path+strlen(path); p >= path && *p != '/'; p--)
//     ;
//   p++;

//   // Return blank-padded name.
//   if(strlen(p) >= DIRSIZ)
//     return p;
//   memmove(buf, p, strlen(p));
//   memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
//   return buf;
// }

// void lsd(){
//   char* path = ".";
//   char buf[512], *p;
//   int fd;
//   struct dirent de;
//   struct stat st;

//   if((fd = open(path, 0)) < 0){
//     cprintf("ls: cannot open %s\n", path);
//     return;
//   }

//   if(fstat(fd, &st) < 0){
//     cprintf("ls: cannot stat %s\n", path);
//     close(fd);
//     return;
//   }

//   switch(st.type){
//   case T_FILE:
//     cprintf("%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
//     break;

//   case T_DIR:
//     if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
//       cprintf("ls: path too long\n");
//       break;
//     }
//     strcpy(buf, path);
//     p = buf+strlen(buf);
//     *p++ = '/';
//     while(read(fd, &de, sizeof(de)) == sizeof(de)){
//       if(de.inum == 0)
//         continue;
//       memmove(p, de.name, DIRSIZ);
//       p[DIRSIZ] = 0;
//       if(stat(buf, &st) < 0){
//         cprintf("ls: cannot stat %s\n", buf);
//         continue;
//       }
//       cprintf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
//     }
//     break;
//   }
//   close(fd);
// }



#define MAX_CONTAINERS 10
struct container containers[MAX_CONTAINERS];

void container_init(){
  int i;
  for(i=0;i<MAX_CONTAINERS;i++){
    // strcpy(containers[i].name,"");
    containers[i].max_proc = 5;
    containers[i].current_proc = -1; // if container is not in use current_proc will be 0
    containers[i].total_proc = 0;
    containers[i].in_use = -1;
    containers[i].id=-1;
  }
}

int get_current_proc(int cont_num){
  return containers[cont_num].current_proc;
}

int get_max_proc(int cont_num){
  return containers[cont_num].max_proc;
}

int get_total_proc(int cont_num){
  return containers[cont_num].total_proc;
}

int get_container_index(int id){
  for(int i=0;i<MAX_CONTAINERS;i++){
    if(containers[i].id == id){
      return i;
    }
  }
  return -1;
}

void set_proc_container(int id){
  int c_index = get_container_index(id);
  myproc()->p_container = &containers[c_index];
}

int alloc_container(int id){
  int cont_id = id;
  for(int i=0;i<MAX_CONTAINERS;i++){
    if(containers[i].in_use==0){
      containers[i].in_use=1;
      containers[i].id = cont_id;
      return i;
    }
  }
  return -1;
}

void reset_container(int id){
  int i = get_container_index(id);
  // int i = c_id;
  containers[i].current_proc = -1; // if container is not in use current_proc will be 0
  containers[i].total_proc = 0; //total number of processes in container
  containers[i].in_use = 0;     // 0-> container not in use, 1-> container in use
  containers[i].id=-1;          
}

int create_container(int id){
    int cont_id = alloc_container(id);
    // char *a[1];
    // int ad = id;
    // strncpy(a,(char *)ad,1);
    // mkdir("0");
    return cont_id; 
}

int join_container(int id){
    struct proc *p = myproc();
    int c_ind = get_container_index(id);
    p->p_container = &containers[c_ind]; // adding the container to process's container pointer
    containers[c_ind].processes[containers[c_ind].total_proc] = p->pid; // adding process's pid to containers array of pids
    containers[c_ind].total_proc +=1;
    p->in_kernal = 0;
    return 0;
}

int leave_container(int id){
    struct proc *p = myproc();
    int c_ind = get_container_index(id);
    containers[c_ind].total_proc -= 1;
    p->in_kernal=1;
    kill(p->pid);
    return 0;
}

int destroy_container(int id){
  int c_index = get_container_index(id);
  for(int i=0;i<containers[c_index].total_proc; i++){
    kill(containers[c_index].processes[i]);
  }
  reset_container(id);
  return 0;
}

