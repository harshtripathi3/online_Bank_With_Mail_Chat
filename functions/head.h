#ifndef FOO_DOT_H    /* This is an "include guard" */
#define FOO_DOT_H    /* prevents the file from being included twice. */
                     /* Including a header file twice causes all kinds */
                     /* of interesting problems.*/

/**
 * This is a function declaration.
 * It tells the compiler that the function exists somewhere.
 */
 
 //================================================================================================================

int gfd;
struct flock lck,lck2;
struct password_change
{
	char oldpassword[1024];
	char newpassword[1024];
};
struct passbook
{
	char data[5000];
	

};
struct acc_no
	{
		long long int account_number;
	};
struct customer{
		char name[1024];
		char username[1024];
		char password[1024];
		char mail[1024];
		long long int account_number;
		double balance;
		char type;
	};
struct modify{
		char oldusername[1024];
		char newusername[1024];
		char password[1024];
		char type;
		int update;
	};
struct customer login_time_cp;
struct customer user;

//================================================================================================================

 
 
 
 
struct customer search(struct customer ser);
void lockjoint(struct customer joint);
char login(struct customer request,int login);
struct customer delete(struct customer del);
char add(struct customer add_req);
void addafterop(struct customer add_req);
struct modify modification(struct modify m);
void writejoint2(struct customer joint);
void updatepassbook(struct customer data);
struct customer bank(struct customer data,int op,struct password_change pass);
char jointadd(struct customer new,struct customer newj);
struct customer cmodification(struct customer data,int sd);



void lockjoint(struct customer joint)
{
	int size;
	lseek(gfd,0,SEEK_SET);
	struct customer r;
	char res='N';
	while((size=read(gfd,&r,sizeof(r)))!=0)
	{
		if(joint.account_number==r.account_number&&strcmp(joint.username,r.username)!=0)
		{
			write(0,"joint2\n",6);
			lseek(gfd,-1*sizeof(r),SEEK_CUR);
					
					
					lck2.l_type=F_WRLCK;
					lck2.l_whence=SEEK_CUR;
					lck2.l_start=0;
					lck2.l_len=sizeof(r);
					lck2.l_pid=getpid();
					fcntl(gfd,F_SETLK,&lck2);
					break;
		}
	}	

}
char login(struct customer request,int login)
{
	
	gfd=open("customer",O_RDWR,0744);
	if(login==1)
	{
		int size;
		lseek(gfd,0,SEEK_SET);
		struct customer r;
		char ans='I';
		int i;
		while((size=read(gfd,&r,sizeof(r)))!=0)
		{
			if(strcmp(request.username,r.username)==0)
			{
				if(strcmp(request.password,r.password)==0)
				{
					
					login_time_cp=r;//incase of normal user we need username so stored it in (login time copy)
					//record locking
					lseek(gfd,-1*sizeof(r),SEEK_CUR);
					
					
					lck.l_type=F_WRLCK;
					lck.l_whence=SEEK_CUR;
					lck.l_start=0;
					lck.l_len=sizeof(r);
					lck.l_pid=getpid();
					//before critical section
					write(0,"before\n",8);
					i=fcntl(gfd,F_SETLK,&lck);
					if(i==-1)
					{
						write(0,"locked\n",8);
						ans='L';
						perror("error:");
						break;
					}
					if(r.type=='j')
					{
						write(0,"rtype\n",6);
						lockjoint(r);	
					}
					
					//inside critical section
					write(0,"Inside\n",8);
					//getchar();
					ans=r.type;
					break;
				}
			}
		}
		//close(fd);
		return ans;
	}
	else
	{
		fcntl(gfd,F_UNLCK,&lck);
		close(gfd);
		return 's';
	}
}
struct customer delete(struct customer del)
{
	int fd=open("customer",O_RDWR,0744);
	int fd2=open("copy",O_CREAT|O_RDWR,0744);
	int size;
	lseek(fd,0,SEEK_SET);
	struct customer r;
	char res='N';
	int delt=0;
	while((size=read(fd,&r,sizeof(r)))!=0)
	{
		if(strcmp(del.username,r.username)==0)//if match found then skipping
		{
			delt=1;
			continue;
		}
		write(fd2,&r,sizeof(r));//writing in copy file
	}
	close(fd);
	close(fd2);
	remove("customer");//removing old file
	rename("copy","customer");//renaming copied file to customer which does not contains deleted record
	if(delt==0)//if username not fond
	{
		strcpy(del.username,"Not found");
	}
	else
	{
		strcpy(del.username,"found");
	}
	
	
	return del;
}
struct customer search(struct customer ser)
{
	int fd=open("customer",O_RDWR,0744);
	int size;
	lseek(fd,0,SEEK_SET);
	struct customer r;
	char res='N';
	while((size=read(fd,&r,sizeof(r)))!=0)
	{
		if(strcmp(ser.username,r.username)==0)
		{
			return r;	//already exist in database
		}
	}
	strcpy(ser.username,"Not found");
	return ser;
}
char add(struct customer add_req)
{
	int fd=open("customer",O_RDWR,0744);
	int size;
	lseek(fd,0,SEEK_SET);
	struct customer r;
	char res='N';
	while((size=read(fd,&r,sizeof(r)))!=0)
	{
		if(strcmp(add_req.username,r.username)==0)
		{
			res='E';	//already exist in database
		}
	}
	if(res=='E')
	{
		return res;
	}
	
	//get an account no
	struct acc_no a;
	int fd2=open("acc_no",O_CREAT|O_RDWR,0744);
	read(fd2,&a,sizeof(a));
	add_req.account_number=++a.account_number;
	lseek(fd2,0,SEEK_SET);
	write(fd2,&a,sizeof(a));
	
	lseek(fd,0,SEEK_END);
	write(fd,&add_req,sizeof(add_req));
	close(fd);
	close(fd2);
	login(add_req,1);
	return 'S';//success
	

}	
void addafterop(struct customer add_req)
{
	int fd=open("customer",O_RDWR,0744);	
	lseek(fd,0,SEEK_END);
	write(fd,&add_req,sizeof(add_req));
	//login(add_req,1);
	//login(add_req,1);
	//write(0,login_time_cp.username,sizeof(login_time_cp.username));
	//write(0,add_req.username,sizeof(login_time_cp.username));
	close(fd);
}


