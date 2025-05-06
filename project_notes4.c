#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOTES 10
#define FILE_NAME "notes.txt"

struct Note {
    int day, month, year, hour, minute;
    char text[100];
};

struct Note notes[MAX_NOTES];
int noteCount = 0;

void clearScreen() {
    printf("\033[H\033[J"); 
}

void saveNotesToFile() {
    FILE *file = fopen(FILE_NAME, "w");
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }
    for (int i = 0; i < noteCount; i++) {
        fprintf(file, "%d %d %d %d %d %s", notes[i].day, notes[i].month, notes[i].year, notes[i].hour, notes[i].minute, notes[i].text);
    }
    fclose(file);
}

void loadNotesFromFile() {
    FILE *file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        return;
    }
    noteCount = 0;
    while (fscanf(file, "%d %d %d %d %d", &notes[noteCount].day, &notes[noteCount].month, &notes[noteCount].year, &notes[noteCount].hour, &notes[noteCount].minute) == 5) {
        fgetc(file); 
        fgets(notes[noteCount].text, 100, file);
        noteCount++;
        if (noteCount >= MAX_NOTES) break;
    }
    fclose(file);
}

int validateDate(int day, int month, int year) {
    if (month < 1 || month > 12) return 0;
    if (day < 1 || day > 31) return 0;
    if (month == 2) {
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
            if (day > 29) return 0; 
        } else {
            if (day > 28) return 0;
        }
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        if (day > 30) return 0;
    }
    return 1;
}

int validateTime(int hour, int minute) {
    if (hour < 0 || hour > 23) return 0;
    if (minute < 0 || minute > 59) return 0;
    return 1;
}

void addNote() {
    clearScreen();
    printf("--- Add Note ---\n");
    if (noteCount >= MAX_NOTES) {
        printf("Notebook is full!\n");
        return;
    }

    int day, month, year, hour, minute;
    while (1) {
        printf("Enter date and time (dd/mm/yyyy hh:mm): ");
        scanf("%d/%d/%d %d:%d", &day, &month, &year, &hour, &minute);
        if (validateDate(day, month, year) && validateTime(hour, minute)) break;
        else printf("Invalid date or time. Please try again.\n");
    }

    notes[noteCount].day = day;
    notes[noteCount].month = month;
    notes[noteCount].year = year;
    notes[noteCount].hour = hour;
    notes[noteCount].minute = minute;
    getchar();

    printf("Enter note: ");
    fgets(notes[noteCount].text, 100, stdin);
    noteCount++;
    saveNotesToFile();
    printf("Note added!\n");
}

void viewNotes() {
    clearScreen();
    printf("--- View Notes ---\n");
    if (noteCount == 0) {
        printf("No notes available!\n");
        return;
    }
    for (int i = 0; i < noteCount; i++) {
        printf("\033[0;37m");
        printf("%02d-%02d-%04d", notes[i].day, notes[i].month, notes[i].year);
        printf(" \033[1;36m%02d:%02d\033[0m", notes[i].hour, notes[i].minute);
        printf(" - \033[38;5;214m%s\033[0m\n", notes[i].text);
    }
}

void viewNotesByDate() {
    clearScreen();
    printf("--- View Notes by Date ---\n");
    int d, m, y, found = 0;

    while (1) {
        printf("Enter date (dd/mm/yyyy): ");
        scanf("%d/%d/%d", &d, &m, &y);
        if (validateDate(d, m, y)) break;
        else printf("Invalid date. Please try again.\n");
    }
    getchar();

    for (int i = 0; i < noteCount; i++) {
        if (notes[i].day == d && notes[i].month == m && notes[i].year == y) {
            printf("\033[1;36m%02d:%02d\033[0m - \033[38;5;214m%s\033[0m", notes[i].hour, notes[i].minute, notes[i].text);
            found = 1;
        }
    }
    if (!found) printf("No notes found!\n");
}

void editNote() {
    clearScreen();
    printf("--- Edit Note ---\n");
    int d, m, y, h, min, found = 0;

    while (1) {
        printf("Enter date and time of the note to edit (dd/mm/yyyy hh:mm): ");
        scanf("%d/%d/%d %d:%d", &d, &m, &y, &h, &min);
        if (validateDate(d, m, y) && validateTime(h, min)) break;
        else printf("Invalid date or time. Please try again.\n");
    }

    for (int i = 0; i < noteCount; i++) {
        if (notes[i].day == d && notes[i].month == m && notes[i].year == y && notes[i].hour == h && notes[i].minute == min) {
            char choice;
            printf("Do you want to change the date and time? (y/n): ");
            scanf(" %c", &choice);
            if (choice == 'y' || choice == 'Y') {
                while (1) {
                    printf("Enter new date and time (dd/mm/yyyy hh:mm): ");
                    scanf("%d/%d/%d %d:%d", &notes[i].day, &notes[i].month, &notes[i].year, &notes[i].hour, &notes[i].minute);
                    if (validateDate(notes[i].day, notes[i].month, notes[i].year) && validateTime(notes[i].hour, notes[i].minute)) break;
                    else printf("Invalid date or time. Please try again.\n");
                }
            }
            getchar();
            printf("Enter new note: ");
            fgets(notes[i].text, 100, stdin);
            saveNotesToFile();
            printf("Note updated!\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("No note found!\n");
}

void deleteNote() {
    clearScreen();
    printf("--- Delete Note ---\n");
    int d, m, y, h, min, found = 0;

    while (1) {
        printf("Enter date and time of the note to delete (dd/mm/yyyy hh:mm): ");
        scanf("%d/%d/%d %d:%d", &d, &m, &y, &h, &min);
        if (validateDate(d, m, y) && validateTime(h, min)) break;
        else printf("Invalid date or time. Please try again.\n");
    }

    for (int i = 0; i < noteCount; i++) {
        if (notes[i].day == d && notes[i].month == m && notes[i].year == y && notes[i].hour == h && notes[i].minute == min) {
            for (int j = i; j < noteCount - 1; j++) {
                notes[j] = notes[j + 1];
            }
            noteCount--;
            saveNotesToFile();
            printf("Note deleted!\n");
            found = 1;
            break;
        }
    }
    if (!found) printf("No note found!\n");
}

void menu() {
    int choice;
    loadNotesFromFile();
    while (1) {
        clearScreen();
        printf("\n--- Notes Application ---\n");
        printf("1. Add Note\n2. View All Notes\n3. View Notes by Date\n4. Edit Note\n5. Delete Note\n6. Exit\nChoose: ");
        scanf("%d", &choice);
        getchar();
        if (choice == 1) { addNote(); }
        else if (choice == 2) { viewNotes(); }
        else if (choice == 3) { viewNotesByDate(); }
        else if (choice == 4) { editNote(); }
        else if (choice == 5) { deleteNote(); }
        else if (choice == 6) { break; }
        else { printf("Invalid choice!\n"); }
        printf("\nPress Enter to return to menu...");
        getchar();
    }
}

int main() {
    menu();
    return 0;
}
