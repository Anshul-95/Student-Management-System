#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUD_FILE "students.txt"
#define CRED_FILE "credentials.txt"

typedef struct {
    int id;
    char name[50];
    int age;
    float marks;
} Student;

typedef struct {
    char username[20];
    char password[20];
    char role[10];
} User;

void addStudent();
void viewStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int login(char *roleOut) {
    FILE *fp = fopen(CRED_FILE, "r");
    if (!fp) {
        printf("Error: credentials file missing!\n");
        return 0;
    }

    char user[20], pass[20];
    printf("Username: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);

    User u;
    while (fscanf(fp, "%s %s %s", u.username, u.password, u.role) != EOF) {
        if (strcmp(u.username, user) == 0 && strcmp(u.password, pass) == 0) {
            strcpy(roleOut, u.role);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void addStudent() {
    FILE *fp = fopen(STUD_FILE, "a");
    Student s;

    printf("Enter ID: ");
    scanf("%d", &s.id);
    printf("Enter Name: ");
    scanf("%s", s.name);
    printf("Enter Age: ");
    scanf("%d", &s.age);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);

    fprintf(fp, "%d %s %d %.2f\n", s.id, s.name, s.age, s.marks);
    fclose(fp);

    printf("Student added successfully!\n");
}

void viewStudents() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    Student s;
    printf("\nID   Name     Age   Marks\n");
    printf("-----------------------------\n");

    while (fscanf(fp, "%d %s %d %f", &s.id, s.name, &s.age, &s.marks) != EOF) {
        printf("%d  %s  %d  %.2f\n", s.id, s.name, s.age, s.marks);
    }

    fclose(fp);
}

void searchStudent() {
    FILE *fp = fopen(STUD_FILE, "r");
    if (!fp) {
        printf("No student records found!\n");
        return;
    }

    int id;
    int found = 0;
    Student s;

    printf("Enter ID to search: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d %f", &s.id, s.name, &s.age, &s.marks) != EOF) {
        if (s.id == id) {
            printf("\nRecord Found:\n");
            printf("ID: %d\nName: %s\nAge: %d\nMarks: %.2f\n",
                   s.id, s.name, s.age, s.marks);
            found = 1;
            break;
        }
    }

    fclose(fp);

    if (!found)
        printf("Student not found!\n");
}

void updateStudent() {
    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    int id;
    int found = 0;
    Student s;

    printf("Enter ID to update: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d %f", &s.id, s.name, &s.age, &s.marks) != EOF) {
        if (s.id == id) {
            printf("Enter new name: ");
            scanf("%s", s.name);
            printf("Enter new age: ");
            scanf("%d", &s.age);
            printf("Enter new marks: ");
            scanf("%f", &s.marks);
            found = 1;
        }
        fprintf(temp, "%d %s %d %.2f\n", s.id, s.name, s.age, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found)
        printf("Record updated!\n");
    else
        printf("Student not found!\n");
}

void deleteStudent() {
    FILE *fp = fopen(STUD_FILE, "r");
    FILE *temp = fopen("temp.txt", "w");

    int id;
    int found = 0;
    Student s;

    printf("Enter ID to delete: ");
    scanf("%d", &id);

    while (fscanf(fp, "%d %s %d %f", &s.id, s.name, &s.age, &s.marks) != EOF) {
        if (s.id == id) {
            found = 1;
            continue;
        }
        fprintf(temp, "%d %s %d %.2f\n", s.id, s.name, s.age, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove(STUD_FILE);
    rename("temp.txt", STUD_FILE);

    if (found)
        printf("Record deleted!\n");
    else
        printf("Student not found!\n");
}

int main() {
    char role[10];

    if (!login(role)) {
        printf("Invalid credentials!\n");
        return 0;
    }

    int choice;

    do {
        printf("\nLogged in as: %s\n", role);
        printf("1. Add Student\n");
        printf("2. View Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student (Admin only)\n");
        printf("0. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (strcmp(role, "ADMIN") != 0 && choice == 5) {
            printf("Permission denied!\n");
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 0: printf("Logging out...\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while (choice != 0);

    return 0;
}
