#include <sys/api.h>
#include <assert.h>

uint16_t OS_new_segment(int arg1, int arg2, int arg3) {
    uint16_t return_val;
    uint32_t discard;
    asm volatile (  "movl $0x06, %%eax\n\t"
                    "int $0x80\n\t"
                     : "=a" (return_val), "=d"(discard)
                     : "c" (arg1),
                       "d" (arg2),
                       "D" (arg3)
                     :  );
    return return_val;  
}

uint32_t OS_get_heap_base(void) {
    uint32_t val;
    asm volatile (  "movl $0x10, %%eax\n\t"
                    "int $0x80\n\t"
                     : "=a" (val)
                     :
                     : "edx" );
    return val;    
}

uint32_t OS_get_heap_size(void) {
    uint32_t val;
    asm volatile (  "movl $0x11, %%eax\n\t"
                    "int $0x80\n\t"
                     : "=a" (val)
                     :
                     : "edx" );
    return val;       
}
int OS_resize_heap(size_t size) {
    int return_val;
    asm volatile (  "movl $0x12, %%eax\n\t"
                    "int $0x80\n\t"
                     : "=a" (return_val)
                     : "c" (size)
                     : "edx" );
    return return_val;
}

int OS_vdev_register(uint8_t *in, uint8_t *out, int *flag_in, int *flag_out) {
    int ret; 
    asm volatile (  "movl $0x20, %%eax\n\t"
                    "int $0x80\n\t"
                     : "=a" (ret)
                     : "c" (in),
                       "d" (flag_in),
                       "D" (out),
                       "S" (flag_out)
                     :  );
    return ret;
}

int OS_vdev_in_ready(void *value) {
    int ret;
    asm volatile (  "movl $0x21, %%eax\n\t"
                    "int $0x80\n\t"
                     : "=a" (ret)
                     : "c" (value)
                     : "edx" );
    return ret;
}

int OS_vdev_out_ready(void *value) {
    int ret;
    asm volatile (  "movl $0x22, %%eax\n\t"
                    "int $0x80\n\t"
                     : "=a" (ret)
                     : "c" (value)
                     : "edx" );
    return ret;
}

int OS_vdev_await(void) {
    int ret;
    asm volatile (  "movl $0x23, %%eax\n\t"
                    "int $0x80\n\t" 
                     : "=a" (ret) 
                     :   
                     : "edx" ); 
    return ret;
}

int OS_read(int fd, void *buffer, size_t len) {
    int return_val, discard;                           
    asm volatile (  "int $0x80\n\t"        
                     : "=a" (return_val), "=d"(discard)   
                     : "a"(0x2c), "c" (fd), 
                       "d" (buffer),
                       "D" (len)
                     : "memory");        
    return return_val;   
}

int OS_close(int fd) {
    int return_val;                           
    asm volatile (  "movl $0x2b, %%eax\n\t"   
                    "int $0x80\n\t"        
                     : "=a" (return_val)        
                     : "c" (fd)
                     : "edx" );        
    return return_val;    
}

int OS_open(const char *path, int flags, mode_t mode) {
    int return_val;                           
    asm volatile (  "movl $0x2a, %%eax\n\t"   
                    "int $0x80\n\t"        
                     : "=a" (return_val)        
                     : "c" (path), 
                       "d" (flags),
                       "D" (mode)
                     :  );        
    return return_val;                               
}

int OS_write(int fd, const void *buffer, size_t len) {
    int return_val, discard;                        
    asm volatile (  "int $0x80\n\t"        
                     : "=a" (return_val), "=d"(discard)        
                     : "a"(0x2d), "c" (fd), 
                       "d" (buffer),
                       "D" (len)
                     : "memory");        
    return return_val;           
}

int OS_vfs_mkdir(const char *path, int perms) {
    int ret;
    asm volatile (  "movl $0x35, %%eax\n\t"
                    "int $0x80\n\t"
                     : "=a" (ret)
                     : "c" (path),
                       "d" (perms)
                     :  );
    return ret;
}

int OS_vfs_create(const char *path, int perms) {
    int ret;
    asm volatile (  "movl $0x36, %%eax\n\t"   
                    "int $0x80\n\t"        
                     : "=a" (ret)                
                     : "c" (path),       
                       "d" (perms)   
                     :  );        
    return ret;
}

int OS_vfs_remove(const char *path) {
    int return_val;                          
    asm volatile (  "movl $0x34, %%eax\n\t"  
                    "int $0x80\n\t"       
                     : "=a" (return_val)    
                     : "c" (path)
                     : "edx" );       
    return return_val;      
}