struct modify modification(struct modify m)
{
	int fd=open("customer",O_RDWR,0744);
	int size,size1;
	lseek(fd,0,SEEK_SET);
	struct customer r;
	int found=0;
	struct customer updated;
	struct customer delet;
	while((size=read(fd,&r,sizeof(r)))!=0)
	{
		if(strcmp(m.oldusername,r.username)==0)
		{
			found=1;
			updated=r;
			delet=r;
			switch(m.update)
			{
				case 1:
					
					lseek(fd,0,SEEK_SET);
					struct customer r1;
					while((size1=read(fd,&r1,sizeof(r1)))!=0)
					{
						if(strcmp(m.newusername,r1.username)==0)
						{
							strcpy(m.oldusername,"exist");
							return m;
						}
					}
					strcpy(updated.username,m.newusername);
					break;
				case 2:
					strcpy(updated.password,m.password);
					break;
				case 3:
					updated.type=m.type;
					break;
					
			}
			delete(delet);
			addafterop(updated);
			strcpy(m.oldusername,"ok");
			return m;
			
		}
	}
	if(found==0)
	{
		strcpy(m.oldusername,"Not found");
	}
	return m;
	
}
void writejoint2(struct customer joint)
{
	int fd=open("customer",O_RDWR,0744);
	int size;
	lseek(fd,0,SEEK_SET);
	struct customer r;
	char res='N';
	while((size=read(fd,&r,sizeof(r)))!=0)
	{
		if(joint.account_number==r.account_number&&strcmp(joint.username,r.username)!=0)
		{
			r.balance=joint.balance;//for responce
			delete(r);
			addafterop(r);
			break;
		}
	}

}
void updatepassbook(struct customer data)
{
	char buf[100],path[100];
	sprintf(path,"passbook/%lld",data.account_number);
	time_t t;
	time(&t);
	int len=sprintf(buf,"%s Balance=%f\n",ctime(&t),data.balance);
	int fd=open(path,O_CREAT|O_RDWR,0744);
	lseek(fd,0,SEEK_END);
	write(fd,buf,len);
	close(fd);
}

