#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <string.h>
#include <conio.h>

/*
To be used for comparison purposes

typedef struct _SYSTEMTIME{
		WORD wYear;
		WORD wMonth;
		WORD wDayOfWeek;
		WORD wDay;
		WORD wHour;
		WORD wMinute;
		WORD wSecond;
		WORD wMilliseconds;
}SYSTEMTIME;
*/

typedef struct
{
    int bldg;
    char street[50];
    char city[50];
}Address;

typedef struct
{
	int Day;
	int Month;
	int Year;
}Date;

typedef struct
{
    char Surname[50];
    char LastName[50];
    int ID;
    Address Address;
    long PhoneNumber;
    int Age;
    char mail[50];
}Member;

typedef struct
{
	char Title[70];
	char AuthorName[70];
	char Publisher[70];
	char ISBN[20];
	Date Publication;
	int copies;
	int copies_available;
	char Category[30];
}Book;

typedef struct
{
	int Transaction;
	char ISBN[20];
	int ID;
	Date Dateissued;
	Date Datedue;
	Date return_date;
}Borrow;

typedef struct
{
	char ISBN[20];
	int occur;
	char title[70];
}BorrowedBook;

Member MembersArray[100];
Book BooksArray[100];
Borrow BorrowsArray[100];
BorrowedBook PopularArray[100];

int c1=0,c2=0,c3=0,LastMember=0,LastBook=0,LastBorrow=0,LastTrans=0,input1,input2,input4,input5,input6,input7,input8;
char input3;
SYSTEMTIME DateTime;
Date today;

void MainMenu();
void BookManagement();

int CheckEmail(char *mail)
{
    int found=0,i,charcount=0,midcount=0,last=0,refused=0;
    for(i=0;i<=strlen(mail);i++)
    {
        if(mail[i]==')' || mail[i]=='(' || mail[i]==','|| mail[i]=='\'' || mail[i]==';' || mail[i]==':' || mail[i]==')' || mail[i]==' ' || mail[i]=='/' || mail[i]=='\\' || mail[i]=='[' || mail[i]==']' || mail[i]=='{' || mail[i]=='}' || mail[strlen(mail) - 1]=='.' )
            refused++;
    }
    if(refused!=0)
    {
        return 0;
    }
    else{
            for(i=0;i<=strlen(mail);i++)
            {
                if(mail[i]=='@')
                    {
                        found++;
                        break;
                    }
                else if(isalpha(mail[i]) || isdigit(mail[i]))
                    charcount++;
            }
            for(i=charcount;i<=strlen(mail);i++)
            {
                if(mail[i]=='.')
                {
                    found++;
                    break;
                }
                else if(isalpha(mail[i]) || isdigit(mail[i]))
                    midcount++;
            }
            for(i=charcount+midcount;i<strlen(mail);i++)
            {
                if(isalpha(mail[i]) || isdigit(mail[i]))
                last++;
            }
            if(found!=2 || charcount==0 || midcount==0 || last==0)
                return 0;
            else
                return 1;
        }
}

int CheckInt(int n)
{
    if(isdigit(n)!=1 || (n>47&&n<58))
        return 1;
    else
        return 0;
}

int CheckDate(Date n)
{
    if(n.Month>12 || n.Month<0 || CheckInt(n.Month)==0)
    return 0;
	else if(n.Day>31 || n.Day<0 || CheckInt(n.Day)==0)
	return 0;
	else if(n.Year>3000 || CheckInt(n.Year)==0)
	return 0;
	else
        return 1;
}

int CheckPhone(int n)
{
    if(n/10000000!=0 || n/1000000==0)
        return 0;
    else
        return 1;
}

int CheckISBN(char *n)
{
    int i=0,digcount=0;
    for(i=0;n[i]!=0;i++)
    {
        if(n[i]>47 && n[i]<58)
            {
            digcount++;
            }
    }
    if(digcount<13 || digcount>13)
        return 0;
    else return 1;
}

int CheckID(int n)
{
    if(n/10000!=0 || n<1000 || CheckInt(n)==0)
        return 0;
    else
        return 1;
}