int OS_vfs_list(const char *path, vfs_metadata_t *metadata, size_t entry) {
    int return_val;                           
    asm volatile (  "movl $0x32, %%eax\n\t"   
                    "int $0x80\n\t"        
                     : "=a" (return_val)        
                     : "c" (path), 
                       "d" (metadata),
                       "D" (entry)
                     :  );        
    return return_val;       
}

int OS_vfs_write(const char *path, size_t loc, const void *value) {
    int return_val;                           
    uint32_t loc_low = loc & 0x00000000ffffffff;
    uint32_t loc_high = loc / 0x100000000;
    asm volatile (  "movl $0x31, %%eax\n\t"   
                    "int $0x80\n\t"        
                     : "=a" (return_val)        
                     : "c" (path), 
                       "d" (loc_low),
                       "D" (loc_high),
                       "S" (value)
                     :  );        
    return return_val;    
}

int OS_vfs_read(const char *path, size_t loc) {
    int return_val;                           
    uint32_t loc_low = loc & 0x00000000ffffffff;
    uint32_t loc_high = loc / 0x100000000;
    asm volatile (  "movl $0x30, %%eax\n\t"   
                    "int $0x80\n\t"        
                     : "=a" (return_val)        
                     : "c" (path), 
                       "d" (loc_low),
                       "D" (loc_high)
                     :  );        
    return return_val;    
}

int OS_vfs_cd(const char *path) {
    int return_val;                           
    asm volatile (  "movl $0x2f, %%eax\n\t"   
                    "int $0x80\n\t"        
                     : "=a" (return_val)     
                     : "c" (path) 
                     : "edx" );        
    return return_val; 
}

int OS_vfs_get_metadata(const char *path, vfs_metadata_t *metadata, int type) {
    int return_val;                           
    asm volatile (  "movl $0x33, %%eax\n\t"   
                    "int $0x80\n\t"        
                     : "=a" (return_val)        
                     : "c" (path), 
                       "d" (metadata),
                       "D" (type)
                     :  );        
    return return_val;                               
}

void OS_pwd(const char *value) {
    asm volatile (  "movl $0x1a, %%eax\n\t"   
                    "int $0x80\n\t"        
                     :                 
                     : "c" (value)     
                     : "eax", "edx" ); 
}

int OS_general_execute_block(task_info_t *taskinfo) {
    uint32_t ret_low;
    uint32_t ret_hi;
    uint64_t ret;
    asm volatile (  "movl $0x02, %%eax\n\t"   
                    "int $0x80\n\t"        
                     : "=a" (ret_low),                
                       "=d" (ret_hi)
                     : "c" (taskinfo)     
                     :  ); 
    ret = ((uint64_t)(ret_hi) << 32) + (uint64_t)ret_low;
    return ret;
}

int OS_general_execute(task_info_t *taskinfo) {
    int ret;
    asm volatile (  "movl $0x01, %%eax\n\t"   
                    "int $0x80\n\t"        
                     : "=a" (ret)
                     : "c" (taskinfo)     
                     : "edx" ); 
    return ret;
}

void OS_what_stdin(const char *path) {
    asm volatile (  "movl $0x1b, %%eax\n\t"   
                    "int $0x80\n\t"        
                     :                 
                     : "c" (path)     
                     : "eax", "edx" ); 
}

void OS_what_stdout(const char *path) {
    asm volatile (  "movl $0x1c, %%eax\n\t"   
                    "int $0x80\n\t"        
                     :                 
                     : "c" (path)     
                     : "eax", "edx" ); 
}

void OS_what_stderr(const char *path) {
    asm volatile (  "movl $0x1d, %%eax\n\t"   
                    "int $0x80\n\t"        
                     :                 
                     : "c" (path)     
                     : "eax", "edx" ); 
}

pid_t OS_ipc_resolve_name(const char *server) {
    pid_t pid;                          
    asm volatile (  "movl $0x0a, %%eax\n\t"  
                    "int $0x80\n\t"       
                     : "=a" (pid)       
                     : "c" (server)
                     : "edx" );       
    return pid;                              
}

void OS_ipc_send_packet(pid_t pid, void *payload, size_t len) {
    asm volatile (  "movl $0x08, %%eax\n\t"
                    "int $0x80\n\t"     
                     :              
                     : "c" (pid),    
                       "d" (payload),
                       "D" (len)  
                     : "eax" );     
}

pid_t OS_getpid(void) {
    pid_t val;                        
    asm volatile (  "movl $0x15, %%eax\n\t"   
                    "int $0x80\n\t"        
                     : "=a" (val)     
                     :
                     : "edx" );        
    return val;                              
}
