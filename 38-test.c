#include <stdio.h> 
#include <string.h> 
 
#define Total_People

#define Person1 "A"
#define Person2 "B"
#define Person3 "C"
#define Person4 "D"

int votesCount1=0, votesCount2=0, votesCount3=0, votesCount4=0, Wastedvotes=0;

void castVote(){
int choice;    
printf("\n\n ### Please choose your Candidate ####\n\n");
printf("\n 1. %s", Person1);
printf("\n 2. %s", Person2);
printf("\n 3. %s", Person3);
printf("\n 4. %s", Person4);
printf("\n 5. %s", "None of These");

printf("\n\n Input your choice (1 - 4) : ");
scanf("%d",&choice);

switch(choice){
    case 1: votesCount1++; 
            break;
    case 2: votesCount2++; 
            break;
    case 3: votesCount3++;
            break;
    case 4: votesCount4++; 
            break;
    case 5: spoiledtvotes++; 
            break;
    default: printf("\n Error: Wrong Choice !! Please retry");
            
             getchar();
}
printf("\n thanks for vote !!");
}

void votesCount(){
printf("\n\n ##### Voting Statics ####");
printf("\n %s - %d ", Person1, votesCount1);
printf("\n %s - %d ", Person1, votesCount2);
printf("\n %s - %d ", Person1, votesCount3);
printf("\n %s - %d ", Person1, votesCount4);
printf("\n %s - %d ", "Spoiled Votes", Wastedvotes); 
}

void getLeadingCandidate(){
    printf("\n\n  #### Leading Candiate ####\n\n");
    if(votesCount1>votesCount2 && votesCount1>votesCount3 && votesCount1 >votesCount4)
    printf("[%s]",Person1);
    else if (votesCount2>votesCount3 && votesCount2>votesCount4 && votesCount2 >votesCount1)
    printf("[%s]",Person2);
    else if(votesCount3>votesCount4 && votesCount3>votesCount2 && votesCount3 >votesCount1)
    printf("[%s]",Person3);
    else if(votesCount4>votesCount1 && votesCount4>votesCount2 && votesCount4 >votesCount3)
    printf("[%s]",Person4);
    else
    printf("----- Warning !!! No-win situation----");    
    
    
    
}


int main() 
{ 
    char username[15]; 
    char password[12]; 
 
 
    printf("Enter your username:\n"); 
    scanf("%s",&username); 
 
    printf("Enter your password:\n"); 
    scanf("%s",&password); 
 
    if(strcmp(username,"Nayeem")==0){ 
        if(strcmp(password,"12345")==0){ 
 
        printf("\nWelcome.Login Success!"); 
            int i;
            int choice;


        do{


printf("\n\n  ******Welcome to Voting 2022 ******");
printf("\n\n 1. Cast Your Vote");
printf("\n 2. Check Vote Count");
printf("\n 3. Check leading Candidate");
printf("\n 0. Exit");

printf("\n\n Please enter your choice : ");
scanf("%d", &choice);

switch(choice)
{
case 1: castVote();break;
case 2: votesCount();break;
case 3: getLeadingCandidate();break;
default: printf("\n Error: Invalid Choice");
}

}while(choice!=0);

getchar();

 
        }else{ 
    printf("\nwrong password"); 
} 
    }else{ 
    printf("\nUser doesn't exist"); 
} 
 
 
 
 
 
    return 0; 
 
}
