extern char* gharbaar;

struct container {
    int max_proc;
    int total_proc;
    int current_proc;
    int in_use;
    int id;

    int processes[10];  //to store pids of processes in container, 
                        //these processes will be killed on contained_destroy
    
    // char name[32];

};