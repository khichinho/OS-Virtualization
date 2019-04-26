#include "syscall.h"
#include "traps.h"

#define SYSCALL(name) \
  .globl name; \
  name: \
    movl $SYS_ ## name, %eax; \
    int $T_SYSCALL; \
    ret

SYSCALL(fork)
SYSCALL(exit)
SYSCALL(wait)
SYSCALL(pipe)
SYSCALL(read)
SYSCALL(write)
SYSCALL(close)
SYSCALL(kill)
SYSCALL(exec)
SYSCALL(open)
SYSCALL(mknod)
SYSCALL(unlink)
SYSCALL(fstat)
SYSCALL(link)
SYSCALL(mkdir)
SYSCALL(chdir)
SYSCALL(dup)
SYSCALL(getpid)
SYSCALL(sbrk)
SYSCALL(sleep)
SYSCALL(uptime)

///// START HERE /////
SYSCALL(schedular_log)
SYSCALL(get_pid)
SYSCALL(ps)
SYSCALL(give_cont)
SYSCALL(get_current_proc)  
SYSCALL(get_max_proc)       
SYSCALL(get_total_proc)      
SYSCALL(container_init)      
SYSCALL(set_proc_container)  
SYSCALL(create_container)    
SYSCALL(join_container)
SYSCALL(leave_container)
SYSCALL(destroy_container)
SYSCALL(get_container_index)
