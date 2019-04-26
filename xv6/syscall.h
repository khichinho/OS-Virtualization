// System call numbers
#define SYS_fork    1
#define SYS_exit    2
#define SYS_wait    3
#define SYS_pipe    4
#define SYS_read    5
#define SYS_kill    6
#define SYS_exec    7
#define SYS_fstat   8
#define SYS_chdir   9
#define SYS_dup    10
#define SYS_getpid 11
#define SYS_sbrk   12
#define SYS_sleep  13
#define SYS_uptime 14
#define SYS_open   15
#define SYS_write  16
#define SYS_mknod  17
#define SYS_unlink 18
#define SYS_link   19
#define SYS_mkdir  20
#define SYS_close  21

///// START HERE /////

#define SYS_get_current_proc        22
#define SYS_get_max_proc            23
#define SYS_get_total_proc          24
#define SYS_container_init          25
#define SYS_set_proc_container      26
#define SYS_create_container        27
#define SYS_join_container          28
#define SYS_leave_container         29
#define SYS_destroy_container       30
#define SYS_get_container_index     31
#define SYS_ps      32
#define SYS_give_cont               33
#define SYS_schedular_log           34
#define SYS_get_pid 35