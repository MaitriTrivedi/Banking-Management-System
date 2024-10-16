#include <sys/socket.h>
#include <string.h>
#include "../HEADERFILES/structures.h"
#include<fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include "../HEADERFILES/password_methods.h"

void create_admin_user_own(struct Admin a){
    int fd;

    // open file in read write file
    fd = open("../DATABASE/admin.txt",O_RDWR);
    if(fd==-1){
        perror("create_admin_user : ");
        return;
    }
    // write data to the file
    write(fd, &a, sizeof(a));
    lseek(fd, 0, SEEK_SET);
    read(fd, &a, sizeof(a));

    close(fd); 

    return ;
}

void update_user_id_by_one_own(){
    int fd;
    struct stat file_stat;

    // open file in read write file
    fd = open("../DATABASE/last_used_user_id.txt",O_RDWR);
    if(fd==-1){
        perror("update_user_id_by_one : ");
        return;
    }

    // check if the file is empty
    fstat(fd, &file_stat);
    if(file_stat.st_size == 0){
        // file is empty, initialize with 0
        int init_val = 0;
        write(fd, &init_val, sizeof(init_val));
        lseek(fd, 0, SEEK_SET);  // reset file pointer to start
    }
    else{
        int t;
        // write data to the file
        lseek(fd, 0, SEEK_SET);
        read(fd, &t, sizeof(t));
        printf("Last Used User Id : %d\n", t);
        t = t+1;
        
        lseek(fd, 0, SEEK_SET);
        write(fd, &t, sizeof(t));

        lseek(fd, 0, SEEK_SET);
        read(fd, &t, sizeof(t));
        printf("Updated User Id : %d\n", t);
    }

    close(fd); 
    return;
}

int show_user_id_by_one_own(){
    int fd;
    int t=0;
    struct stat file_stat;

    // open file in read write file
    fd = open("../DATABASE/last_used_user_id.txt",O_RDWR);
    if(fd==-1){
        perror("show_user_id_by_one : ");
        return t;
    }

    // check if the file is empty
    fstat(fd, &file_stat);
    if(file_stat.st_size == 0){
        // file is empty, initialize with 0
        int init_val = 0;
        printf("Last Used User Id : %d\n", t);
    }
    else{
        
        lseek(fd, 0, SEEK_SET);
        read(fd, &t, sizeof(t));
        printf("Last Used User Id : %d\n", t);
    }

    close(fd); 
    return t;
}

void main(int argc, char *argv[]){
    int type;
    if(argc>1){
        char *type = argv[1];
    }
    else{
        printf("Enter Type of User You want to add :\n1. Admin\n2. Manager\n3. Employee\n4. Customer\n");
        scanf("%d", &type);
    }
    struct Admin a;
    strcpy(a.u.username, "Maitri");
    hashPassword( "1234", a.u.password);
    a.u.userid = show_user_id_by_one_own()+1;
    create_admin_user_own(a);
    update_user_id_by_one_own();
    return;
}

// gcc create_new_user.c -o create_new_user -lssl -lcrypto
// gcc create_new_user_script.c -lssl -lcrypto