int first()
{
	struct acc_no
	{
		long long int account_number;
	}a;
	struct customer{
		char name[1024];
		char username[1024];
		char password[1024];
		char mail[1024];
		long long int account_number;
		double balance;
		char type;
	}add;
	printf("\n\n =============Welcome to Lena Dena Bank============= \n");
	printf("\nFirst account should be Admin: ");
	printf("\nEnter name: ");
	scanf("%s",add.name);
	printf("Enter username: ");
	scanf("%s",add.username);
	printf("Enter password: ");
	scanf("%s",add.password);
	printf("Enter Mail");
	scanf("%s",add.mail);
	printf("Thank you, Admin account created, now admin can add Clients\n\n");
	add.type='a';
	add.balance=0.0;
	
	//account no
	int fd2=open("acc_no",O_CREAT|O_RDWR,0744);
	read(fd2,&a,sizeof(a));
	add.account_number=++a.account_number;
	lseek(fd2,0,SEEK_SET);
	write(fd2,&a,sizeof(a));
	
	
	
	int fd=open("customer",O_CREAT|O_RDWR,0744);
	lseek(fd,0,SEEK_END);
	write(fd,&add,sizeof(add));
	printf("Entry has been added into database.\n");
	

	return 0;
	
}
