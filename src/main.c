#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "Cmd.h"
#include "She_l.h"
#include "Cmd_Node.h"
#include "Hash_t.h"

int main(int argc, char** argv)
{
    int status;
    pid_t id;
    char* user_name = read_command("whoami");
    She_l* shell    = CREATE_SHE_L();
    Hash_t* tab     = CREATE_HASH_T(50000`);
    Cmd* temp_command;
    tab->load_cmds(tab, "/bin/");
    tab->load_cmds(tab, "/usr/bin/");

    while (1) {
        temp_command = CREATE_CMD(user_name, tab);
        temp_command = shell->get_user_input(shell, temp_command);
        shell->run(shell);
        temp_command->destroy(temp_command);
    }

    return 0;
}
