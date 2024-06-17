#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int IsPassOk(void);

int main(void)
{
    int PwStatus;

    puts("Enter password:");

    PwStatus = IsPassOk();

    if (PwStatus == 0) {
        printf("Bad password!\n");
        exit(1);
    } else {
        printf("Access granted!\n"); // Строка для которой нужно
                                     // выяснить адрес
    }

    return 0;
}

int IsPassOk(void)
{
	char Pass[13];
	fgets(Pass, 64, stdin);
	return 0 == strcmp(Pass, "test");
}

//set disassembly-flavor intel
//-fno-stack-protector -no-pie
// ./a.out < file_in
