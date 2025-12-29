#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define DATAFILE "students.dat"
#define TMPFILE  "temp.dat"
typedef struct {
    int roll;
    char name[50];
    int age;
    char gender;    // 'M' or 'F' or 'O'
    char course[40];
    float marks;
} Student;

void flushInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void trimNewline(char *s) {
    size_t n = strlen(s);
    if (n && s[n-1] == '\n') s[n-1] = '\0';
}

int rollExists(int roll) {
    FILE *fp = fopen(DATAFILE, "rb");
    if (!fp) return 0;
    Student s;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == roll) { fclose(fp); return 1; }
    }
    fclose(fp);
    return 0;
}

void addStudent(void) {
    Student s;
    printf("Enter roll number: ");
    if (scanf("%d", &s.roll) != 1) { printf("Invalid input.\n"); flushInput(); return; }
    flushInput();
    if (rollExists(s.roll)) {
        printf("A student with roll %d already exists.\n", s.roll);
        return;
    }
    printf("Enter name: ");
    fgets(s.name, sizeof(s.name), stdin);
    trimNewline(s.name);
    printf("Enter age: ");
    if (scanf("%d", &s.age) != 1) { printf("Invalid input.\n"); flushInput(); return; }
    flushInput();
    printf("Enter gender (M/F/O): ");
    s.gender = getchar();
    flushInput();
    s.gender = toupper((unsigned char)s.gender);
    printf("Enter course: ");
    fgets(s.course, sizeof(s.course), stdin);
    trimNewline(s.course);
    printf("Enter marks (0.0 - 100.0): ");
    if (scanf("%f", &s.marks) != 1) { printf("Invalid input.\n"); flushInput(); return; }
    flushInput();

    FILE *fp = fopen(DATAFILE, "ab");
    if (!fp) { perror("Failed to open data file"); return; }
    fwrite(&s, sizeof(Student), 1, fp);
    fclose(fp);
    printf("Student added successfully.\n");
}

void displayAll(void) {
    FILE *fp = fopen(DATAFILE, "rb");
    if (!fp) { printf("No records found.\n"); return; }
    Student s;
    printf("--------------------------------------------------------------------------------\n");
    printf("| %-6s | %-20s | %-3s | %-6s | %-18s | %-6s |\n", "Roll", "Name", "Age", "Gender", "Course", "Marks");
    printf("--------------------------------------------------------------------------------\n");
    int found = 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        printf("| %-6d | %-20s | %-3d | %-6c | %-18s | %-6.2f |\n",
               s.roll, s.name, s.age, s.gender, s.course, s.marks);
        found = 1;
    }
    if (!found) printf("|                         No records available                              |\n");
    printf("--------------------------------------------------------------------------------\n");
    fclose(fp);
}

void searchStudent(void) {
    int choice;
    printf("Search by: 1) Roll  2) Name\nChoose (1-2): ");
    if (scanf("%d", &choice) != 1) { printf("Invalid input.\n"); flushInput(); return; }
    flushInput();

    FILE *fp = fopen(DATAFILE, "rb");
    if (!fp) { printf("No records found.\n"); return; }
    Student s;
    int found = 0;
    if (choice == 1) {
        int r;
        printf("Enter roll number: ");
        if (scanf("%d", &r) != 1) { printf("Invalid input.\n"); flushInput(); fclose(fp); return; }
        flushInput();
        while (fread(&s, sizeof(Student), 1, fp) == 1) {
            if (s.roll == r) {
                printf("Record found:\n");
                printf("Roll: %d\nName: %s\nAge: %d\nGender: %c\nCourse: %s\nMarks: %.2f\n",
                       s.roll, s.name, s.age, s.gender, s.course, s.marks);
                found = 1; break;
            }
        }
    } else if (choice == 2) {
        char qname[50];
        printf("Enter name or substring to search: ");
        fgets(qname, sizeof(qname), stdin);
        trimNewline(qname);
        while (fread(&s, sizeof(Student), 1, fp) == 1) {
            if (strcasestr(s.name, qname) != NULL) {
                if (!found) printf("Matching records:\n");
                printf("Roll: %d | Name: %s | Age: %d | Gender: %c | Course: %s | Marks: %.2f\n",
                       s.roll, s.name, s.age, s.gender, s.course, s.marks);
                found = 1;
            }
        }
    } else {
        printf("Invalid choice.\n");
    }
    if (!found) printf("No matching records found.\n");
    fclose(fp);
}

