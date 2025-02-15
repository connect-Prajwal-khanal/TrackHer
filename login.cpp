#include<stdio.h>
#include<conio.h>
#include<windows.h>
#include<stdlib.h>
#include<string.h>
#define ENTER 13
#define TAB 9
#define BCKSPC 8
void takepassword(char pwd[50]);
char generateUsername(char email[50],char username[50]);
void takeinput(char ch[50]);

struct user{
	char fullName[50];
	char email[50];
	char password[50];
	char username[50];
	
};


int main(){
	system("color 17");
	FILE *fp;
	int opt,usrFound = 0;
	struct user user;
	char password2[50],again ;
	
	Back:
	system("cls");
	printf("\n\t\t\t\t----------Welcome to authentication system----------");
	printf("\nPlease choose your operation");
	printf("\n1.Signup");
	printf("\n2.Login");
	printf("\n3.Exit");
	
	printf("\n\nYour choice:\t");
	scanf("%d",&opt);
	fgetc(stdin);
	
	switch(opt){
		case 1:
			signup :
			system("cls");
			printf("\n\t\t\t\t\t\t Signup Page ");
			
			printf("\nEnter your full name:\t");
			takeinput(user.fullName);
			printf("Enter your email:\t");
			takeinput(user.email);
			printf("\nPassword must contain more than 8 letters");
			printf("Enter your password:\t");
			takepassword(user.password);
			if(strlen(user.password)<9){
				
				system("cls");
				goto signup ; 
			}
			else{
			    printf("\nConfirm your password:\t");
			takepassword(password2);
		        }
			if(!strcmp(user.password,password2)){
				generateUsername(user.email,user.username);
				fp = fopen("Users.dat","a+");
				fwrite(&user,sizeof(struct user),1,fp);
				if(fwrite != 0) printf("\n\nUser resgistration success\nYour generated username is %s",user.username);
				else printf("\n\nSorry! Something went wrong :(");
				fclose(fp);
			}
			else{
				printf("\n\nPassword donot matched");
				Beep(750,300);
			}
		break;
		
		case 2:
			
			system("cls");
            char username[50],pword[50];
			struct user usr;
			printf("\n\t\t\t\t\t\t\t Login Page ");
			printf("\nEnter your username:\t");
			takeinput(username);
			printf("Enter your password:\t");
			takepassword(pword);
			
			fp = fopen("Users.dat","r");
			while(fread(&usr,sizeof(struct user),1,fp)){
				if(!strcmp(usr.username,username)){
					if(!strcmp(usr.password,pword)){
						system("cls");
						printf("\n\t\t\t\t\t\tWelcome %s",usr.fullName);
						printf("\n\n|Full Name:\t%s",usr.fullName);
						printf("\n|Email:\t\t%s",usr.email);
						printf("\n|Username:\t%s",usr.username);
				
					}
					else {
						printf("\n\nInvalid Password!");
						Beep(800,300);
					}
					usrFound = 1;
				}
			}
			if(!usrFound){
				printf("\n\nUser is not registered!");
				Beep(800,300);
			}
			fclose(fp);
			break;
			
		case 3:
			system("cls");
			printf("\n\n\t\t\t\t\t\tThanks For Visiting\n");
			return 0;
			
		
		default:
			system("cls");
			printf("\n\n\t\t\t\t\t\tInvalid Operation\nFor accessing homepage press:y\n");
			again=getchar();
			if(again=='y'||again=='Y')
			{
				goto Back ; 
			}
			
			return 0;
	}
			
	
	
	
	return 0;
}


void takeinput(char ch[50]){
	fgets(ch,50,stdin);
	ch[strlen(ch) - 1] = 0; 
}

char generateUsername(char email[50],char username[50]){
	
	for(int i=0;i<strlen(email);i++){
		if(email[i] == '@') break;
		else username[i] = email[i];
	}
}

void takepassword(char pwd[50]){
	int i;
	char ch;
	while(1){
		ch = getch();
		if(ch == ENTER || ch == TAB){
			pwd[i] = '\0';
			break;
		}else if(ch == BCKSPC){
			if(i>0){
				i--;
				printf("\b \b");
			}
		}else{
			pwd[i++] = ch;
			printf("* \b");
		}
	}
}