void OverdueBooks()
{
    int i=0;
    printf("Transaction \t ID \t ISBN \t Date due\n");
    for(i=0;i<LastBorrow-1;i++)
    {
        if(BorrowsArray[i].return_date.Day==0)
        {
            if (BorrowsArray[i].Datedue.Year<DateTime.wYear)
            {
                printf("%d \t\t %d \t %s \t %d/%d/%d\n",BorrowsArray[i].Transaction, BorrowsArray[i].ID,BorrowsArray[i].ISBN, BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
            }
            else if(BorrowsArray[i].Datedue.Year==DateTime.wYear)
            {
                if(BorrowsArray[i].Datedue.Month<DateTime.wMonth)
                {
                    printf("%d \t %d \t %s \t %d/%d/%d\n",BorrowsArray[i].Transaction, BorrowsArray[i].ID,BorrowsArray[i].ISBN, BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
                }
                else if(BorrowsArray[i].Datedue.Day<DateTime.wDay)
                {
                    printf("%d \t %d \t %s \t %d/%d/%d\n",BorrowsArray[i].Transaction, BorrowsArray[i].ID,BorrowsArray[i].ISBN, BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
                }
            }
        }
    }
    printf("\n\nPress Any Key to Continue:\n");
    getch();
}

void PopBooks()
{
    BorrowedBook temp;
    int i=0,j=0,k,Last=0,flag=0;
    for(i=0;i<100;i++)
    {
        PopularArray[i].occur=0;
    }
    for(i=0;i<LastBorrow-1;i++)
    {
        flag=0;
        for(k=0;k<=Last;k++)
        {
            if(strcmp(BorrowsArray[i].ISBN,PopularArray[k].ISBN)==0)
            {
                PopularArray[k].occur++;
                flag=1;
            }
        }
        if(flag==0)
        {
                strcpy(PopularArray[Last].ISBN,BorrowsArray[i].ISBN);
                PopularArray[Last].occur=1;
                Last++;
        }
    }
    for(i=0;i<Last;i++)
    {
        for(j=0;j<=LastBook;j++)
        {
            if(strcmp(BooksArray[j].ISBN,PopularArray[i].ISBN)==0)
                strcpy(PopularArray[i].title,BooksArray[j].Title);
        }
    }
    for(i=0;i<Last-1;i++)
    {
        j=i+1;
        for(j;j<Last;j++)
        {
            if(PopularArray[j].occur>PopularArray[i].occur)
            {
                temp.occur=PopularArray[i].occur;
                strcpy(temp.ISBN,PopularArray[i].ISBN);
                strcpy(temp.title,PopularArray[i].title);

                PopularArray[i].occur=PopularArray[j].occur;
                strcpy(PopularArray[i].ISBN,PopularArray[j].ISBN);
                strcpy(PopularArray[i].title,PopularArray[j].title);

                PopularArray[j].occur=temp.occur;
                strcpy(PopularArray[j].ISBN,temp.ISBN);
                strcpy(PopularArray[j].title,temp.title);
            }
        }
    }
    printf("HOT READS!!\n");
    printf("Book Title \t\t ISBN \t\t\t #Times Borrowed\n");
    for(i=0;i<5;i++)
    {
        printf("%s \t %s \t %d\n",PopularArray[i].title,PopularArray[i].ISBN,PopularArray[i].occur);
    }
    printf("\nPress Any Key to Continue\n");
    getch();
}

void NewBorrow()
{
    int checkid,checkdate,i=0,found=0,flag=0;
    int found1=0;
    Borrow temp;
    do{
        printf("Please enter the ID of the borrowing member:\n");
        do{
            scanf("%d",&temp.ID);
            checkid=CheckID(temp.ID);
            if(checkid==0)
                printf("Please enter a legitimate ID! ID numbers start from 1001 and end at 9999.\n");
        }while(checkid==0);
        for(i=0;(found==0)&&(i<LastMember-1) ;i++)
        {
            if(MembersArray[i].ID==temp.ID)
                found=1;
        }
        if(found==0)
            printf("This member ID isn't registered in the system!\n");
    }while(found==0);
    if(found!=0 && checkid!=0)
    {
        for(i=0;i<LastBorrow;i++)
        {
            if(BorrowsArray[i].ID==temp.ID)
                flag++;
        }
        if(flag==3)
        {
            printf("This member can't borrow any more books as they have 3 unreturned books!\n");
            printf("Press Any Key to Continue\n");
            getch();
            MainMenu();
        }
        else{
            do{
            printf("Please enter the ISBN of the book to be borrowed:(ex: 978-3-16-148410-0)\n");
            fflush(stdin);
            fgets(temp.ISBN,sizeof(temp.ISBN),stdin);
            temp.ISBN[strlen(temp.ISBN) - 1] = 0;
            if(CheckISBN(temp.ISBN)==0)
                printf("Please enter a legitimate ISBN!(ex: 978-3-16-148410-0)\n");
            }while(CheckISBN(temp.ISBN)==0);
            for(i=0;(found1==0);i++)
            {
                if(strcmp(BooksArray[i].ISBN,temp.ISBN)==0)
                {
                    found1=1;
                    if(BooksArray[i].copies_available>0)
                    BooksArray[i].copies_available--;
                    else
                    {
                        printf("There are no more copies available.\n");
                        printf("\nPress Any Key to Continue\n");
                        getch();
                        MainMenu();
                    }
                }
            }
            for(i=0;i<99;i++)
            {
                if(strcmp(BooksArray[i].ISBN,temp.ISBN)==0)
                    {found=1;
                    printf("Book wanted is: %s by %s\n",BooksArray[i].Title,BooksArray[i].AuthorName);
                    flag=i;
                    break;
                    }
            }
        if(found1==0)
            printf("This book ISBN isn't registered in the system!\n");
        else
        {
            BorrowsArray[LastBorrow-1].Transaction=BorrowsArray[LastBorrow-2].Transaction+1;
        do{
                checkdate=1;
                printf("Please enter Date due to return. (DD enter MM enter YY enter) \n");
                scanf("%d %d %d",&temp.Datedue.Day,&temp.Datedue.Month,&temp.Datedue.Year);
                if(DateTime.wYear>temp.Datedue.Year)
                {
                    printf("Date due can't be before today's date!\n");
                    checkdate=0;
                }
                else if((DateTime.wYear==temp.Datedue.Year) && (DateTime.wMonth>temp.Datedue.Month))
                {
                    printf("Date due can't be before today's date!\n");
                    checkdate=0;
                }
                else if((DateTime.wYear==temp.Datedue.Year) && (DateTime.wMonth==temp.Datedue.Month) && (DateTime.wDay>temp.Datedue.Day))
                {
                    printf("Date due can't be before today's date!\n");
                    checkdate=0;
                }
        }while(checkdate==0);
        }}}
        BorrowsArray[LastBorrow-1].ID=temp.ID;
        BorrowsArray[LastBorrow-1].Dateissued.Day=DateTime.wDay;
        BorrowsArray[LastBorrow-1].Dateissued.Month=DateTime.wMonth;
        BorrowsArray[LastBorrow-1].Dateissued.Year=DateTime.wYear;
        BorrowsArray[LastBorrow-1].Datedue.Day=temp.Datedue.Day;
        BorrowsArray[LastBorrow-1].Datedue.Month=temp.Datedue.Month;
        BorrowsArray[LastBorrow-1].Datedue.Year=temp.Datedue.Year;
        BorrowsArray[LastBorrow-1].return_date.Day=0;
        BorrowsArray[LastBorrow-1].return_date.Month=0;
        BorrowsArray[LastBorrow-1].return_date.Year=0;
        strcpy(BorrowsArray[LastBorrow-1].ISBN,temp.ISBN);
        LastBorrow++;
        LastTrans++;
        printf("Borrow transaction added successfully!");
        printf("\nPress Any Key to Continue\n");
        getch();
}

void ReturnBorrow()
{
    Date temp;
    int Trans,i=0,found=0,checkdate=1,found1=0;
    char ISBN[20];
    printf("Books that are yet to be returned:\n");
    printf("Transaction \t ID \t ISBN \t Date due\n");
    for(i=0;i<LastBorrow-1;i++)
    {
        if(BorrowsArray[i].return_date.Day==0)
        {
           printf("%d \t\t %d \t %s \t %d/%d/%d\n",BorrowsArray[i].Transaction, BorrowsArray[i].ID,BorrowsArray[i].ISBN, BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
        }
    }
    printf("Please enter Transaction number:\n");
        do{
            scanf("%d",&Trans);
            if(CheckInt(Trans)==0)
                printf("\nPlease enter an integer!");
        }while(CheckInt(Trans)==0);
    for(i=0;i<LastBorrow;i++)
    {
        if(BorrowsArray[i].Transaction==Trans)
        {
            found=1;
            strcpy(ISBN,BorrowsArray[i].ISBN);
            break;
        }
        i++;
    }
    if(found==0)
        printf("This transaction number is not registered or already returned!");
    else{
    do{
        printf("Please enter Date of return. (DD, enter, MM, enter, YY, enter) \n");
        scanf("%d %d %d",&temp.Day,&temp.Month,&temp.Year);
        checkdate=CheckDate(temp);
        if(checkdate==0)
            printf("Please enter a date that is not later than today! \n");
        }while(checkdate==0);
    BorrowsArray[i].return_date.Day=temp.Day;
    BorrowsArray[i].return_date.Month=temp.Month;
    BorrowsArray[i].return_date.Year=temp.Year;
    for(i=0;(found1==0);i++)
    {
        if(strcmp(BooksArray[i].ISBN,ISBN)==0)
        {
            found1=1;
            BooksArray[i].copies_available++;
        }
    }
    printf("Book's return was registered successfully!\n");
    }
    printf("\n\n");
    printf("Press Any Key to Continue\n");
    getch();
}

void DisplayMembers()
{
    int i=0;
    printf("\t Surname \t   Last Name \t Age \t ID \t Phone Number \t Address \t Email \n");
    for(i=0;i<LastMember-1;i++)
    {
        printf(" %d \t %s  %15s \t %d \t %d \t %ld \t %d,%s,%s \t %30s \n",i+1,MembersArray[i].Surname,MembersArray[i].LastName,MembersArray[i].Age,MembersArray[i].ID,MembersArray[i].PhoneNumber,MembersArray[i].Address.bldg,MembersArray[i].Address.street,MembersArray[i].Address.city,MembersArray[i].mail);
    }
    printf("\n\n");
    printf("Press Any Key to Continue\n");
    getch();
}

void DisplayBorrowings()
{

int i=0;
    printf("Transaction \t ISBN \t\t Borrower ID \t Issued Date \t Due Date \t Return Date\n");
    for(i=0;i<LastBorrow-1;i++)
    {
        printf("%d      \t %s \t %d \t\t %d/%d/%d \t %d/%d/%d \t",BorrowsArray[i].Transaction,BorrowsArray[i].ISBN,BorrowsArray[i].ID,BorrowsArray[i].Dateissued.Day,BorrowsArray[i].Dateissued.Month,BorrowsArray[i].Dateissued.Year,BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
        if(BorrowsArray[i].return_date.Day==0)
            printf("Not yet returned\n");
        else
            printf("%d/%d/%d\n",BorrowsArray[i].return_date.Day,BorrowsArray[i].return_date.Month,BorrowsArray[i].return_date.Year);
    }
    printf("\n\n");
    printf("Press Any Key to Continue\n");
    getch();
}

void DeleteMember()
{
    system("cls");
    int id,checkid,i,found=0,flag=0,j,ActiveBorrow=0;
   do { printf("Please enter the ID of the member to be deleted:\n");
    scanf("%d",&id);
    checkid=CheckID(id);
    }while (checkid==0);
    for(i=0;i<100;i++)
    {
        if(MembersArray[i].ID==id)
            {
                found=1;
                flag=i;
            }
    }
        if (found==0)
        {printf("This member ID isn't registered in the system!");
        MainMenu();}
        else{
                for(j=0;j<LastBorrow;j++)
                {
                    if(BorrowsArray[j].ID==id && BorrowsArray[j].return_date.Day==0)
                        ActiveBorrow=1;
                }
                if(ActiveBorrow==1)
                    printf("This user has an active borrow and has not returned a book yet! They can't be deleted till then!");
                else{
                    while(flag<99)
                    {
                        MembersArray[flag].ID=MembersArray[flag+1].ID;
                        MembersArray[flag].Age=MembersArray[flag+1].Age;
                        MembersArray[flag].PhoneNumber=MembersArray[flag+1].PhoneNumber;
                        MembersArray[flag].Address.bldg=MembersArray[flag+1].Address.bldg;
                        strcpy(MembersArray[flag].LastName,MembersArray[flag+1].LastName);
                        strcpy(MembersArray[flag].Surname,MembersArray[flag+1].Surname);
                        strcpy(MembersArray[flag].mail,MembersArray[flag+1].mail);
                        strcpy(MembersArray[flag].Address.city,MembersArray[flag+1].Address.city);
                        strcpy(MembersArray[flag].Address.street,MembersArray[flag+1].Address.street);
                        flag++;
                    }
                    LastMember--;
                    printf("Member data has been deleted successfully!");
                    }
            }

    printf("\n\n");
    printf("Press Any Key to Continue\n");
    getch();
}

void NewMember()
{
    system("cls");
    Member temp;
    int i=0;
    int checkid=1;
    printf("Please enter the surname:\n");
    fgetc(stdin);
    fgets(temp.Surname,sizeof(temp.Surname),stdin);
    temp.Surname[strlen(temp.Surname) - 1] = 0;
    printf("Please enter the last name:\n");
    fgets(temp.LastName,sizeof(temp.LastName),stdin);
    temp.LastName[strlen(temp.LastName) - 1] = 0;
    do{
        printf("Please enter the Email:(ex: example@mail.com)\n");

        fgets(temp.mail,sizeof(temp.mail),stdin);
        temp.mail[strlen(temp.mail) - 1] = 0;
        if(CheckEmail(temp.mail)==0)
            printf("Please enter a valid Email (ex: example@mail.com)\n");
        }while(CheckEmail(temp.mail)==0);
    do{
        checkid=1;
        printf("Enter the member ID:\n");
        scanf("%d",&temp.ID);
        if(CheckID(temp.ID)==0)
            printf("\nPlease enter a legitimate ID! ID numbers start from 1001 and end at 9999.\n");
        for(i=0;i<LastMember;i++)
        {
            if(temp.ID==MembersArray[i].ID)
            {
                checkid=0;
                printf("This member is already registered in the system!\n");
            }
    }}while(checkid==0 || CheckID(temp.ID)==0);
    printf("Please enter the member's city of residence:\n");
    fgetc(stdin);
    fgets(temp.Address.city,sizeof(temp.Address.city),stdin);
    temp.Address.city[strlen(temp.Address.city) - 1] = 0;
    printf("Please enter the member house's street:\n");
    fgets(temp.Address.street,sizeof(temp.Address.street),stdin);
    temp.Address.street[strlen(temp.Address.street) - 1] = 0;
    printf("Please enter the member house's building number:\n");
    do{
        scanf("%d",&temp.Address.bldg);
        if(CheckInt(temp.Address.bldg)==0)
            printf("Please enter an integer!\n");
    }while(CheckInt(temp.Address.bldg)==0);
    do{
        printf("Enter the member's age:\n");
        scanf("%d",&temp.Age);
        if(CheckInt(temp.Age)==0)
            printf("Please enter an integer!\n");
    }while(CheckInt(temp.Age)==0);
    do{
        printf("Enter the member's phone number:\n");
        scanf("%ld",&temp.PhoneNumber);
        if(CheckPhone(temp.PhoneNumber)==0)
            printf("Please enter a valid phone number!\n");
    }while(CheckPhone(temp.PhoneNumber)==0);
    printf("\nMember added successfully!");

    MembersArray[LastMember-1].ID=temp.ID;
    MembersArray[LastMember-1].PhoneNumber=temp.PhoneNumber;
    MembersArray[LastMember-1].Age=temp.Age;
    MembersArray[LastMember-1].Address.bldg=temp.Address.bldg;
    strcpy(MembersArray[LastMember-1].Surname,temp.Surname);
    strcpy(MembersArray[LastMember-1].LastName,temp.LastName);
    strcpy(MembersArray[LastMember-1].mail,temp.mail);
    strcpy(MembersArray[LastMember-1].Address.city,temp.Address.city);
    strcpy(MembersArray[LastMember-1].Address.street,temp.Address.street);
    LastMember++;

    printf("\n\n");
    printf("Press Any Key to Continue\n");
    getch();
}

void AddCopies()
{
    system("cls");
    int i,flag=0,n,found=0;
    Book temp;
    printf("Please enter book ISBN:\n");
     fgetc(stdin);
    fgets(temp.ISBN,sizeof(temp.ISBN),stdin);
    temp.ISBN[strlen(temp.ISBN) - 1] = 0;
    for(i=0;i<99;i++)
    {
        if(strcmp(BooksArray[i].ISBN,temp.ISBN)==0 || strcmp(BooksArray[i].ISBN,temp.ISBN)=='\0')
            {found=1;
            printf("Book wanted is: %s by %s\n",BooksArray[i].Title,BooksArray[i].AuthorName);
            flag=i;
            break;
            }
    }
    if(CheckISBN(temp.ISBN)!=1)
    {
        printf("Please enter a legitimate ISBN!(ex: 978-3-16-148410-0)\n");
    }
    else if(found==0)
        printf("This book ISBN isn't registered in the system!\n");
    else
    {
    printf("How many copies do you want added:\n");
    scanf("%d",&n);
    BooksArray[flag].copies+=n;
    BooksArray[flag].copies_available+=n;
    printf("Copies have been added successfully!");
    }
    printf("\n\n");
    printf("Press Any Key to Continue\n");
    getch();
    }

void DeleteBook()
{
    system("cls");
    Book temp;
    int i,found=0,flag=0;
    printf("Please enter the ISBN of the book to be deleted: (ex: 978-3-16-148410-0)\n");
    fgetc(stdin);
    fgets(temp.ISBN,sizeof(temp.ISBN),stdin);
    temp.ISBN[strlen(temp.ISBN) - 1] = 0;
    for(i=0;i<99;i++)
    {
        if(strcmp(BooksArray[i].ISBN,temp.ISBN)==0 || strcmp(BooksArray[i].ISBN,temp.ISBN)=='\0')
            {
            found=1;
            flag=i;
            break;
            }
    }
    if(CheckISBN(temp.ISBN)!=1)
    {
        printf("Please enter a legitimate ISBN!(ex: 978-3-16-148410-0)\n");
    }
    else if(found==0)
        printf("This book ISBN isn't registered in the system!\n");
    else
    {
        while(flag<99)
        {
            BooksArray[flag].copies=BooksArray[flag+1].copies;
            BooksArray[flag].copies_available=BooksArray[flag+1].copies_available;
            BooksArray[flag].Publication.Day=BooksArray[flag+1].Publication.Day;
            BooksArray[flag].Publication.Month=BooksArray[flag+1].Publication.Month;
            BooksArray[flag].Publication.Year=BooksArray[flag+1].Publication.Year;
            strcpy(BooksArray[flag].AuthorName,BooksArray[flag+1].AuthorName);
            strcpy(BooksArray[flag].Title,BooksArray[flag+1].Title);
            strcpy(BooksArray[flag].ISBN,BooksArray[flag+1].ISBN);
            strcpy(BooksArray[flag].Publisher,BooksArray[flag+1].Publisher);
            flag++;
        }
        LastBook--;
        printf("Book data has been deleted successfully!");
    }
    printf("\n\n");
    printf("Press Any Key to Continue\n");
    getch();
}

void NewBook()
{
    system("cls");
    Book temp;
    int i=0,flag=0;
    int checkdate;
    printf("Please enter the book title:");
    fgetc(stdin);
    fgets(temp.Title,sizeof(temp.Title),stdin);
    temp.Title[strlen(temp.Title) - 1] = 0;
    printf("\nPlease enter the author's name:");
    fgets(temp.AuthorName,sizeof(temp.Title),stdin);
    temp.AuthorName[strlen(temp.AuthorName) - 1] = 0;
    printf("\nPlease enter the publisher's name:");
    fgets(temp.Publisher,sizeof(temp.Publisher),stdin);
    temp.Publisher[strlen(temp.Publisher) - 1] = 0;
    do{
    printf("\nPlease enter Date Published:(DD enter MM enter YYYY enter)\n");
    scanf("%d %d %d",&temp.Publication.Day,&temp.Publication.Month,&temp.Publication.Year);
    checkdate=CheckDate(temp.Publication);
    if(checkdate==0)
        printf("\nPlease enter a date that is not later than today!\n");
    }while(checkdate==0);
    do{
    printf("\nEnter the book ISBN (ex: 978-3-16-148410-0):\n");
    fflush(stdin);
    fgets(temp.ISBN,sizeof(temp.ISBN),stdin);
    temp.ISBN[strlen(temp.ISBN) - 1] = 0;
    if(CheckISBN(temp.ISBN)==0)
        printf("\nPlease enter a legitimate ISBN! (ex: 978-3-16-148410-0)");
    else{
        for(i=0;i<=LastBook;i++)
    {
        if(strcmp(temp.ISBN,BooksArray[i].ISBN)==0)
        {
            flag=1;
            printf("This book ISBN is already registered in the system. You can add new copies or enter another ISBN!");
        }
    }}}while(CheckISBN(temp.ISBN)==0 || flag==1);
    do{
    printf("Enter the number of copies:\n");
    scanf("%d",&temp.copies);
    if(CheckInt(temp.copies)==0)
        printf("Please enter an integer! \n");
    }while(CheckInt(temp.copies)==0);
    temp.copies_available=temp.copies;
    printf("Enter the book category:\n");
    fgetc(stdin);
    fgets(temp.Category,sizeof(temp.Category),stdin);
    temp.Category[strlen(temp.Category) - 1] = 0;

    BooksArray[LastBook-1].copies=temp.copies;
    BooksArray[LastBook-1].copies_available=temp.copies_available;
    BooksArray[LastBook-1].Publication.Day=temp.Publication.Day;
    BooksArray[LastBook-1].Publication.Month=temp.Publication.Month;
    BooksArray[LastBook-1].Publication.Year=temp.Publication.Year;
    strcpy(BooksArray[LastBook-1].ISBN,temp.ISBN);
    strcpy(BooksArray[LastBook-1].Title,temp.Title);
    strcpy(BooksArray[LastBook-1].AuthorName,temp.AuthorName);
    strcpy(BooksArray[LastBook-1].Category,temp.Category);
    strcpy(BooksArray[LastBook-1].Publisher,temp.Publisher);
    LastBook++;
    printf("%d",LastBook);
    printf("\nBook Successfully added ");
    printf("\n\n");

    printf("Press Any Key to Continue\n");
    getch();
}

void DisplayBooks()
{
    system("cls");
    int i=0;
    printf("   Book Title\t\tAuthor Name \t\t ISBN \t\t\t Category \t\t Publisher\t\tPublication date \t Copies registered \t Copies Available \n\n");
    for(i=0;i<LastBook-1;i++)
    {
        printf("%d %s \t %s \t %s \t %15s \t %30s \t %d/%d/%d \t\t %d \t\t %d \n",i+1,BooksArray[i].Title,BooksArray[i].AuthorName,BooksArray[i].ISBN,BooksArray[i].Category,BooksArray[i].Publisher,BooksArray[i].Publication.Day,BooksArray[i].Publication.Month,BooksArray[i].Publication.Year,BooksArray[i].copies,BooksArray[i].copies_available);
    }
    printf("\n");
    printf("\n\n");
    printf("Press Any Key to Continue\n");
    getch();
}

void SearchBook()
{
    system("cls");
    int i,j=0,k=0,l=0,found=0;
    char *strpart,Auth[70],Title[70];
    Book temp;
    Book SearchArray1[100], SearchArray2[100], SearchArray3[100];
    for(i=0;i>100;i++)
    {
        SearchArray1[i].copies=0;
        SearchArray2[i].copies=0;
        SearchArray3[i].copies=0;
    }
    printf("Please choose an action (1-3): \n");
    printf(" 1- Specific Search by ISBN\n");
    printf(" 2- Search by Title, Author and/or Category\n");
    printf(" 3- Back\n");
    do{
        scanf("%d",&input8);
        if(input8>3 || input8<1 || CheckInt(input8)==0)
        printf("Please enter an integer (1-3)!\n");
        else{
        switch(input8)
        {
            case 1:

                do{
                printf("Please enter an ISBN (ex: 978-3-16-148410-0)\n");
                fgetc(stdin);
                fgets(temp.ISBN,sizeof(temp.ISBN),stdin);
                temp.ISBN[strlen(temp.ISBN) - 1] = 0;
                if(CheckISBN(temp.ISBN)==0)
                printf("Please enter an IBSN! (ex: 978-3-16-148410-0) \n");
                }while(CheckISBN(temp.ISBN)==0);
                for(i=0;i<LastBook;i++)
                {
                    if(strcmp(temp.ISBN,BooksArray[i].ISBN)==0)
                        {
                            found=1;
                            printf("\nSearch Results: \n\n");
                            printf("Book Title \t\t Author Name \t ISBN \t\t\t Category \n");
                            printf("%s \t %s \t %s \t %s \n",BooksArray[i].Title,BooksArray[i].AuthorName,BooksArray[i].ISBN,BooksArray[i].Category);
                        }
                }
                if(found!=1)
                {
                    printf("This book isn't registered!\n\n");
                    printf("Press Any Key to Continue\n");
                    getch();
                    SearchBook();
                }
                break;
            case 2:
                printf("\nEnter Book title or leave it blank to skip:");
                fgetc(stdin);
                fgets(temp.Title,sizeof(temp.Title),stdin);
                temp.Title[strlen(temp.Title) - 1] = 0;
                printf("\nEnter Author name or leave it blank to skip:");
                fgets(temp.AuthorName,sizeof(temp.AuthorName),stdin);
                temp.AuthorName[strlen(temp.AuthorName) - 1] = 0;
                printf("\nEnter category or leave it blank to skip:");
                fgets(temp.Category,sizeof(temp.Category),stdin);
                temp.Category[strlen(temp.Category) - 1] = 0;
                if(temp.Category[0]!=NULL)
                {
                    for(i=0;i<LastBook;i++)
                    {
                        strpart=strcmpi(temp.Category,BooksArray[i].Category);
                        if(strpart==0)
                        {
                            strcpy(SearchArray1[j].ISBN,BooksArray[i].ISBN);
                            strcpy(SearchArray1[j].Title,BooksArray[i].Title);
                            strcpy(SearchArray1[j].Category,BooksArray[i].Category);
                            strcpy(SearchArray1[j].AuthorName,BooksArray[i].AuthorName);
                            j++;
                        }
                    }
                }else{
                    for(i=0;i<LastBook;i++)
                    {
                        strcpy(SearchArray1[i].ISBN,BooksArray[i].ISBN);
                        strcpy(SearchArray1[i].Title,BooksArray[i].Title);
                        strcpy(SearchArray1[i].Category,BooksArray[i].Category);
                        strcpy(SearchArray1[i].AuthorName,BooksArray[i].AuthorName);
                        j++;
                    }
                }
                for(i=0;i<j;i++)
                if(temp.Title[0]!=NULL)
                {
                    for(i=0;i<j;i++)
                    {
                        strcpy(Title,SearchArray1[i].Title);
                        strpart=strstr(strlwr(Title),strlwr(temp.Title));
                        if(strpart!=NULL)
                        {
                            strcpy(SearchArray2[k].ISBN,SearchArray1[i].ISBN);
                            strcpy(SearchArray2[k].Title,SearchArray1[i].Title);
                            strcpy(SearchArray2[k].Category,SearchArray1[i].Category);
                            strcpy(SearchArray2[k].AuthorName,SearchArray1[i].AuthorName);
                            k++;
                        }
                    }

                }else{
                    for(i=0;i<j;i++)
                    {
                        strcpy(SearchArray2[i].ISBN,SearchArray1[i].ISBN);
                        strcpy(SearchArray2[i].Title,SearchArray1[i].Title);
                        strcpy(SearchArray2[i].Category,SearchArray1[i].Category);
                        strcpy(SearchArray2[i].AuthorName,SearchArray1[i].AuthorName);
                        k++;
                    }
                }
                if(temp.AuthorName[0]!=NULL)
                {
                    for(i=0;i<k;i++)
                    {
                        strcpy(Auth,SearchArray1[i].AuthorName);
                        strpart=strstr(strlwr(Auth),strlwr(temp.AuthorName));
                        if(strpart!=NULL)
                        {
                            strcpy(SearchArray3[l].ISBN,SearchArray2[i].ISBN);
                            strcpy(SearchArray3[l].Title,SearchArray2[i].Title);
                            strcpy(SearchArray3[l].Category,SearchArray2[i].Category);
                            strcpy(SearchArray3[l].AuthorName,SearchArray2[i].AuthorName);
                            SearchArray3[l]=SearchArray2[i];
                            l++;
                        }
                    }
                }else
                {
                    for(i=0;i<k;i++)
                    {
                        strcpy(SearchArray3[i].ISBN,SearchArray2[i].ISBN);
                        strcpy(SearchArray3[i].Title,SearchArray2[i].Title);
                        strcpy(SearchArray3[i].Category,SearchArray2[i].Category);
                        strcpy(SearchArray3[i].AuthorName,SearchArray2[i].AuthorName);
                        l++;
                    }
                }
                if(temp.AuthorName[0]==NULL && temp.Category[0]==NULL && temp.Title[0]==NULL)
                    printf("\nNo matches found. Please check your spelling or try a different search term!\n");
                else{
                        printf("\nSearch Results: \n");
                        if(SearchArray3[0].Title[0]==NULL)
                            printf("\nNo matches found. Please check your spelling or try a different search term!\n");
                        else
                        {
                            printf("Book Title \t\t Author Name \t ISBN \t\t\t Category \n");
                            for(i=0;i<l;i++)
                            {
                                printf("%s \t %s \t %s \t %s\n",SearchArray3[i].Title,SearchArray3[i].AuthorName,SearchArray3[i].ISBN,SearchArray3[i].Category);
                            }
                        }
                }
                break;
            case 3:
                BookManagement();
    }
    printf("\nPress Any Key to Continue\n");
    getch();
    }}while(input8>3 || input8<1 || CheckInt(input8)==0);
}

void Save()
{
    FILE *a,*b,*c;
    int i=0;
    a=fopen("Members.txt", "w");
    b=fopen("Books.txt", "w");
    c=fopen("Borrows.txt", "w");
    while(i<LastMember-1)
    {
        fprintf(a,"%d,%s,%s,%d,%s,%s,%d,%s,%ld\n",MembersArray[i].ID,MembersArray[i].LastName,MembersArray[i].Surname,MembersArray[i].Address.bldg,MembersArray[i].Address.street,MembersArray[i].Address.city,MembersArray[i].Age,MembersArray[i].mail,MembersArray[i].PhoneNumber);
        i++;
    }
    i=0;
    while(i<LastBook-1)
    {
        fprintf(b,"%d,%d,%d,%s,%s,%s,%s,%s,%d,%d\n",BooksArray[i].Publication.Day,BooksArray[i].Publication.Month,BooksArray[i].Publication.Year,BooksArray[i].ISBN,BooksArray[i].Title,BooksArray[i].AuthorName,BooksArray[i].Publisher,BooksArray[i].Category,BooksArray[i].copies,BooksArray[i].copies_available);
        i++;
    }
    i=0;
    while(i<LastBorrow-1)
    {
        fprintf(c,"%d,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n",BorrowsArray[i].Transaction,BorrowsArray[i].ISBN,BorrowsArray[i].ID,BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year,BorrowsArray[i].Dateissued.Day,BorrowsArray[i].Dateissued.Month,BorrowsArray[i].Dateissued.Year,BorrowsArray[i].return_date.Day,BorrowsArray[i].return_date.Month,BorrowsArray[i].return_date.Year);
        i++;
    }
    fclose(a);
    fclose(b);
    fclose(c);
    printf("Have no fear! Your data is in the bag!");
    printf("\n\n");
    printf("Press Any Key to Continue\n");
    getch();
    MainMenu();
}

void BookManagement()
{
    system("cls");
    printf("Please choose an action (1-5): \n");
    printf(" 1- Insert a new book\n");
    printf(" 2- Search for a book\n");
    printf(" 3- Add new copies\n");
    printf(" 4- Delete book\n");
    printf(" 5- Display all books data\n");
    printf(" 6- Back\n");
	do{
        scanf("%d",&input4);
        if(input4>6 || input4<1 || CheckInt(input4)==0)
        printf("Please enter an integer (1-5)!\n");
        else{
        switch(input4)
        {
            case 1:
                NewBook();
                break;						/* Calls a function through which a new book will be created in the Books File */
            case 2:
                SearchBook();               /* Calls a function that searches the books file for a book */
                break;
            case 3:
                AddCopies();					/* Calls a function that is used to add copies to an existing book in the books file */
                break;
            case 4:
                DeleteBook();					/* Calls a function that deletes a book's data from the books f */
                break;
            case 5:
                DisplayBooks();
                break;
            case 6:
                MainMenu();
        }}}while(input4>6 || input4<1 || CheckInt(input4)==0);
    MainMenu();
}

void MemberManagement()
{
    system("cls");
    printf("Please choose an action (1-3): \n");
		printf(" 1- Register a new member\n");
		printf(" 2- Delete an existing member\n");
		printf(" 3- Display all members data\n");
		printf(" 4- Back\n");
	do{
        scanf("%d",&input5);
        if(input5>4 || input5<1 || CheckInt(input5)==0)
        printf("Please enter an integer (1-3)!\n");
        else{
        switch(input5)
        {
            case 1:
                NewMember();
                break;
            case 2:
                DeleteMember();
                break;
            case 3:
                DisplayMembers();
                break;
            case 4:
                MainMenu();
                break;
	}}}while(input5>4 || input5<1 || CheckInt(input5)==0);
    MainMenu();
}

void BorrowManagement()
{
    system("cls");
    printf("Please choose an action (1-2): \n");
		printf(" 1- Register a new borrowing transaction\n");
		printf(" 2- Register the return of a book\n");
		printf(" 3- Display all borrowing transactions\n");
		printf(" 4- Back\n");
    do{
        scanf("%d",&input6);
        if(input6>4 || input6<1 || CheckInt(input6)==0)
        printf("Please enter an integer (1-3)!\n");
        else{
        switch(input6)
        {
            case 1:
                NewBorrow();
                break;
            case 2:
                ReturnBorrow();
                break;
            case 3:
                DisplayBorrowings();
                break;
            case 4:
                MainMenu();
                break;
	}}}while(input6>4 || input6<1 || CheckInt(input6)==0);
    MainMenu();
}

void AdminActions()
{
    system("cls");
    printf("Please choose an action (1-2): \n");
		printf(" 1- Display Overdue Books\n");
		printf(" 2- Display the most popular books\n");
		printf(" 3- Back\n");
    do{
        scanf("%d",&input7);
        if(input7>3 || input7<1 || CheckInt(input7)==0)
        printf("Please enter an integer (1-2)!\n");
        else{
        switch(input7)
        {
            case 1:
                OverdueBooks();
                break;
            case 2:
                PopBooks();
                break;
            case 3:
                MainMenu();
                break;
	}}}while(input7>3 || input7<1 || CheckInt(input7)==0);
	MainMenu();
}

void MainMenu()
{
    system("cls");
        printf("Date: %d/%d/%d ",DateTime.wDay,DateTime.wMonth,DateTime.wYear);
    if((DateTime.wHour+2)<12)
        printf("Current Time: %d:%d AM \n \n",(DateTime.wHour+2),DateTime.wMinute);
    else if((DateTime.wHour+2)==12)
        printf("Current Time: %d:%d PM \n \n",(DateTime.wHour+2),DateTime.wMinute);
    else if((DateTime.wHour+2)>24)
        printf("Current Time: %d:%d AM \n \n",(24-DateTime.wHour),DateTime.wMinute);
    else
        printf("Current Time: %d:%d PM \n \n",DateTime.wHour-10,DateTime.wMinute);
    printf("Please choose an action (1-6): \n");
    printf(" 1- Book Management\n");
    printf(" 2- Member Management\n");
    printf(" 3- Borrowing Management\n");
    printf(" 4- Administrative Actions\n");
    printf(" 5- Save\n");
    printf(" 6- Exit\n");

    do{
        scanf("%d",&input1);
        if((input1<1) || (input1>6) || (CheckInt(input1)==0))
    {
    printf("%d",input1);
    printf("Please enter an integer (1-6) that corresponds to your action of choice!\n");
    }
    else{
    switch(input1)
    {
        case 1:
            BookManagement(); /* Calls a function that manages books */
            break;
        case 2:
            MemberManagement(); /* Calls a function that manages members */
            break;
        case 3:
            BorrowManagement(); /* Calls a function that manages borrowings */
            break;
        case 4:
            AdminActions(); /* Calls a function that manages administrative actions */
            break;
        case 5:
            Save();
            break;
        case 6:
            printf("Please pick an action(1-3): \n");
                printf(" 1- Save and Exit\n");
                printf(" 2- Exit and discard changes\n");
                printf(" 3- Get back to the main menu\n");
                do{
                scanf("%d",&input2);
                if(input2<1 || input2>3 || CheckInt(input2)==0)
                    printf("Please enter an integer (1-3)!");
                else
                if(input2==1){
                    Save();
                    exit(0);
                }
                else if(input2==2){
                    printf("Are you sure you want to discard all changes?(Y/N)\n");
                    do{
                    scanf(" %c",&input3);
                    if(input3!= 'Y' && input3!='N' && input3!= 'y' && input3!='n')
                    printf("Please enter Y or N\n");
                    }while(input3!= 'Y' && input3!='N' && input3!= 'y' && input3!='n');
                    if(input3=='n' || 'N')
                    MainMenu();
                    else if(input3=='y' || 'Y')
                    exit(0);
                }
                else
                    MainMenu();
			}while(input2>3 || input2<1 || CheckInt(input2)==0);}
			break;
    }
}while(input1<1 || input1>6 || CheckInt(input1)==0);
}

int main()
{
    int i,flag=0;
    FILE *f,*j,*k;
    GetSystemTime(&DateTime);

    /*Loop to Zero all IDs in MembersArray*/
    for(i=0;i>100;i++)
        MembersArray[i].ID=0;
    /*Loop to Zero all ISBNs in BooksArray*/
    for(i=0;i>100;i++)
        BooksArray[i].copies=0;
    /*Loop to Zero all IDs in BorrowsArray*/
    for(i=0;i>100;i++)
        BorrowsArray[i].Transaction=0;
    f=fopen("Members.txt", "r");
    if (f != NULL)
    {
        while(flag==0){
         fscanf(f, "%d,%[^,],%[^,],%d,%[^,],%[^,],%d,%[^,],%ld",&MembersArray[c1].ID,MembersArray[c1].LastName,MembersArray[c1].Surname,&MembersArray[c1].Address.bldg,MembersArray[c1].Address.street,MembersArray[c1].Address.city,&MembersArray[c1].Age,MembersArray[c1].mail,&MembersArray[c1].PhoneNumber);
         fscanf(f, "\n");
         if(MembersArray[c1].ID==0)
            flag=1;
         c1++;
         LastMember++;
        }
         fclose(f);
    } else
        printf("Error opening the file!\n");
    flag=0;
    j=fopen("Books.txt", "r");
    if (j != NULL)
    {
        while(flag==0)
        {
         fscanf(j,"%d,%d,%d,%[^,],%[^,],%[^,],%[^,],%[^,],%d,%d",&BooksArray[c2].Publication.Day,&BooksArray[c2].Publication.Month,&BooksArray[c2].Publication.Year,BooksArray[c2].ISBN,BooksArray[c2].Title,BooksArray[c2].AuthorName,BooksArray[c2].Publisher,BooksArray[c2].Category,&BooksArray[c2].copies,&BooksArray[c2].copies_available);
         fscanf(j,"\n");
         if(BooksArray[c2].copies==0)
            flag=1;
         c2++;
         LastBook++;
        }
         fclose(j);
    } else
        printf("Error opening the file!\n");
    flag=0;
    k=fopen("Borrows.txt", "r");
    if (k != NULL)
    {
        while (flag==0)
        {
         fscanf(k,"%d,%[^,],%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",&BorrowsArray[c3].Transaction,BorrowsArray[c3].ISBN,&BorrowsArray[c3].ID,&BorrowsArray[c3].Dateissued.Day,&BorrowsArray[c3].Dateissued.Month,&BorrowsArray[c3].Dateissued.Year,&BorrowsArray[c3].Datedue.Day,&BorrowsArray[c3].Datedue.Month,&BorrowsArray[c3].Datedue.Year,&BorrowsArray[c3].return_date.Day,&BorrowsArray[c3].return_date.Month,&BorrowsArray[c3].return_date.Year);
         fscanf(k, "\n");
         if(BorrowsArray[c3].Transaction==0)
            flag=1;
         LastTrans++;
         c3++;
         LastBorrow++;
        }
         fclose(k);
    } else
        printf("Error opening the file!\n");
    PopBooks();
    MainMenu();
    return 0;
}
