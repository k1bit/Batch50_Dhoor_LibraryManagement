/* Library program. Compile: gcc library.c -o library   Run: ./library */
#include <stdio.h>
#include <stdlib.h>
#define MAX_BOOKS 3
#define FINE_PER_DAY 5
#define RENEW_DAYS 7
char category[5][15] = {"Programming", "Science", "History", "Fiction", "Self-Help"};
/* Book number = position + 1. Books 1-2 Programming, 3-4 Science, and so on */
char title[10][30] = {"The C Language", "Clean Code", "Cosmos", "The Selfish Gene",
    "Sapiens", "Discovery of India", "The Alchemist", "The Hobbit",
    "Atomic Habits", "Wings of Fire"};
/* status: 0 = in library, 1 = with me, 2 = with another person */
int status[10] = {0, 0, 0, 2, 0, 0, 0, 2, 0, 0};
int dueDay[10] = {0, 0, 0, 6, 0, 0, 0, 10, 0, 0};
int renewed[10];
int today = 1, totalFine = 0;
int readNumber(int low, int high) {
    int num, ok;
    do {
        ok = scanf("%d", &num);
        if (ok != 1) {
            scanf("%*s");
            num = low - 1;
        }
        if (num < low || num > high) printf("Enter a number from %d to %d: ", low, high);
    } while (num < low || num > high);
    return num;
}
int loanDays(int cat) {
    switch (cat) {
        case 0: return 10;
        case 1: return 14;
        case 2: return 21;
        case 3: return 14;
        default: return 7;
    }
}
int countMyBooks() {
    int i, count = 0;
    for (i = 0; i < 10; i++)
        if (status[i] == 1) count++;
    return count;
}
int hasOverdue() {
    int i;
    for (i = 0; i < 10; i++)
        if (status[i] == 1 && today > dueDay[i]) return 1;
    return 0;
}
void showBooks() {
    int c, b, i;
    printf("\n====== LIBRARY BOOKS (Day %d) ======\n", today);
    for (c = 0; c < 5; c++) {
        printf("\n%s (loan: %d days)\n", category[c], loanDays(c));
        for (b = 0; b < 2; b++) {
            i = c * 2 + b;
            printf("  %d. %-20s ", i + 1, title[i]);
            if (status[i] == 0) printf("Available\n");
            else if (status[i] == 1) printf("With YOU (due day %d)\n", dueDay[i]);
            else printf("Taken, back on day %d\n", dueDay[i]);
        }
    }
}
void showMyBooks() {
    int i;
    printf("\nYour books:\n");
    for (i = 0; i < 10; i++)
        if (status[i] == 1) printf("  %d. %s (due day %d)\n", i + 1, title[i], dueDay[i]);
}
void payFine() {
    char answer;
    if (totalFine == 0) { printf("\nYou have no fine.\n"); return; }
    printf("\nYour fine is Rs %d. Pay now? (y/n): ", totalFine);
    scanf(" %c", &answer);
    if (answer == 'y' || answer == 'Y') {
        printf("Fine paid. Thank you!\n");
        totalFine = 0;
    } else
        printf("Fine not paid. You cannot borrow until you pay.\n");
}
void borrowBook() {
    int n, i;
    if (totalFine > 0) { printf("\nPay your fine of Rs %d first.\n", totalFine); return; }
    if (hasOverdue()) { printf("\nYou have a late book. Return it first.\n"); return; }
    if (countMyBooks() >= MAX_BOOKS) { printf("\nLimit is %d books.\n", MAX_BOOKS); return; }
    showBooks();
    printf("\nEnter book number (1-10, 0 to cancel): ");
    n = readNumber(0, 10);
    if (n == 0) return;
    i = n - 1;
    if (status[i] == 1) printf("\nYou already have this book.\n");
    else if (status[i] == 2) printf("\nSomeone else has it. Back on day %d.\n", dueDay[i]);
    else {
        status[i] = 1;
        dueDay[i] = today + loanDays(i / 2);
        renewed[i] = 0;
        printf("\nYou got: %s\n", title[i]);
        printf("Issue day : %d\nRETURN BY : day %d\n", today, dueDay[i]);
        printf("Late fine : Rs %d per day\n", FINE_PER_DAY);
    }
}
void returnBook() {
    int n, i, late;
    if (countMyBooks() == 0) { printf("\nYou have no books to return.\n"); return; }
    showMyBooks();
    printf("Enter book number to return (0 to cancel): ");
    n = readNumber(0, 10);
    if (n == 0) return;
    i = n - 1;
    if (status[i] != 1) { printf("\nThat book is not with you.\n"); return; }
    late = today - dueDay[i];
    printf("\nReturned: %s\n", title[i]);
    if (late > 0) {
        totalFine = totalFine + late * FINE_PER_DAY;
        printf("%d day(s) late. Fine = Rs %d\n", late, late * FINE_PER_DAY);
    } else
        printf("Returned on time. No fine.\n");
    status[i] = 0;
    dueDay[i] = 0;
    if (totalFine > 0) payFine();
}
void renewBook() {
    int n, i;
    if (countMyBooks() == 0) { printf("\nYou have no books to renew.\n"); return; }
    showMyBooks();
    printf("Enter book number to renew (0 to cancel): ");
    n = readNumber(0, 10);
    if (n == 0) return;
    i = n - 1;
    if (status[i] != 1) printf("\nThat book is not with you.\n");
    else if (today > dueDay[i]) printf("\nThis book is late. Please return it.\n");
    else if (renewed[i] == 1) printf("\nYou can renew a book only one time.\n");
    else {
        dueDay[i] = dueDay[i] + RENEW_DAYS;
        renewed[i] = 1;
        printf("\nRenewed! New return day: %d\n", dueDay[i]);
    }
}
void myAccount() {
    int i, left;
    printf("\n--- MY ACCOUNT (Today is day %d) ---\n", today);
    printf("Books with me: %d out of %d\n", countMyBooks(), MAX_BOOKS);
    for (i = 0; i < 10; i++) {
        if (status[i] != 1) continue;
        left = dueDay[i] - today;
        printf("  %s: ", title[i]);
        if (left > 0) printf("%d day(s) left\n", left);
        else if (left == 0) printf("DUE TODAY\n");
        else printf("LATE by %d day(s), fine Rs %d\n", -left, -left * FINE_PER_DAY);
    }
    printf("Fine to pay: Rs %d\n", totalFine);
}
void skipDays() {
    int i;
    printf("\nHow many days to skip? (1-60): ");
    today = today + readNumber(1, 60);
    for (i = 0; i < 10; i++)
        if (status[i] == 2 && dueDay[i] <= today) { status[i] = 0; dueDay[i] = 0; }
    printf("Now it is day %d.\n", today);
}
int main() {
    char name[30];
    int id, tries = 0, choice;
    printf("===== WELCOME TO CITY LIBRARY =====\nEnter your name: ");
    scanf("%29s", name);
    do {
        printf("Enter 4-digit member ID (1000-9999): ");
        id = readNumber(0, 99999);
        tries++;
        if (id < 1000 || id > 9999) printf("Wrong ID!\n");
    } while ((id < 1000 || id > 9999) && tries < 3);
    if (id < 1000 || id > 9999) { printf("Too many wrong tries. Goodbye.\n"); return 1; }
    printf("\nHello %s! Max %d books, fine Rs %d per late day.\n", name, MAX_BOOKS, FINE_PER_DAY);
    do {
        printf("\n------ MENU (Day %d) ------\n", today);
        printf("1. View books\n2. Borrow a book\n3. Return a book\n4. Renew a book\n");
        printf("5. My account\n6. Pay fine\n7. Skip days\n0. Exit\nEnter choice: ");
        choice = readNumber(0, 7);
        switch (choice) {
            case 1: showBooks(); break;
            case 2: borrowBook(); break;
            case 3: returnBook(); break;
            case 4: renewBook(); break;
            case 5: myAccount(); break;
            case 6: payFine(); break;
            case 7: skipDays(); break;
            case 0: printf("\nThank you, %s. Goodbye!\n", name); break;
        }
    } while (choice != 0);
    return 0;
}