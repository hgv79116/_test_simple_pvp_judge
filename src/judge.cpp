#include<stdio.h> 
#include <stdlib.h> 
#include<unistd.h> 
#include<string.h>
#include<errno.h>
#include <sys/wait.h>

#define MAXNAME 100
#define MAXMESSAGE 100
#define REND 0
#define WEND 1

int main(int argc, char **argv, char **env) { 
    int player_num = argc - 1; 
    if (player_num >= 3) { 
        printf("not yet implemented"); 
        return 1; 
    }

    char player_names[player_num][MAXNAME]; 
    for(int i = 0; i < player_num; i++) { 
        strcpy(player_names[i], argv[i + 1]); 
    }

    int fildes[player_num][2][2]; 
    int fildes_cnt = 2; 
    for(int i = 0; i < player_num; i++) { 
        for(int j = 0; j < 2; j++) { 
            for(int k = 0; k < 2; k++) { 
                fildes[i][j][k] = fildes_cnt++; 
            }
            pipe(fildes[i][j]); 
            printf(" pipe from %d to %d \n", fildes[i][j][WEND], fildes[i][j][REND]); 
        }
    }

    for(int i = 0; i < player_num; i++) { 
        int id = fork(); 
        if(id == 0) { 
            int in_fildes = fildes[i][0][REND]; 
            int out_fildes = fildes[i][1][WEND]; 

            printf(" %s < %d > %d \n", player_names[i], in_fildes, out_fildes); 

            // fprintf(stderr, " dup results: %d\n", dup2(in_fildes, STDIN_FILENO)); 
            fprintf(stderr, " dup results of %s: %d\n", player_names[i], dup2(out_fildes, STDOUT_FILENO));  
            // close(in_fildes); 
            // close(out_fildes); 
            // close(fildes[i][0][WEND]); 
            // close(fildes[i][1][REND]); 

            execl(
                player_names[i], 
                player_names[i], 
                NULL
            ); 
            printf("launched %s, errno: %d\n", player_names[i], errno); 
        }
    }

    FILE* inp[player_num]; 
    FILE* out[player_num]; 

    for(int i = 0; i < player_num; i++) { 
        // out[i] = fdopen(fildes[i][0][WEND], "w"); 
        inp[i] = fdopen(fildes[i][1][REND], "r"); 
        // close(fildes[i][0][REND]); 
        // close(fildes[i][1][WEND]); 
        printf(" judge > %d < %d \n", fildes[i][0][WEND], fildes[i][1][REND]); 
    }

    int x, y; 
    fscanf(inp[0], "%d %d", &x, &y); 
    printf("%d %d\n", x, y); 

    wait(NULL); 
    printf("judge stop\n"); 
}