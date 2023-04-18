 #include<stdio.h>
 #include <sys/mman.h>

 #pragma pack(push,2)
 struct counter
 {
     char buf[62];
     long long c;
 };
 #pragma pack(pop)

 int main () {
     struct counter *p;
     int size = sizeof(struct counter);
     int prot = PROT_READ | PROT_WRITE;
     int flags = MAP_PRIVATE | MAP_ANONYMOUS;

     p = (struct counter *) mmap(0, size, prot, flags, -1, 0);

     while(1) {
         __sync_fetch_and_add(&p->c, 1);
     }

     return 0;
 }

