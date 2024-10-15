#include "../HEADERFILES/create_new_user.h"

void main(int argc, char *argv[]){
    int type;
    if(argc>1){
        char *type = argv[1];
    }
    else{
        printf("Enter Type of User You want to add :\n1. Admin\n2. Manager\n3. Employee\n4. Customer\n");
        scanf("%d", &type);
    }
    // create_new_user(acpt, type);
    return;
}

// gcc create_new_user.c -o create_new_user -lssl -lcrypto
// gcc create_new_user_script.c -lssl -lcrypto