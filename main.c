#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <strings.h>
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

int LastMember=0,LastBook=0,LastBorrow=0,LastTrans=0,input1,input2,input4,input5,input6, input7,input8;
char input3;
SYSTEMTIME DateTime;

char *stristr (char *str, char *strSearch) {
    char *sors, *subs, *res = NULL;
    if ((sors = strdup (str)) != NULL) {
        if ((subs = strdup (strSearch)) != NULL) {
            res = strstr (strlwr (sors), strlwr (subs));
            if (res != NULL)
                res = str + (res - sors);
            free (subs);
        }
        free (sors);
    }
    return res;
}

void MainMenu();

int CheckInt(int n)
{
    if(isdigit(n)!=1)
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
	else if(n.Year>DateTime.wYear || CheckInt(n.Year)==0)
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
    for(i=0;n[i]!='\0';i++)
    {
        if(CheckInt(n[i]!=0))
            digcount++;
    }
    if(digcount<13 || digcount>13)
        return 0;
    else return 1;
}
int CheckID(int n)
{
    if(n/10000!=0 || n<1000 || isdigit(n)==0)
        return 0;
    else
        return 1;
}

void ODbooks()
{
    int i=0;
    printf("Transaction# \t ID \t ISBN \t Date due");
    for(i=0;i<=LastBorrow;i++)
    {
        if (BorrowsArray[i].Datedue.Year>DateTime.wYear)
            printf("%d \t %d \t %s \t %d/%d/%d",BorrowsArray[i].Transaction, BorrowsArray[i].ID,BorrowsArray[i].ISBN, BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
        else if((BorrowsArray[i].Datedue.Year=DateTime.wYear) && (BorrowsArray[i].Datedue.Month>DateTime.wMonth))
            printf("%d \t %d \t %s \t %d/%d/%d",BorrowsArray[i].Transaction, BorrowsArray[i].ID,BorrowsArray[i].ISBN, BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
        else if((BorrowsArray[i].Datedue.Year=DateTime.wYear) && (BorrowsArray[i].Datedue.Month>DateTime.wMonth) && (BorrowsArray[i].Datedue.Day>DateTime.wDay));
            printf("%d \t %d \t %s \t %d/%d/%d",BorrowsArray[i].Transaction, BorrowsArray[i].ID,BorrowsArray[i].ISBN, BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
    }

}

void PopBooks()
{
    BorrowedBook SearchArray[100];
    BorrowedBook temp;
    int i=0,j=0,k=0,Last=0;

    for(j=0;j<100;j++)
    {
        SearchArray[j].occur=0;
    }

    for(i=0;i<=LastBorrow;i++)
    {

        for(k=0;k<100;k++)
        {
            if(strcmp(BorrowsArray[i].ISBN,SearchArray[k].ISBN)==0)
            SearchArray[k].occur++;
            else
            {
                strcpy(SearchArray[Last].ISBN,BorrowsArray[i].ISBN);
                SearchArray[Last].occur++;
                Last++;
            }
        }
    }
    for(i=0;i<=Last;i++)
    {
        for(j=0;j<=LastBook;j++)
        {
            if(strcmp(BorrowsArray[i].ISBN,SearchArray[j].ISBN)==0)
                strcpy(SearchArray[i].title,BooksArray[j].Title);
        }
    }
    for(i=0;i<Last++;i++)
    {
        for(j=0;j<=Last;j++)
        {
            if(SearchArray[i].occur<SearchArray[j].occur)
                strcpy(temp.ISBN,SearchArray[i].ISBN);
                strcpy(temp.title,SearchArray[i].title);
                temp.occur=SearchArray[i].occur;

                SearchArray[i]=SearchArray[j];

                strcpy(SearchArray[i].ISBN,SearchArray[j].ISBN);
                strcpy(SearchArray[i].title,SearchArray[j].title);
                SearchArray[i].occur=SearchArray[j].occur;

                SearchArray[j]=temp;

                strcpy(SearchArray[j].ISBN,temp.ISBN);
                strcpy(SearchArray[j].title,temp.title);
                SearchArray[j].occur=temp.occur;
        }
    }
    printf("Title \t ISBN \t Times Borrowed");
    for(i=0;i<=5;i++)
    {
        printf("%s \t %s \t %d",SearchArray[i].title,SearchArray[i].ISBN,SearchArray[i].occur);
    }
}

void NewBorrow()
{
    int checkid,checkdate,i=0,found=0,flag=0;
    int found1=0;
    Borrow temp;
    printf("Please enter the ID of the borrowing member:\n");
    scanf("%d",&temp.ID);
    checkid=CheckID(temp.ID);
    if(checkid==0)
        printf("Please enter a legitimate ID! ID numbers start from 1001 and end at 9999.\n");
    for(i=0;(found==0);i++)
    {
        if(MembersArray[i].ID==temp.ID)
            found=1;
    }
    if(found==0)
        printf("This member ID isn't registered in the system!\n");
    else
    {
        for(i=0;i<LastBorrow;i++)
        {
            if(BorrowsArray[i].ID==temp.ID)
                flag++;
        }
        if(flag==3)
        {
            printf("This member can't borrow any more books as they have 3 unreturned books!\n");
            printf("returning to main menu ... \n");
            MainMenu();
        }
        else{
        printf("Please enter the ISBN of the book to be borrowed:\n");
        fgets(temp.ISBN,sizeof(temp.ISBN),stdin);
        if(CheckISBN(temp.ISBN)==0)
            printf("Please enter a legitimate ISBN!(ex: 978-3-16-148410-0)\n");
        for(i=0;(found1==0);i++)
        {
            if(strcmp(BooksArray[i].ISBN,temp.ISBN))
            {
                found1=1;
                if(BooksArray[i].copies_available>0)
                BooksArray[i].copies_available--;
                else
                {
                    printf("There are no more copies available.\n");
                    printf("returning to main menu ... \n");
                    MainMenu();
                }
            }
        }
        if(found1==0)
            printf("This book ISBN isn't registered in the system!\n");
        else
        {
            BorrowsArray[LastBorrow+1].Transaction=BorrowsArray[LastBorrow].Transaction+1;
            do{
                printf("Please enter Date issued. (DD, enter, MM, enter, YY, enter) \n");
                scanf("%d %d %d",&temp.Dateissued.Day,&temp.Dateissued.Month,&temp.Dateissued.Year);
                checkdate=CheckDate(temp.Dateissued);
                if(checkdate==0)
                    printf("Please enter a date that is not later than today! \n");
        }while(checkdate==0);
            do{
                printf("Please enter Date due to return. (DD, enter, MM, enter, YY, enter) \n");
                scanf("%d %d %d",&temp.Datedue.Day,&temp.Datedue.Month,&temp.Datedue.Year);
                checkdate=CheckDate(temp.Dateissued);
                if(checkdate==0)
                    printf("Please enter a date that is not later than today!\n");
        if(temp.Dateissued.Year>temp.Datedue.Year)
        {
            printf("Date entered can't be after today's date!\n");
            checkdate=0;
        }
        else if((temp.Dateissued.Year=temp.Datedue.Year) && (temp.Dateissued.Month>temp.Datedue.Month))
        {
            printf("Date entered can't be after today's date!");
            checkdate=0;
        }
        else if((temp.Dateissued.Year=temp.Datedue.Year) && (temp.Dateissued.Month<=temp.Datedue.Month) && (temp.Dateissued.Day>temp.Datedue.Day))
        {
            printf("Date entered can't be after today's date!");
            checkdate=0;
        }
        }while(checkdate==0);
        }}}
        BorrowsArray[LastBorrow].Transaction=LastTrans-1;
        BorrowsArray[LastBorrow].ID=temp.ID;
        BorrowsArray[LastBorrow].Dateissued.Day=temp.Dateissued.Day;
        BorrowsArray[LastBorrow].Dateissued.Month=temp.Dateissued.Month;
        BorrowsArray[LastBorrow].Dateissued.Year=temp.Dateissued.Year;
        BorrowsArray[LastBorrow].Datedue.Day=temp.Datedue.Day;
        BorrowsArray[LastBorrow].Datedue.Month=temp.Datedue.Month;
        BorrowsArray[LastBorrow].Datedue.Year=temp.Datedue.Year;
        strcpy(BorrowsArray[LastBorrow].ISBN,temp.ISBN);
        printf("Borrow transaction added successfully!");
        LastBorrow++;
}

void ReturnBorrow()
{
    Date temp;
    int Trans,i=0,found=0,checkdate=1;
    do{
    printf("Please enter Transaction number");
    scanf("%d",&Trans);
    while(found==0)
    {
        if(BorrowsArray[i].Transaction==Trans)
            found=1;
        i++;
    }
    if(found==0)
        printf("This transaction number is not registered. Try again!");
    }while(found==0);
    do{
            printf("Please enter Date of return. (DD, enter, MM, enter, YY, enter) \n");
            scanf("%d %d %d",&temp.Day,&temp.Month,&temp.Year);
            checkdate=CheckDate(temp);
            if(checkdate==0)
                printf("Please enter a date that is not later than today! \n");
        }while(checkdate==0);
    printf("Book's return was registered successfully!\n");
}

void DisplayMembers()
{
    int i=0;
    printf("\t Surname \t Last Name \t ID \t Email \n");
    for(i=0;i<LastBook;i++)
    {
        printf("%d \t %s \t %s \t %d \t %s \n",i+1,MembersArray[i].Surname,MembersArray[i].LastName,MembersArray[i].ID,MembersArray[i].mail);
    }
}

void DisplayBorrowings()
{

int i=0;
    printf("Transaction \t ISBN \t ID of Borrower \t Issued Date \t Due Date\n");
    for(i=0;i<LastBorrow;i++)
    {
        printf("%d \t %s \t %d \t %d/%d/%d \t %d/%d/%d\n",BorrowsArray[i].Transaction,BorrowsArray[i].ISBN,BorrowsArray[i].ID,BorrowsArray[i].Dateissued.Day,BorrowsArray[i].Dateissued.Month,BorrowsArray[i].Dateissued.Year,BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
    }
}

void DeleteMember()
{
int id,checkid,i,found=0;
    printf("Please enter the ID of the member to be deleted:\n");
    scanf("%d",&id);
    checkid=CheckID(id);
    if(checkid==0)
        printf("Please enter a legitimate ID! ID numbers start from 1001 and end at 9999.");
    for(i=0;(found==0);i++)
    {
        if(MembersArray[i].ID==id)
            found=1;
    }
    if(found==0)
        printf("This member ID isn't registered in the system!");
    else
    {
        for(i=0;i>99;i++)
        {
            MembersArray[i].ID=MembersArray[i+1].ID;
            MembersArray[i].Age=MembersArray[i+1].Age;
            MembersArray[i].PhoneNumber=MembersArray[i+1].PhoneNumber;
            MembersArray[i].Address.bldg=MembersArray[i+1].Address.bldg;
            strcpy(MembersArray[i].LastName,MembersArray[i+1].LastName);
            strcpy(MembersArray[i].Surname,MembersArray[i+1].Surname);
            strcpy(MembersArray[i].mail,MembersArray[i+1].mail);
            strcpy(MembersArray[i].Address.city,MembersArray[i+1].Address.city);
            strcpy(MembersArray[i].Address.street,MembersArray[i+1].Address.street);

        }
    }
    printf("Member data has been deleted successfully!");
    LastMember--;
}

void NewMember()
{
    system("cls");
    Member temp;
    int i=0;
    int checkid;
    printf("Please enter the surname:");
    fgets(temp.Surname,sizeof(temp.Surname),stdin);
    printf("\nPlease enter the last name:");
    fgets(temp.LastName,sizeof(temp.LastName),stdin);
    printf("\nPlease enter the Email:");
    fgets(temp.mail,sizeof(temp.mail),stdin);
    do{
    printf("\nEnter the member ID:");
    scanf("%d",&temp.ID);
    checkid=CheckID(temp.ID);
    if(checkid==0)
        printf("\nPlease enter a legitimate ID! ID numbers start from 1001 and end at 9999.");
    for(i=0;i<=LastMember;i++)
    {
        if(temp.ID==MembersArray[i].ID)
        {
            checkid=0;
            printf("This member is already registered in the system!");
        }
    }
    printf("\nPlease enter the member's city of residence:");
    fgets(temp.Address.city,sizeof(temp.Address.city),stdin);
    printf("\nPlease enter the member house's street:");
    fgets(temp.Address.street,sizeof(temp.Address.street),stdin);
    printf("\nPlease enter the member house's building number:");
    scanf("%d",&temp.Address.bldg);
    }while(checkid==0);
    do{
    printf("\nEnter the member's age");
    scanf("%d",&temp.Age);;
    if(CheckInt(temp.Age)==0)
        printf("\nPlease enter an integer!");
    }while(CheckInt(temp.Age)==0);
    do{
    printf("\nEnter the member's phone number");
    scanf("%ld",&temp.PhoneNumber);
    if(CheckInt(temp.PhoneNumber)==0)
        printf("\nPlease enter an integer!");
    }while(CheckInt(temp.Age)==0);
    printf("\nMember added successfully!");

    MembersArray[LastMember].ID=temp.ID;
    MembersArray[LastMember].PhoneNumber=temp.PhoneNumber;
    MembersArray[LastMember].Age=temp.Age;
    MembersArray[LastMember].Address.bldg=temp.Address.bldg;
    strcpy(MembersArray[LastMember].Surname,temp.Surname);
    strcpy(MembersArray[LastMember].LastName,temp.LastName);
    strcpy(MembersArray[LastMember].mail,temp.mail);
    strcpy(MembersArray[LastMember].Address.city,temp.Address.city);
    strcpy(MembersArray[LastMember].Address.street,temp.Address.street);
    LastMember++;
}

void DisplayBooks()
{
    int i=0;
    printf("\t Book Title \t Author Name \t ISBN \t Category \n");
    for(i=0;i<LastBook;i++)
    {
        printf("%d \t %s \t %s \t %s \t %s \n",i+1,BooksArray[i].Title,BooksArray[i].AuthorName,BooksArray[i].ISBN,BooksArray[i].Category);
    }
}

void AddCopies()
{
    int i,flag=0,n;
    Book temp;
    printf("Please enter book ISBN");
    fgets(temp.ISBN,sizeof(temp.ISBN),stdin);
    for(i=0;(flag==1);i++){
        if(strcmp(BooksArray[i].ISBN,temp.ISBN)==0)
        {
            flag=1;
            printf("Book wanted is %s \n",BooksArray[i].Title);
        }
    }
    printf("How many copies do you want added:");
    scanf("%d",&n);
    BooksArray[i].copies+=n;
    BooksArray[i].copies_available+=n;
    printf("Copies have been added successfully!");
    }

void DeleteBook()
{
    Book temp;
    system("cls");
    int i,found=0;
    printf("Please enter the ISBN of the book to be deleted: (ex: 978-3-16-148410-0)\n");
    fgets(temp.ISBN,sizeof(temp.ISBN),stdin);
    if(CheckISBN(temp.ISBN)==0)
        printf("Please enter a legitimate ISBN!(ex: 978-3-16-148410-0)");
    for(i=0;(found==0);i++)
    {
        if(strcmp(BooksArray[i].ISBN,temp.ISBN)==0)
            found=1;
    }
    if(found==0)
        printf("This book ISBN isn't registered in the system!");
    else
    {
        for(i=0;i>99;i++)
        {

            BooksArray[i]=BooksArray[i+1];
        }
    }
    printf("Book data has been deleted successfully!");
    LastBook--;
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
    printf("\nPlease enter the author's name:");
    fgets(temp.AuthorName,sizeof(temp.Title),stdin);
    printf("\nPlease enter the publisher's name:");
    fgets(temp.Publisher,sizeof(temp.Publisher),stdin);
    do{
    printf("\nPlease enter Date Published:(DD, enter, MM, enter, YY, enter)");
    scanf("%d %d %d",&temp.Publication.Day,&temp.Publication.Month,&temp.Publication.Year);
    checkdate=CheckDate(temp.Publication);
    if(checkdate==0)
        printf("\nPlease enter a date that is not later than today!");
    }while(checkdate==0);
    do{
    printf("\nEnter the book ISBN (ex: 978-3-16-148410-0):");
    fgets(temp.ISBN,sizeof(temp.ISBN),stdin);
    if(CheckISBN(temp.ISBN)==0)
        printf("\nPlease enter a legitimate ISBN! (ex: 978-3-16-148410-0)");
    for(i=0;i<=LastBook;i++)
    {
        if(temp.ISBN==BooksArray[i].ISBN)
        {
            flag=1;
            printf("This book ISBN is already registered in the system. You can add new copies or enter another ISBN!");
        }
    }
    }while(CheckISBN(temp.ISBN)==0 || flag==1);
    do{
    printf("Enter the number of copies:\n");
    scanf("%d",&temp.copies);
    if(CheckInt(temp.copies)==0)
        printf("Please enter an integer! \n");
    }while(CheckInt(temp.copies)==0);
    temp.copies_available=temp.copies;
    printf("Enter the book category:\n");
    fgets(temp.Category,sizeof(temp.Category),stdin);

    BooksArray[LastBook].copies=temp.copies;
    BooksArray[LastBook].copies_available=temp.copies_available;
    BooksArray[LastBook].Publication.Day=temp.Publication.Day;
    BooksArray[LastBook].Publication.Month=temp.Publication.Month;
    BooksArray[LastBook].Publication.Year=temp.Publication.Year;
    strcpy(BooksArray[LastBook].ISBN,temp.ISBN);
    strcpy(BooksArray[LastBook].Title,temp.Title);
    strcpy(BooksArray[LastBook].AuthorName,temp.AuthorName);
    strcpy(BooksArray[LastBook].Category,temp.Category);
    strcpy(BooksArray[LastBook].Publisher,temp.Publisher);

    printf("\nBook Successfully added ");
    LastBook++;
}

void SearchBook()
{
    system("cls");
    int i,j=0,k=0,l=0,found=0,strequal;
    char *strpart;
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
    do{
        scanf("%d",&input8);
        if(input8>2 || input8<1 || CheckInt(input8)==0)
        printf("Please enter an integer (1-2)!\n");
        else{
        switch(input8)
        {
            case 1:

                do{
                printf("Please enter an ISBN (ex: 978-3-16-148410-0)");
                fgetc(stdin);
                fgets(temp.ISBN,sizeof(temp.ISBN),stdin);
                if(CheckISBN(temp.ISBN)==0)
                printf("Please enter an IBSN! (ex: 978-3-16-148410-0) \n");
                }while(CheckISBN(temp.ISBN)==0);
                for(i=0;i>LastBook;i++)
                {
                    if(strcmp(temp.ISBN,BooksArray[i].ISBN)==0)
                        {
                            found=1;
                            printf("Search Results: \n");
                            printf("Book Title \t Author Name \t ISBN \t Category \n");
                            printf("%s \t %s \t %s \t %s \n",BooksArray[i].Title,BooksArray[i].AuthorName,BooksArray[i].ISBN,BooksArray[i].Category);
                        }
                }
                if(found!=1)
                {
                    printf("This book isn't registered!\n");
                    printf("Returning to the book searching menu ...\n");
                    SearchBook();
                }
            case 2:
                printf("Enter Book title or leave it blank to skip");
                fgets(temp.Title,sizeof(temp.Title),stdin);
                printf("Enter Author name or leave it blank to skip");
                fgets(temp.AuthorName,sizeof(temp.AuthorName),stdin);
                printf("Enter category or leave it blank to skip");
                fgets(temp.Category,sizeof(temp.Category),stdin);
                if(temp.Category!='\0')
                {
                    for(i=0;i<=LastBook;i++)
                    {
                        strequal=stricmp(temp.Category,BooksArray[i].Category);
                        if(strequal==0)
                        {
                            strcpy(SearchArray1[j].ISBN,BooksArray[i].ISBN);
                            strcpy(SearchArray1[j].Title,BooksArray[i].Title);
                            strcpy(SearchArray1[j].Category,BooksArray[i].Category);
                            strcpy(SearchArray1[j].AuthorName,BooksArray[i].AuthorName);
                            j++;
                        }
                    }
                }else{
                    for(i=0;i<=LastBook;i++)
                    {
                        strcpy(SearchArray1[i].ISBN,BooksArray[i].ISBN);
                        strcpy(SearchArray1[i].Title,BooksArray[i].Title);
                        strcpy(SearchArray1[i].Category,BooksArray[i].Category);
                        strcpy(SearchArray1[i].AuthorName,BooksArray[i].AuthorName);
                    }
                }
                if(temp.Title!='\0')
                {
                    for(i=0;i<=j;i++)
                    {
                        strpart=stristr(SearchArray1[i].Title,temp.Title);
                        if(strpart!='\0')
                        {
                            strcpy(SearchArray2[k].ISBN,SearchArray1[i].ISBN);
                            strcpy(SearchArray2[k].Title,SearchArray1[i].Title);
                            strcpy(SearchArray2[k].Category,SearchArray1[i].Category);
                            strcpy(SearchArray2[k].AuthorName,SearchArray1[i].AuthorName);
                            k++;
                        }
                    }

                }else{
                    for(i=0;i<100;i++)
                    {
                        strcpy(SearchArray2[i].ISBN,SearchArray1[i].ISBN);
                        strcpy(SearchArray2[i].Title,SearchArray1[i].Title);
                        strcpy(SearchArray2[i].Category,SearchArray1[i].Category);
                        strcpy(SearchArray2[i].AuthorName,SearchArray1[i].AuthorName);
                    }
                }
                if(temp.AuthorName!='\0')
                {
                        for(i=0;i<=k;i++)
                    {
                        strequal=stricmp(SearchArray2[i].AuthorName,temp.AuthorName);
                        if(strequal==0)
                        {
                            strcpy(SearchArray3[l].ISBN,SearchArray2[i].ISBN);
                            strcpy(SearchArray3[l].Title,SearchArray2[i].Title);
                            strcpy(SearchArray3[l].Category,SearchArray2[i].Category);
                            strcpy(SearchArray3[l].AuthorName,SearchArray2[i].AuthorName);
                            SearchArray3[l]=SearchArray2[i];
                            l++;
                        }
                    }
                }else{
                    for(i=0;i<100;i++)
                    {
                        strcpy(SearchArray3[i].ISBN,SearchArray2[i].ISBN);
                        strcpy(SearchArray3[i].Title,SearchArray2[i].Title);
                        strcpy(SearchArray3[i].Category,SearchArray2[i].Category);
                        strcpy(SearchArray3[i].AuthorName,SearchArray2[i].AuthorName);
                    }
                }
                if(temp.AuthorName=='\0' && temp.Category=='\0' && temp.Title=='\0')
                    printf("No matches found. Please check your spelling or try a different search term!");
                else{
                        printf("Search Results: \n");
                        printf("Book Title \t Author Name \t ISBN \t Category \n");
                        for(i=0;i<=l;i++)
                        {
                            printf("%s \t %s \t %s \t %s\n",SearchArray3[i].Title,SearchArray3[i].AuthorName,SearchArray3[i].ISBN,SearchArray3[i].Category);
                        }
                }
    }}}while(input8>2 || input8<1 || CheckInt(input8)==0);
}

void Save()
{
    system("cls");
    FILE *a,*b,*c;
    int i=0;
    a=fopen("Members.txt", "w");
    b=fopen("Books.txt", "w");
    c=fopen("Borrows.txt", "w");
    while(MembersArray[i].ID>0)
    {
        fprintf(a,"%d,%s,%s,%d,%s,%s,%d,%s,%ld\n",MembersArray[i].ID,MembersArray[i].LastName,MembersArray[i].Surname,MembersArray[i].Address.bldg,MembersArray[i].Address.street,MembersArray[i].Address.city,MembersArray[i].Age,MembersArray[i].mail,MembersArray[i].PhoneNumber);
        i++;
    }
    i=0;
    while(BooksArray[i].ISBN>0)
    {
        fprintf(b,"%d,%d,%d,%s,%s,%s,%s,%s,%d,%d\n",BooksArray[i].Publication.Day,BooksArray[i].Publication.Month,BooksArray[i].Publication.Year,BooksArray[i].ISBN,BooksArray[i].Title,BooksArray[i].AuthorName,BooksArray[i].Publisher,BooksArray[i].Category,BooksArray[i].copies,BooksArray[i].copies_available);
        i++;
    }
    i=0;
    while(BorrowsArray[i].ISBN>0)
    {
        fprintf(c,"%d,%s,%d,%d,%d,%d,%d,%d,%d\n",BorrowsArray[i].Transaction,BorrowsArray[i].ISBN,BorrowsArray[i].ID,BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year,BorrowsArray[i].Dateissued.Day,BorrowsArray[i].Dateissued.Month,BorrowsArray[i].Dateissued.Year);
        i++;
    }
    fclose(a);
    fclose(b);
    fclose(c);
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
	do{
        scanf("%d",&input4);
        if(input4>5 || input4<1 || CheckInt(input4)==0)
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
        }}}while(input4>5 || input4<1 || CheckInt(input4)==0);
    printf("Returning back to main menu ...\n");
    MainMenu();
}

void MemberManagement()
{
    system("cls");
    printf("Please choose an action (1-3): \n");
		printf(" 1- Register a new members\n");
		printf(" 2- Delete an existing member\n");
		printf(" 3- Display all members data\n");
	do{
        scanf("%d",&input5);
        if(input5>3 || input5<1 || CheckInt(input5)==0)
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
	}}}while(input5>3 || input5<1 || CheckInt(input5)==0);
    printf("Returning back to main menu ...\n");
    MainMenu();
}

