#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include "./functions/head.h"
#include "./functions/first.h"
int main()
{
	char acc;
	printf("Server is starting...");
	printf("\n\nWants to create an Account, Press Y for yes and any other key to continue : ");
	scanf("%c",&acc);
	if(acc=='y' || acc=='Y'){
		int ff=first();
	}	
	struct sockaddr_in serv, cli;
	mkdir("passbook",0777);
	int sd,nsd,size_client; 
	sd= socket (AF_INET, SOCK_STREAM, 0);

	serv.sin_family = AF_INET;
	serv.sin_addr.s_addr = INADDR_ANY;
	
	//Insert new port no here
	serv.sin_port = htons(7940);
	
	int bind_result=bind(sd, (void *)(&serv), sizeof(serv));
	int listen_result=listen (sd, 5);
	printf("Server Initializing...\nSocket returns :: %d\nBind result:: %d\nListening status %d\n",sd,bind_result,listen_result);
	while(1) {
	int sz=sizeof(cli);
	nsd =accept (sd, (void *)&cli,&sz);
	printf("%d",nsd);
	if (!fork( )) {
		   while(1)
		   {
			   close(sd);
			   write(0,"start\n",7);//to check where control goes
			   struct customer request;
			   char c;
			   int size=read(nsd,&request,sizeof(request)); // will take a request a customer which is of struct customer
			   c=login(request,1);//login classified as a for admin,n for normal,j for joint and before and inside
			   login(request,1);		//L for alreday logged in
			   if(c=='a')
			   {
			   	
			   		write(0,"adminstart\n",12);//to check where control goes
			   		write(nsd,"admin",6);
			   		struct customer new;
			   		int in=0,newsize;
			   		while(in==0)
			   		{
				   		char cmd[10];
				   		read(nsd,&cmd,10); // thro socket programming we are getting value
				   		if(strcmp(cmd,"add")==0)
				   		{
					   		int j=1;
					   		char c;
					   		struct customer newj;
					   		while(1)
					   		{
					   		while((newsize=read(nsd,&new,sizeof(new)))==0);
					   		if(new.type=='j')//for joint
					   		{
					   			read(nsd,&newj,sizeof(newj));
					   			c=jointadd(new,newj);
					   		}
					   		else
					   		{
					   			c=add(new);
					   		}
					   	
						   		switch(c)
						   		{
						   			case 'E':
						   				write(nsd,"exist",6);
						   				break;
						   			case 'S':
						   				write(nsd,"ok",3);
										break;
						   				break;
						   			default:
						   				write(nsd,"default",7);
						   				break;
						   				break;
						   		}
						   	}
					   	}
					   	else if(strcmp(cmd,"mod")==0)
					   	{
					   		struct modify m;
					   		read(nsd,&m,sizeof(m));
					   		m=modification(m);
					   		write(nsd,&m,sizeof(m));
					   	
					   	}
					   	else if(strcmp(cmd,"srh")==0)
					   	{
					   		struct customer ser;
					   		read(nsd,&ser,sizeof(ser));
					   		ser=search(ser);
					   		//write(0,ser.username,sizeof(ser.username));
					   		write(nsd,&ser,sizeof(ser));
					   	
					   	}
					   	else if(strcmp(cmd,"del")==0)
					   	{
					   		struct customer del;
					   		read(nsd,&del,sizeof(del));
					   		del=delete(del);
					   		//write(0,del.username,sizeof(ser.username));
					   		write(nsd,&del,sizeof(del));
					   	}
					   	else if(strcmp(cmd,"out")==0)
					   	{
					   		in=1;
					   	}
					   	
			   		}
			   	}
			   	else if(c=='n'||c=='j')
			   	{
			   		write(0,"normalstart\n",14);//to check where control goes
			   		if(c=='j')
			   		{
			   			write(nsd,"joint",6);
			   		}
			   		else
			   		{
			   			write(nsd,"normal",6);
			   		}
			   		char cmd[10];
			   		int in_n=1;
			   		
			   		while(in_n==1)
			   		{
			   			read(nsd,&cmd,10);
			   			struct password_change dummy;
			   			if(strcmp(cmd,"dep")==0)//deposit
			   			{
			   				//remaining from here
			   				struct customer dep;
			   				read(nsd,&dep,sizeof(dep));
			   				dep=bank(dep,1,dummy);
			   				write(nsd,&dep,sizeof(dep));
			   			}
			   			else if(strcmp(cmd,"wit")==0)//withdraw
			   			{
			   				struct customer wit;
			   				read(nsd,&wit,sizeof(wit));
			   				wit=bank(wit,2,dummy);
			   				write(nsd,&wit,sizeof(wit));
			   			}
			   			else if(strcmp(cmd,"bal")==0)
			   			{
			   				struct customer bal;
			   				bal=bank(bal,3,dummy);
			   				write(nsd,&bal,sizeof(bal));
			   			}
			   			else if(strcmp(cmd,"pas")==0)
			   			{
			   				struct password_change pass;
			   				struct customer res;
			   				read(nsd,&pass,sizeof(pass));
			   				res=bank(res,4,pass);
			   				write(nsd,&res,sizeof(res));
			   				
			   			}
			   			else if(strcmp(cmd,"vie")==0)
			   			{
			   				
							char path[100],buf[10000];
							sprintf(path,"passbook/%lld",login_time_cp.account_number);
							int fd=open(path,O_CREAT|O_RDONLY,0744);
							int len=read(fd,buf,10000);
							if(len==-1||len==0)
							{
								write(nsd,"empty",6);	
							}
							else
							{
								write(nsd,buf,len);
							}
							close(fd);
							
			   				
			   			}
			   			else if(strcmp(cmd,"out")==0)
			   			{
			   				in_n=0;
			   			}
			   			else if(strcmp(cmd,"cht")==0)
			   			{
			   				char give[1024],take[1024];
			   				static int i=1;
			   				i++;
			   				while(1){
			   				read(nsd,&take,1024);
			   				printf("Client %s and it's query %s\n", request.username,take);
			   				printf("Write into clinet.. :");
			   				gets(give);
			   				write(nsd,&give,1024);
			   			}
			   			
			   		}
			   		write(0,"normalend\n",10);//to check where control goes
			   	}
				}
			   	else if(c=='I')
			   	{
			   		write(nsd,"Invalid",7);
			   	}	
			   	else if(c=='L')
			   	{
			   	
			   		write(nsd,"logged",7);
			   	}
			   	else
			   	{
			   		write(nsd,"default\n",6);
			   	}
			   	struct customer dumy;
			   	login(dumy,0);
			   } 
	}
	else{
	 	close(nsd);
	 	}
	}

}
