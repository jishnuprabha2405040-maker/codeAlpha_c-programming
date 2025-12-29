#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100   // maximum number of students

typedef struct {
    int roll;
    char name[50];
    int age;
    char gender;    // 'M' or 'F' or 'O'
    char course[40];
    float marks;
} Student;

Student students[MAX];
int count = 0;

void addStudent() {
    if (count >= MAX) {
        printf("Cannot add more students.\n");
        return;
    }
    Student s;
    printf("Enter roll number: ");
    scanf("%d", &s.roll);
    getchar(); // clear newline

    printf("Enter name: ");
    fgets(s.name, sizeof(s.name), stdin);
    s.name[strcspn(s.name, "\n")] = '\0';

    printf("Enter age: ");
    scanf("%d", &s.age);
    getchar();

    printf("Enter gender (M/F/O): ");
    s.gender = getchar();
    getchar();

    printf("Enter course: ");
    fgets(s.course, sizeof(s.course), stdin);
    s.course[strcspn(s.course, "\n")] = '\0';

    printf("Enter marks: ");
    scanf("%f", &s.marks);

    students[count++] = s;
    printf("Student added successfully!\n");
}

void displayAll() {
    if (count == 0) {
        printf("No records to display.\n");
        return;
    }
    printf("-------------------------------------------------------------\n");
    printf("| Roll | Name                 | Age | Gender | Course   | Marks |\n");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("| %-4d | %-20s | %-3d | %-6c | %-8s | %-5.2f |\n",
               students[i].roll, students[i].name, students[i].age,
               students[i].gender, students[i].course, students[i].marks);
    }
    printf("-------------------------------------------------------------\n");
}

void searchStudent() {
    int roll;
    printf("Enter roll number to search: ");
    scanf("%d", &roll);
    for (int i = 0; i < count; i++) {
        if (students[i].roll == roll) {
            printf("Found: %s (Age %d, Course %s, Marks %.2f)\n",
                   students[i].name, students[i].age,
                   students[i].course, students[i].marks);
            return;
        }
    }
    printf("No student found with roll %d.\n", roll);
}

void updateStudent() {
    int roll;
    printf("Enter roll number to update: ");
    scanf("%d", &roll);
    for (int i = 0; i < count; i++) {
        if (students[i].roll == roll) {
            printf("Enter new name: ");
            getchar();
            fgets(students[i].name, sizeof(students[i].name), stdin);
            students[i].name[strcspn(students[i].name, "\n")] = '\0';

            printf("Enter new age: ");
            scanf("%d", &students[i].age);

            printf("Enter new gender (M/F/O): ");
            getchar();
            students[i].gender = getchar();

            printf("Enter new course: ");
            getchar();
            fgets(students[i].course, sizeof(students[i].course), stdin);
            students[i].course[strcspn(students[i].course, "\n")] = '\0';

            printf("Enter new marks: ");
            scanf("%f", &students[i].marks);

            printf("Record updated successfully!\n");
            return;
        }
    }
    printf("No student found with roll %d.\n", roll);
}

void deleteStudent() {
    int roll;
    printf("Enter roll number to delete: ");
    scanf("%d", &roll);
    for (int i = 0; i < count; i++) {
        if (students[i].roll == roll) {
            for (int j = i; j < count - 1; j++) {
                students[j] = students[j + 1];
            }
            count--;
            printf("Record deleted successfully!\n");
            return;
        }
    }
    printf("No student found with roll %d.\n", roll);
}

int main() {
    int choice;
    while (1) {
        printf("\n=== Student Management System ===\n");
        printf("1. Add student\n");
        printf("2. Display all students\n");
        printf("3. Search student\n");
        printf("4. Update student\n");
        printf("5. Delete student\n");
        printf("6. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Exiting...\n"); exit(0);
            default: printf("Invalid choice.\n");
        }
    }
    return 0;
}
