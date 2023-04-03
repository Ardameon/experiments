#include <syslog.h>
#include <stdio.h>

#define IDENTITY_STR "syslog_test_app"

void print_prompt(void);
void print_help(void);

int main(void)
{
    char cmd[64];
    int option = LOG_CONS | LOG_NDELAY | LOG_PID;
//    int priority = LOG_INFO;
    int priority = LOG_WARNING;
//    int facility = LOG_USER;
    int facility = LOG_DAEMON;

    openlog(IDENTITY_STR, option, facility);

    print_prompt();

    while(fgets(cmd, sizeof(cmd), stdin))
    {
        switch (*cmd)
        {
            case 'q':
                goto _exit;
                break;

            case 'h':
                print_help();
                break;

            default:
                syslog(priority, "%.63s", cmd);
                break;
        }

        print_prompt();
    }

_exit:
    closelog();

    return 0;

}

void print_prompt(void)
{
    printf("enter command > ");
}

void print_help(void)
{
    printf(
            "q - quit\n"
            );
}
