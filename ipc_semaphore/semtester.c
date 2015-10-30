#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <errno.h>

#define SEMPATH "/dev"

static struct sembuf sem_lock   = { 0, -1, 0 };
static struct sembuf sem_unlock = { 0,  1, 0 };

//static struct sembuf sem_try_lock   = { 0, -1, IPC_NOWAIT };
//static struct sembuf sem_try_unlock = { 0,  1, IPC_NOWAIT };

int main(void)
{
    int semid;
    key_t key;
    char c;
    int cycle = 1;

    key = ftok(SEMPATH, 's');

	if(-1 == key)
	{
		perror("ftok() failed\n");
		return -1;
	}

	semid = semget(key, 1, IPC_CREAT);
	if(-1 == semid)
	{
		perror("semget() failed\n");
		return -1;
	}

	while(cycle)
	{
		printf("> ");
		scanf("%c", &c);

		switch(c)
		{
			case 'q':
				cycle = 0;
				break;

			case 'l':
				printf("Blocking lock...\n");
				semop(semid, &sem_lock, 1);
				printf("Sem locked\n");
				break;

			case 'u':
				printf("Blocking unlock...\n");
				semop(semid, &sem_unlock, 1);
				printf("Sem unlocked\n");
				break;

			default:
				printf("\n"
					   "q - exit\n"
					   "l - blocking lock\n"
					   "u - blocking unlock\n\n");
				break;
		}
	}

	return 0;
}
