#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include <strings.h>

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
    char Surname[30];
    char LastName[30];
    int ID;
    Address Address;
    int PhoneNumber;
    int Age;
    char mail[50];
}Member;

typedef struct
{
	char Title[70];
	char AuthorName[70];
	char Publisher[70];
	int ISBN;
	Date Publication;
	int copies;
	int copies_available;
	char Category[30];
}Book;

typedef struct
{
	int Transaction;
	int ISBN;
	int ID;
	Date Dateissued;
	Date Datedue;
	Date return_date;
}Borrow;

typedef struct
{
	int ISBN;
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

int CheckDate(Date n)
{
    if(n.Month>12 || n.Month<0)
    return 0;
	if(n.Day>31 || n.Day<0)
	return 0;
	if(n.Year>DateTime.wYear)
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
int CheckISBN(int n)
{
    int a;
    a=isdigit(n);
    if(n/10000000000000!=0 || n/1000000000000==0 || a==0)
    {
        return 0;
    }else{
    return 1;
    }
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
            printf("%d \t %d \t %d \t %d/%d/%d",BorrowsArray[i].Transaction, BorrowsArray[i].ID,BorrowsArray[i].ISBN, BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
        else if((BorrowsArray[i].Datedue.Year=DateTime.wYear) && (BorrowsArray[i].Datedue.Month>DateTime.wMonth))
            printf("%d \t %d \t %d \t %d/%d/%d",BorrowsArray[i].Transaction, BorrowsArray[i].ID,BorrowsArray[i].ISBN, BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
        else if((BorrowsArray[i].Datedue.Year=DateTime.wYear) && (BorrowsArray[i].Datedue.Month>DateTime.wMonth) && (BorrowsArray[i].Datedue.Day>DateTime.wDay));
            printf("%d \t %d \t %d \t %d/%d/%d",BorrowsArray[i].Transaction, BorrowsArray[i].ID,BorrowsArray[i].ISBN, BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
    }

}

void PopBooks()
{
    BorrowedBook SearchArray[100];
    BorrowedBook temp;
    int i=0,j=0,k=0,Last=0;

    for(j=0;j<100;j++)
    {
        SearchArray[j].ISBN=0;
        SearchArray[j].occur=0;
    }

    for(i=0;i<=LastBorrow;i++)
    {

        for(k=0;k<100;k++)
        {
            if(BorrowsArray[i].ISBN==SearchArray[k].ISBN)
            SearchArray[k].occur++;
            else
            {
                SearchArray[Last].ISBN=BorrowsArray[i].ISBN;
                SearchArray[Last].occur++;
                Last++;
            }
        }
    }
    for(i=0;i<=Last;i++)
    {
        for(j=0;j<=LastBook;j++)
        {
            if(SearchArray[i].ISBN==BooksArray[j].ISBN)
                strcpy(SearchArray[i].title,BooksArray[j].Title);
        }
    }
    for(i=0;i<Last++;i++)
    {
        for(j=0;j<=Last;j++)
        {
            if(SearchArray[i].occur<SearchArray[j].occur)
                temp=SearchArray[i];
                SearchArray[i]=SearchArray[j];
                SearchArray[j]=temp;
        }
    }
    printf("Title \t ISBN \t Times Borrowed");
    for(i=0;i<=5;i++)
    {
        printf("%s \t %d \t %d",SearchArray[i].title,SearchArray[i].ISBN,SearchArray[i].occur);
    }
}

void NewBorrow()
{
    int checkid,checkdate,i=0,found=0,flag=0;
    int checkisbn,found1=0;
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
        scanf("%d",&temp.ISBN);
        checkisbn=CheckISBN(temp.ISBN);
        if(checkisbn==0)
            printf("Please enter a legitimate ISBN! ISBNs are 13 digits long.\n");
        for(i=0;(found1==0);i++)
        {
            if(BooksArray[i].ISBN==temp.ISBN)
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
                printf("Please enter Date issued. Day first, then Month, then Year: \n");
                scanf("%d %d %d",&temp.Dateissued.Day,&temp.Dateissued.Month,&temp.Dateissued.Year);
                checkdate=CheckDate(temp.Dateissued);
                if(checkdate==0)
                    printf("Please enter a date that is not later than today! \n");
        }while(checkdate==0);
            do{
                printf("Please enter Date due to return. Day first, then Month, then Year: \n");
                scanf("%d %d %d",&temp.Datedue.Day,&temp.Datedue.Month,&temp.Datedue.Year);
                checkdate=CheckDate(temp.Dateissued);
                if(checkdate==0)
                    printf("Please enter a date that is not later than today! \n");
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
            printf("Please enter Date of return. Day first, then Month, then Year: \n");
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
    for(i=0;i<=LastBook;i++)
    {
        printf("%d \t %s \t %s \t %d \t %s \n",i+1,MembersArray[i].Surname,MembersArray[i].LastName,MembersArray[i].ID,MembersArray[i].mail);
    }
}

void DisplayBorrowings()
{

int i=0;
    printf("Transaction \t ISBN \t ID of Borrower \t Issued Date \t Due Date\n");
    for(i=0;i<=LastBook;i++)
    {
        printf("%d \t %d \t %d \t %d/%d/%d \t %d/%d/%d\n",BorrowsArray[i].Transaction,BorrowsArray[i].ISBN,BorrowsArray[i].ID,BorrowsArray[i].Dateissued.Day,BorrowsArray[i].Dateissued.Month,BorrowsArray[i].Dateissued.Year,BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year);
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
    Member temp;
    int i=0;
    int checkid,checkint;
    printf("Please enter the surname:\n");
    fgets(temp.Surname,sizeof(temp.Surname),stdin);
    printf("Please enter the last name:\n");
    fgets(temp.LastName,sizeof(temp.LastName),stdin);
    printf("Please enter the Email:\n");
    fgets(temp.mail,sizeof(temp.mail),stdin);
    do{
    printf("Enter the member ID:\n");
    scanf("%d",&temp.ID);
    checkid=CheckID(temp.ID);
    if(checkid==0)
        printf("Please enter a legitimate ID! ID numbers start from 1001 and end at 9999. \n");
    for(i=0;i<=LastMember;i++)
    {
        if(temp.ID==MembersArray[i].ID)
        {
            checkid=0;
            printf("This member is already registered in the system!");
        }
    }
    printf("Please enter the member's city of residence:\n");
    fgets(temp.Address.city,sizeof(temp.Address.city),stdin);
    printf("Please enter the member house's street:\n");
    fgets(temp.Address.street,sizeof(temp.Address.street),stdin);
    printf("Please enter the member house's building number:\n");
    scanf("%d",&temp.Address.bldg);
    }while(checkid==0);
    do{
    printf("Enter the member's age\n");
    scanf("%d",&temp.Age);
    checkint=isdigit(temp.Age);
    if(checkint==0)
        printf("Please enter an integer! \n");
    }while(checkint==0);
    do{
    printf("Enter the member's phone number\n");
    scanf("%d",&temp.PhoneNumber);
    checkint=isdigit(temp.PhoneNumber);
    if(checkint==0)
        printf("Please enter an integer! \n");
    }while(checkint==0);
    printf("Member added successfully!\n");
    LastMember++;
}

void DisplayBooks()
{
    int i=0;
    printf("\t Book Title \t Author Name \t ISBN \t Category \n");
    for(i=0;i<=LastBook;i++)
    {
        printf("%d \t %s \t %s \t %d \t %s \n",i+1,BooksArray[i].Title,BooksArray[i].AuthorName,BooksArray[i].ISBN,BooksArray[i].Category);
    }
}

void AddCopies()
{
    int ISB,i,flag=0,n;
    printf("Please enter book ISBN");
    scanf("%d",&ISB);
    for(i=0;(flag==1);i++){
        if(BooksArray[i].ISBN==ISB){
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
    int isbn,checkisbn,i,found=0;
    printf("Please enter the ISBN of the book to be deleted:\n");
    scanf("%d",&isbn);
    checkisbn=CheckISBN(isbn);
    if(checkisbn==0)
        printf("Please enter a legitimate ISBN! ISBNs are 13 digits long.");
    for(i=0;(found==0);i++)
    {
        if(BooksArray[i].ISBN==isbn)
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
    Book temp;
    int i=0;
    int checkdate,checkisbn,checkint;
    printf("Please enter the book title:\n");
    fgets(temp.Title,sizeof(temp.Title),stdin);
    printf("Please enter the author's name:\n");
    fgets(temp.AuthorName,sizeof(temp.Title),stdin);
    printf("Please enter the publisher's name:\n");
    fgets(temp.Publisher,sizeof(temp.Publisher),stdin);
    do{
    printf("Please enter Date Published. Day first, then Month, then Year: \n");
    scanf("%d %d %d",&temp.Publication.Day,&temp.Publication.Month,&temp.Publication.Year);
    checkdate=CheckDate(temp.Publication);
    if(checkdate==0)
        printf("Please enter a date that is not later than today! \n");
    }while(checkdate==0);
    do{
    printf("Enter the book ISBN (all in digits):\n");
    scanf("%d",&temp.ISBN);
    checkisbn=CheckISBN(temp.ISBN);
    if(checkisbn==0)
        printf("Please enter a legitimate ISBN! ISBNs are 13 digits long. \n");
    for(i=0;i<=LastBook;i++)
    {
        if(temp.ISBN==BooksArray[i].ISBN)
        {
            checkisbn=0;
            printf("This book ISBN is already registered in the system. You can add new copies or enter another ISBN!");
        }
    }
    }while(checkisbn==0);
    do{
    printf("Enter the number of copies:\n");
    scanf("%d",&temp.copies);
    checkint=isdigit(temp.copies);
    if(checkint==0)
        printf("Please enter an integer! \n");
    }while(checkint==0);
    temp.copies_available=temp.copies;
    printf("Enter the book category:\n");
    fgets(temp.Category,sizeof(temp.Category),stdin);
    printf("\nBook Successfully added ");
    LastBook++;
}

void SearchBook()
{
    int checkint,i,j=0,k=0,l=0,checkisbn=0,isbn=0,found=0,strequal;
    char *strpart;
    Book temp;
    Book SearchArray1[100], SearchArray2[100], SearchArray3[100];
    for(i=0;i>100;i++)
    {
        SearchArray1[i].ISBN=0;
        SearchArray2[i].ISBN=0;
        SearchArray3[i].ISBN=0;
    }
    printf("Please choose an action (1-3): \n");
    printf(" 1- Specific Search by ISBN\n");
    printf(" 2- Search by Title, Author and/or Category\n");
    do{
        scanf("%d",&input8);
        checkint=isdigit(input8);
        if(input8>2 || input8<1 || checkint==0)
        printf("Please enter an integer (1-2)!\n");
        else{
        switch(input8)
        {
            case 1:

                do{
                scanf("%d",&isbn);
                checkisbn=CheckISBN(isbn);
                if(checkisbn==0)
                printf("Please enter an IBSN! ISBNs are 13 digits long.\n");
                }while(checkisbn==0);
                for(i=0;i>LastBook;i++)
                {
                    if(BooksArray[i].ISBN==isbn)
                        {
                            found=1;
                            printf("Search Results: \n");
                            printf("Book Title \t Author Name \t ISBN \t Category \n");
                            printf("%s \t %s \t %d \t %s \n",BooksArray[i].Title,BooksArray[i].AuthorName,BooksArray[i].ISBN,BooksArray[i].Category);
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
                            SearchArray1[j].ISBN=BooksArray[i].ISBN;
                            strcpy(SearchArray1[j].Title,BooksArray[i].Title);
                            strcpy(SearchArray1[j].Category,BooksArray[i].Category);
                            strcpy(SearchArray1[j].AuthorName,BooksArray[i].AuthorName);
                            j++;
                        }
                    }
                }else{
                    for(i=0;i<=LastBook;i++)
                    {
                        SearchArray1[i].ISBN=BooksArray[i].ISBN;
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
                            SearchArray2[k].ISBN=SearchArray1[i].ISBN;
                            strcpy(SearchArray2[k].Title,SearchArray1[i].Title);
                            strcpy(SearchArray2[k].Category,SearchArray1[i].Category);
                            strcpy(SearchArray2[k].AuthorName,SearchArray1[i].AuthorName);
                            k++;
                        }
                    }

                }else{
                    for(i=0;i<100;i++)
                    {
                        SearchArray2[i].ISBN=SearchArray1[i].ISBN;
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
                            SearchArray3[l].ISBN=SearchArray2[i].ISBN;
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
                        SearchArray3[i].ISBN=SearchArray2[i].ISBN;
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
                            printf("%s \t %s \t %d \t %s\n",SearchArray3[i].Title,SearchArray3[i].AuthorName,SearchArray3[i].ISBN,SearchArray3[i].Category);
                        }
                }
    }}}while(input8>2 || input8<1 || checkint==0);
}

void Save()
{
    FILE *a,*b,*c;
    int i=0;
    a=fopen("Members.txt", "w");
    b=fopen("Books.txt", "w");
    c=fopen("Borrows.txt", "w");
    while(MembersArray[i].ID>0)
    {
        fprintf(a,"%s,%s,%d,%d,%s,%s,%d,%d,%s\n",MembersArray[i].Surname,MembersArray[i].LastName,MembersArray[i].ID,MembersArray[i].Address.bldg,MembersArray[i].Address.city,MembersArray[i].Address.street,MembersArray[i].PhoneNumber,MembersArray[i].Age,MembersArray[i].mail);
        i++;
    }
    i=0;
    while(BooksArray[i].ISBN>0)
    {
        fprintf(b,"%s,%s,%s,%d,%d,%d,%d,%d,%s\n",BooksArray[i].Title,BooksArray[i].AuthorName,BooksArray[i].Publisher,BooksArray[i].ISBN,BooksArray[i].Publication.Day,BooksArray[i].Publication.Month,BooksArray[i].Publication.Year,BooksArray[i].copies,BooksArray[i].Category);
        i++;
    }
    i=0;
    while(BorrowsArray[i].ISBN>0)
    {
        fprintf(c,"%d,%d,%d,%d,%d,%d,%d,%d,%d\n",BorrowsArray[i].Transaction,BorrowsArray[i].ISBN,BorrowsArray[i].ID,BorrowsArray[i].Datedue.Day,BorrowsArray[i].Datedue.Month,BorrowsArray[i].Datedue.Year,BorrowsArray[i].Dateissued.Day,BorrowsArray[i].Dateissued.Month,BorrowsArray[i].Dateissued.Year);
        i++;
    }
}

void BookManagement()
{
    int checkint;
    printf("Please choose an action (1-5): \n");
    printf(" 1- Insert a new book\n");
    printf(" 2- Search for a book\n");
    printf(" 3- Add new copies\n");
    printf(" 4- Delete book\n");
    printf(" 5- Display all books data\n");
	do{
        scanf("%d",&input4);
        checkint=isdigit(input4);
        if(input4>5 || input4<1 || checkint==0)
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
                DeleteBook();					/* Calls a function that deletes a book's data from the books file */
                break;
            case 5:
                DisplayBooks();
                break;
        }}}while(input4>5 || input4<1 || checkint==0);
    printf("Returning back to main menu ...\n");
    MainMenu();
}

void MemberManagement()
{
    int checkint;
    printf("Please choose an action (1-3): \n");
		printf(" 1- Register a new members\n");
		printf(" 2- Delete an existing member\n");
		printf(" 3- Display all members data\n");
	do{
        scanf("%d",&input5);
        checkint=isdigit(input5);
        if(input5>3 || input5<1 || checkint==0)
        printf("Please enter an integer (1-3)!\n");
        else{
        switch(input5)
        {
            case 1:
                NewMember();					/* Calls a function through which a new member will be created in the members File */
                break;
            case 2:
                DeleteMember();					/* Calls a function that deletes a member's data from the members file */
                break;
	}}}while(input5>3 || input5<1 || checkint==0);
    printf("Returning back to main menu ...\n");
    MainMenu();
}

void BorrowManagement()
{
    int checkint;
    printf("Please choose an action (1-2): \n");
		printf(" 1- Register a new borrowing transaction\n");
		printf(" 2- Register the return of a book\n");
		printf(" 3- Display all borrowing transactions\n");
    do{
        scanf("%d",&input6);
        checkint=isdigit(input6);
        if(input6>3 || input6<1 || checkint==0)
        printf("Please enter an integer (1-3)!\n");
        else{
        switch(input6)
        {
            case 1:
                NewBorrow();					/* Calls a function through which a new borrowing transaction will be created */
                break;
            case 2:
                ReturnBorrow();					/* Calls a function that confirms the return of a book */
                break;
            case 3:
                DisplayBorrowings();					/* Displays all books */
                break;
	}}}while(input6>3 || input6<1 || checkint==0);
    printf("Returning back to main menu ...\n");
    MainMenu();
}

void AdminActions()
{
    int checkint;
    printf("Please choose an action (1-2): \n");
		printf(" 1- Display Overdue Books\n");
		printf(" 2- Display the most popular books\n");
    do{
        scanf("%d",&input7);
        checkint=isdigit(input7);
        if(input7>3 || input7<1 || checkint==0)
        printf("Please enter an integer (1-2)!\n");
        else{
        switch(input6)
        {
            case 1:
                ODbooks();					/* Calls a function through which a new member will be created in the members File */
                break;
            case 2:
                PopBooks();					/* Calls a function that deletes a member's data from the members file */
                break;
	}}}while(input7>3 || input7<1 || checkint==0);
	printf("Returning back to main menu ...");
}

void MainMenu()
{
    int checkint,checkint1;
    printf("Please choose an action (1-6): \n");
    printf(" 1- Book Management\n");
    printf(" 2- Member Management\n");
    printf(" 3- Borrowing Management\n");
    printf(" 4- Administrative Actions\n");
    printf(" 5- Save\n");
    printf(" 6- Exit\n");

    do{
        scanf("%d",&input1);
        checkint=isdigit(input1);
        if((input1<1) || (input1>6) || (checkint==0))
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
                checkint1=isdigit(input2);
                if(input2<1 || input2>3 || checkint1==0)
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
			}while(input2>3 || input2<1 || checkint);}
			break;
    }
}while(input1<1 || input1>6 || checkint==0);
}

int main()
{

    int i=0;
    FILE *f,*j,*k;
    GetSystemTime(&DateTime);
    printf("Date: %d/%d/%d ",DateTime.wDay,DateTime.wMonth,DateTime.wYear);
    if((DateTime.wHour+2)<12)
        printf("Current Time: %d:%d AM \n \n",(DateTime.wHour+2),DateTime.wMinute);
    else if((DateTime.wHour+2)==12)
        printf("Current Time: %d:%d PM \n \n",(DateTime.wHour+2),DateTime.wMinute);
    else
        printf("Current Time: %d:%d PM \n \n",DateTime.wHour-10,DateTime.wMinute);

    /*Loop to Zero all IDs in MembersArray*/
    for(i=0;i>100;i++)
        MembersArray[i].ID=0;
    /*Loop to Zero all ISBNs in BooksArray*/
    for(i=0;i>100;i++)
        BooksArray[i].ISBN=0;
    /*Loop to Zero all IDs in BorrowsArray*/
    for(i=0;i>100;i++)
        BorrowsArray[i].ISBN=0;
    i=0;
    f=fopen("Members.txt", "r");
    if (f != NULL)
    {
    while (!feof(f))
        {
         fscanf(f, "%[^,],%[^,],%d,%d,%[^,],%[^,],%d,%d,%[^,]",MembersArray[i].Surname,MembersArray[i].LastName,&MembersArray[i].ID,&MembersArray[i].Address.bldg,MembersArray[i].Address.city,MembersArray[i].Address.street,&MembersArray[i].PhoneNumber,&MembersArray[i].Age,MembersArray[i].mail);
         fscanf(f, "\n");
         i++;
         LastMember++;
        }
         fclose(f);
    } else
        printf("Error opening the file!\n");
    i=0;
    j=fopen("Books.txt", "r");
    if (j != NULL)
    {
    while (!feof(j))
        {
         fscanf(j, "%[^,],%[^,],%[^,],%d,%d,%d,%d,%d,%[^,]",BooksArray[i].Title,BooksArray[i].AuthorName,BooksArray[i].Publisher,&BooksArray[i].ISBN,&BooksArray[i].Publication.Day,&BooksArray[i].Publication.Month,&BooksArray[i].Publication.Year,&BooksArray[i].copies,BooksArray[i].Category);
         fscanf(j, "\n");
         i++;
         LastBook++;
        }
         fclose(j);
    } else
        printf("Error opening the file!\n");
    i=0;
    k=fopen("Borrows.txt", "r");
    if (k != NULL)
    {
        while (!feof(k))
        {
         fscanf(k, "%d,%d,%d,%d,%d,%d,%d,%d",&BorrowsArray[i].ISBN,&BorrowsArray[i].ID,&BorrowsArray[i].Datedue.Day,&BorrowsArray[i].Datedue.Month,&BorrowsArray[i].Datedue.Year,&BorrowsArray[i].Dateissued.Day,&BorrowsArray[i].Dateissued.Month,&BorrowsArray[i].Dateissued.Year);
         fscanf(k, "\n");
         BorrowsArray[i].Transaction=i+1;
         LastTrans++;
         i++;
         LastBorrow++;
        }
         fclose(k);
    } else
        printf("Error opening the file!\n");
    MainMenu();
}
