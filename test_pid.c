#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>

int main(){
    printf("Enter pid to find: ");
    int pid;
    scanf("%d", &pid);
    const int MAX_LEN = 5;
    char name[MAX_LEN];
    int res = syscall(351, pid, name, MAX_LEN);
    if (res == 0) printf("System call returned %s\n", name);
    else if (res == -1) printf("Error or Not found!\n");
    else if (res > 0) printf("True length: %d\n", res);
    return 0;
}
