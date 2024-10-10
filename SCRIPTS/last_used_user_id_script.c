#include <sys/file.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h>
#include <sys/stat.h>
#include "../HEADERFILES/last_used_user_id.h"


int main(int argc, char const *argv[])
{
    printf("Number of accounts = %d\n", show_user_id_by_one());
    return 0;
}
