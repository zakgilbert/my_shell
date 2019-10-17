#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "Cmd.h"
#include "She_l.h"

int main(int argc, char** argv)
{
    char* user_name = read_command("whoami");
    She_l* shell    = CREATE_SHE_L();
    Cmd* temp_command;
    int running = 1;

    while (running) {
        temp_command = CREATE_CMD(user_name);
        temp_command = shell->get_user_input(shell, temp_command);
        shell->run(shell);
        running = strcmp(temp_command->argv[0], "exit");
        temp_command->destroy(temp_command);
    }

    shell->destroy(shell);
    return 0;
}