/* strcasestr is not standard C on all platforms; provide fallback */
char *strcasestr(const char *haystack, const char *needle) {
    if (!*needle) return (char*)haystack;
    for (; *haystack; ++haystack) {
        const char *h = haystack;
        const char *n = needle;
        while (*h && *n && tolower((unsigned char)*h) == tolower((unsigned char)*n)) { h++; n++; }
        if (!*n) return (char*)haystack;
    }
    return NULL;
}

void updateStudent(void) {
    int r;
    printf("Enter roll number to update: ");
    if (scanf("%d", &r) != 1) { printf("Invalid input.\n"); flushInput(); return; }
    flushInput();

    FILE *fp = fopen(DATAFILE, "r+b");
    if (!fp) { printf("No records found.\n"); return; }
    Student s;
    int found = 0;
    long pos;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == r) {
            found = 1;
            pos = ftell(fp) - sizeof(Student);
            printf("Current details:\n");
            printf("Roll: %d\nName: %s\nAge: %d\nGender: %c\nCourse: %s\nMarks: %.2f\n",
                   s.roll, s.name, s.age, s.gender, s.course, s.marks);

            printf("Enter new name (leave blank to keep current): ");
            char buf[50];
            fgets(buf, sizeof(buf), stdin);
            trimNewline(buf);
            if (strlen(buf)) strncpy(s.name, buf, sizeof(s.name));

            printf("Enter new age (0 to keep current): ");
            int a;
            if (scanf("%d", &a) == 1) {
                if (a > 0) s.age = a;
            } else { flushInput(); }
            flushInput();

            printf("Enter new gender (M/F/O or 0 to keep current): ");
            int ch = getchar();
            if (ch != '\n' && ch != EOF) {
                char g = (char)ch;
                flushInput();
                if (g != '0') s.gender = toupper((unsigned char)g);
            }

            printf("Enter new course (leave blank to keep current): ");
            fgets(buf, sizeof(buf), stdin);
            trimNewline(buf);
            if (strlen(buf)) strncpy(s.course, buf, sizeof(s.course));

            printf("Enter new marks (-1 to keep current): ");
            float m;
            if (scanf("%f", &m) == 1) {
                if (m >= 0.0f) s.marks = m;
            } else { flushInput(); }
            flushInput();

            /* Write back at correct position */
            fseek(fp, pos, SEEK_SET);
            fwrite(&s, sizeof(Student), 1, fp);
            printf("Record updated.\n");
            break;
        }
    }
    if (!found) printf("Student with roll %d not found.\n", r);
    fclose(fp);
}

void deleteStudent(void) {
    int r;
    printf("Enter roll number to delete: ");
    if (scanf("%d", &r) != 1) { printf("Invalid input.\n"); flushInput(); return; }
    flushInput();

    FILE *fp = fopen(DATAFILE, "rb");
    if (!fp) { printf("No records found.\n"); return; }
    FILE *tmp = fopen(TMPFILE, "wb");
    if (!tmp) { perror("Failed to open temporary file"); fclose(fp); return; }

    Student s;
    int found = 0;
    while (fread(&s, sizeof(Student), 1, fp) == 1) {
        if (s.roll == r) {
            found = 1;
            continue; /* skip writing this record */
        }
        fwrite(&s, sizeof(Student), 1, tmp);
    }
    fclose(fp);
    fclose(tmp);

    if (!found) {
        remove(TMPFILE);
        printf("Student with roll %d not found.\n", r);
        return;
    }

    if (remove(DATAFILE) != 0) perror("Warning: could not remove original data file");
    if (rename(TMPFILE, DATAFILE) != 0) perror("Warning: could not rename temp file");
    printf("Record deleted successfully.\n");
}

void showMenu(void) {
    printf("\n=== Student Management System ===\n");
    printf("1. Add student\n");
    printf("2. Display all students\n");
    printf("3. Search student\n");
    printf("4. Update student\n");
    printf("5. Delete student\n");
    printf("6. Exit\n");
    printf("Choose an option (1-6): ");
}

int main(void) {
    int choice;
    while (1) {
        showMenu();
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Try again.\n");
            flushInput();
            continue;
        }
