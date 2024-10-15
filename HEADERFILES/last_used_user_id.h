void update_user_id_by_one(){
    int fd;
    struct stat file_stat;

    // open file in read write file
    fd = open("DATABASE/last_used_user_id.txt",O_RDWR);
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

int show_user_id_by_one(){
    int fd;
    int t=0;
    struct stat file_stat;

    // open file in read write file
    fd = open("DATABASE/last_used_user_id.txt",O_RDWR);
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
