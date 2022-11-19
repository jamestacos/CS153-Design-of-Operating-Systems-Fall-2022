#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void) // part a: terminate current process / store exit status in corresponding structure
{
  int status; // initialize status variable -> assigning the int to the address of status
  argint(0, &status); // argint is passing values into the trap frame - by means of any offset that varies
  exit(status); // retrieve the value from status address and using it as an argument in exit function
  return 0;  // not reached
}

int
sys_wait(void)
{
    int *status; // initialize status pointer
    if (argptr(0, (void*)&status, sizeof(status)) < 0) // argptr is used to pass a pointer to a struct to a syscall
        return -1; // fetches the pointer and dereferences the value at status comparing it to len 0
    return wait(status);    // return value of status into wait function for wait time
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

int
sys_hello(void){
    hello();
    return 0;
}

int
sys_waitpid(void) { // part c: created syscall method for waitpid
    int pid;
    int *status;
    int options;

    argint(0, &pid); // argint is used to pass in an integer state to the address of pid
    argint(2, &options); // argint for passing in an integer argument when declaring/using options variable

    if (argptr(1, (void*)&status, sizeof(status)) < 0) { // fetches value at status, checking if size is less than len 0
        return -1;
    }

    return waitpid(pid, status, options); // return all 3 arguments once it has been defined and checked for conditions
}
// helper function arg int parses the user stack for kernel to fetch -> takes it from the stack -> puts in the syscall

// wait -> wait until any child branch process is terminated before any current process continues
// waitpid -> similar to wait, difference is process still runs up until a PID matches argument

// if ptr for status is invalid, return -1

// no direct data flow ->> through user stack

// wait -> wait for any child process to finish then it stops waiting
// waitpid -> doesnt have to be a child, just as long as the specified pid is met/match
// specify address where the kernel can read from
// parents just waits until child finishes and matches the pid value

int
sys_setpriority(void){
    int prior_val;
    argint(0, &prior_val);
    return setpriority(prior_val);
}