struct customer bank(struct customer data,int op,struct password_change pass)
{	double nbalance;
	switch(op)
	{
		case 1:
			nbalance=login_time_cp.balance+data.balance;//deposit
			login_time_cp.balance=nbalance;//to store in database
			data.balance=nbalance;//for responce
			delete(login_time_cp);
			addafterop(login_time_cp);
			if(login_time_cp.type=='j')
			{
				writejoint2(login_time_cp);
			}
			updatepassbook(login_time_cp);
			//login(login_time_cp,1);
			login(login_time_cp,1);
			strcpy(data.username,"success");
			break;
		case 2:
			if(login_time_cp.balance>=data.balance)
			{
				nbalance = login_time_cp.balance - data.balance;//withdraw
				login_time_cp.balance = nbalance;//to store in database
				data.balance = nbalance;//for responce
				delete(login_time_cp);
				addafterop(login_time_cp);
				if(login_time_cp.type=='j')
				{
					writejoint2(login_time_cp);
				}
				updatepassbook(login_time_cp);
				login(login_time_cp,1);
				strcpy(data.username,"success");
			}
			else{
				strcpy(data.username,"notenough");
			}
			break;
		case 3:
			data.balance=login_time_cp.balance;//balance query;
			strcpy(data.username,"success");
			break;
		case 4:
			if(strcmp(login_time_cp.password,pass.oldpassword)==0)
			{
				strcpy(login_time_cp.password,pass.newpassword);
				delete(login_time_cp);
				addafterop(login_time_cp);
				login(login_time_cp,1);
				strcpy(data.username,"success");
					
			}
			else
			{
				strcpy(data.username,"notmatch");
			}
			break;
			
		
	}
	return data;

}
char jointadd(struct customer new,struct customer newj)
{
	int fd=open("customer",O_RDWR,0744);
	int size;
	int i=2;
	struct customer test=new;
	while(i-->0)
	{
		lseek(fd,0,SEEK_SET);
		struct customer r;
		char res='N';
		while((size=read(fd,&r,sizeof(r)))!=0)
		{
			if(strcmp(test.username,r.username)==0)
			{
				res='E';
				return res;	//already exist in database
			}
		}
		test=newj;
	}
	
	//get an account no
	struct acc_no a;
	int fd2=open("acc_no",O_CREAT|O_RDWR,0744);
	read(fd2,&a,sizeof(a));
	new.account_number=++a.account_number;
	newj.account_number=new.account_number;//same acc_no
	lseek(fd2,0,SEEK_SET);
	write(fd2,&a,sizeof(a));
	
	lseek(fd,0,SEEK_END);
	write(fd,&new,sizeof(new));
	write(fd,&newj,sizeof(newj));
	close(fd);
	close(fd2);
	return 'S';//success

}



struct customer cmodification(struct customer data,int sd)
{
	struct modify new;
	int c=1;
	while(c==1)
	{
	printf("\nChoose What Wants To Update\n");
	printf("1.Update UserName\n");
	printf("2.Password\n");
	printf("3.Usertype\n");
	int choice;
	scanf("%d",&choice);
	
	
	printf("\nPlease Enter following details\n");
	printf("\nEnter original username(UNIQUE):");
	scanf("%s",new.oldusername);
	switch(choice)
	{
		case 1:
			printf("\nEnter new username(UNIQUE):");
			scanf("%s",new.newusername);
			new.update=1;
			c=0;
			break;	
		case 2:
			printf("\nEnter new password:");
			scanf("%s",new.password);
			new.update=2;
			c=0;
			break;	
		case 3:
			printf("\nEnter usertype(n:normal a:admin j joint):");
			scanf(" %c",&new.type);
			new.update=3;
			c=0;
			break;	
		default:
			printf("\nInvalid choice");
			break;
	
	
	}
	}
	write(sd,&new,sizeof(new));
	struct modify res;
	read(sd,&res,sizeof(res));
	if(strcmp(res.oldusername,"ok")==0)
	{
 		printf("\n\nYour Information is modified successfully.\n\n");
	}
	else
	if(strcmp(res.oldusername,"exist")==0)
	{
		printf("\n\nUsername exist. please use diffrenet username\n\n");
	}
	else
	if(strcmp(res.oldusername,"Not found")==0)
	{
		printf("\n\nUsername not found :(\n\n");
	}
}






//=====================================================================================================================


#endif /* FOO_DOT_H */
