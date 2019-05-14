#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include <openssl/md5.h>
#include "md5.h"

int main() 
{ 
    //create the two pipes
    int fd1[2];   
    int fd2[2];   
    //the input string from the user up to 20 chars
    char inputString[20]; 
    pid_t p; 
    // in case that the pipe operation was Failed
    if(pipe(fd1)==-1 || pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe() Failed" ); 
        return 1; 
    } 
    
    //create the child process
    p=fork(); 
    
    //in case of fork opration Failed
    if(p<0) 
    { 
        fprintf(stderr, "fork() Failed" ); 
        return 1; 
    } 
  
    //parent process case  
    else if(p>0) 
    { 
        //get the input string from user
        scanf("%s", inputString); 
        char final_ans[20]; 
 
        close(fd1[0]);
  
        // Write input string and close writing end of first pipe
        write(fd1[1], inputString, strlen(inputString)+1); 
        close(fd1[1]); 
  
        wait(NULL); 
  
        // Close writing end of second pipe 
        close(fd2[1]); 
  
        // Read string from child, print it and close 
        read(fd2[0], final_ans, 20); 
        printf("the key is : %s",final_ans);

        close(fd2[0]); 
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  
  
        // Read a string using first pipe 
        char final_ans[20]; 
        read(fd1[0], final_ans, 20); 
        int k = strlen(final_ans); 
        int i; 
        final_ans[k] = '\0';   
 
        close(fd1[0]); 
        close(fd2[0]); 
        
        
        char ans[20];
        sprintf(ans, "%d", md5(inputString));

        write(fd2[1],ans , strlen(ans)+1); 
        close(fd2[1]); 
        
        
        exit(0); 
    } 
} 