void BorrowManagement()
{
    system("cls");
    printf("Please choose an action (1-2): \n");
		printf(" 1- Register a new borrowing transaction\n");
		printf(" 2- Register the return of a book\n");
		printf(" 3- Display all borrowing transactions\n");
    do{
        scanf("%d",&input6);
        if(input6>3 || input6<1 || CheckInt(input6)==0)
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
	}}}while(input6>3 || input6<1 || CheckInt(input6)==0);
    printf("Returning back to main menu ...\n");
    MainMenu();
}

void AdminActions()
{
    system("cls");
    printf("Please choose an action (1-2): \n");
		printf(" 1- Display Overdue Books\n");
		printf(" 2- Display the most popular books\n");
    do{
        scanf("%d",&input7);
        if(input7>3 || input7<1 || CheckInt(input7)==0)
        printf("Please enter an integer (1-2)!\n");
        else{
        switch(input6)
        {
            case 1:
                ODbooks();
                break;
            case 2:
                PopBooks();
                break;
	}}}while(input7>3 || input7<1 || CheckInt(input7)==0);
	printf("Returning back to main menu ...");
}

void MainMenu()
{

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
                    printf("Are you sure you want to discard all changes?(Y/N)");
                    do{
                    scanf("%c",&input3);
                    if(input3!= 'Y' && input3!='N' && input3!= 'y' && input3!='n')
                    printf("Please enter Y or N");
                    }while(input3!= 'Y' && input3!='N' && input3!= 'y' && input3!='n');
                    if(input3=='n' || 'N')
                    MainMenu();
                    else
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

    int c1=0,c2=0,c3=0,i,flag=0;
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
    i=c2;
    k=fopen("Borrows.txt", "r");
    if (k != NULL)
    {
        while (flag==0)
        {
         fscanf(k, "%d,%[^,],%d,%d,%d,%d,%d,%d,%d",&BorrowsArray[c3].Transaction,BorrowsArray[c3].ISBN,&BorrowsArray[c3].ID,&BorrowsArray[c3].Datedue.Day,&BorrowsArray[c3].Datedue.Month,&BorrowsArray[c3].Datedue.Year,&BorrowsArray[c3].Dateissued.Day,&BorrowsArray[c3].Dateissued.Month,&BorrowsArray[c3].Dateissued.Year);
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
    MainMenu();
}