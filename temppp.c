// /*
//             add a new user
//         */

//         // recv signal (ready from client)
//         if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
//             printf("Error\n");
//         }
//         printf("%s\n", buffer);
//         printf("recvd of ready sig \n============================================\n");
//         // getchar();

//         // send type signal once
//         strcpy(buffer, "1"); // type 1
//         if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
//             perror("Error sending login data");
//         }
//         printf("%s\n", buffer);
//         printf("sent TYPE sig \n============================================\n");
//         // getchar();


//         // send menu
//         strcpy(buffer, "==========================================\nType of user you want to add\nSelect Your Option :\n1. Admin\n2. Managers\n3. Employees\n4. Customers\nEnter your choice :");
//         if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
//             perror("Error sending login data");
//         }
//         // strcpy(buffer, "==========================================\nType of user you want to add\nSelect Your Option :\n1. Admin\n2. Managers\n3. Employees\n4. Customers\nEnter your choice :");
//         if (send(acpt, buffer, strlen(buffer)+1, 0) == -1) {
//             perror("Error sending login data");
//         }
//         printf("%s\n", buffer);
//         printf("sent menu \n============================================\n");
//         // getchar();

//         // recv choice once
//         if(recv(acpt, &buffer, sizeof(buffer), 0)==-1){
//             printf("Error\n");
//         }
//         printf("%s\n", buffer);
//         printf("recvd choice \n============================================\n");
//         // getchar();