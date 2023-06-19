#include "types.h"
#include "stat.h"
#include "user.h"

#define N 3000		//global array size - change to see effect. Try 3000, 5000, 10000
int glob[N];

int main(){
    printf(1,"Printing initial page table:\n");
    pgtprint();
    if(fork()==0){
        printf(1,"Inside child process\n\n");
        printf(1,"Initial child page table:\n");
        pgtprint();
        glob[1690]=69;
        printf(1,"Printing final child page table:\n");
        pgtprint();
        exit();
    } else {
        wait();
        printf(1,"Printing final parent page table:\n");
        pgtprint();
        printf(1,"Value: %d\n", glob[1690]);
        exit();
    }
}