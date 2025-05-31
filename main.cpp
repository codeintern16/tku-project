#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COURSES 100
#define MAX_NAME_LEN 50
#define MAX_SLOTS 5
#define DAYS 7
#define PERIODS 14

struct slot {
    int weekday;       // 1 = Monday, ..., 7 = Sunday
    int start_period;
    int end_period;
};

struct course {
    int course_id;
    int credits;
    char name[MAX_NAME_LEN];
    int slot_count;
    struct slot slots[MAX_SLOTS];
};

struct course courses[MAX_COURSES];
int course_count = 0;

int check_conflict(struct slot s) {
    for (int i = 0; i < course_count; i++) {
        for (int j = 0; j < courses[i].slot_count; j++) {
            if (courses[i].slots[j].weekday == s.weekday) {
                if (!(s.end_period < courses[i].slots[j].start_period || s.start_period > courses[i].slots[j].end_period)) {
                    return 1; // Conflict
                }
            }
        }
    }
    return 0;
}

void add_course() {
    if (course_count >= MAX_COURSES) {
        printf("Course list is full.\n");
        return;
    }

    struct course c;
    printf("Enter course ID: ");
    scanf("%d", &c.course_id);

    for (int i = 0; i < course_count; i++) {
        if (courses[i].course_id == c.course_id) {
            printf("Error: Course ID already exists.\n");
            return;
        }
    }

    printf("Enter course name: ");
    scanf("%s", c.name);
    printf("Enter credits: ");
    scanf("%d", &c.credits);

    printf("Enter number of time slots (max %d): ", MAX_SLOTS);
    scanf("%d", &c.slot_count);

    if (c.slot_count > MAX_SLOTS || c.slot_count < 1) {
        printf("Invalid slot count.\n");
        return;
    }

    for (int i = 0; i < c.slot_count; i++) {
        printf("Slot %d:\n", i + 1);
        printf("  Weekday (1=Mon, ..., 7=Sun): ");
        scanf("%d", &c.slots[i].weekday);
        printf("  Start period (1-14): ");
        scanf("%d", &c.slots[i].start_period);
        printf("  End period (1-14): ");
        scanf("%d", &c.slots[i].end_period);

        if (c.slots[i].end_period < c.slots[i].start_period) {
            printf("  Invalid period range.\n");
            return;
        }

        if (check_conflict(c.slots[i])) {
            printf("  Conflict detected on slot %d.\n", i + 1);
            return;
        }
    }

    courses[course_count++] = c;
    printf("Course added successfully.\n");
}

void delete_course() {
    int id;
    printf("Enter course ID to delete: ");
    scanf("%d", &id);
    for (int i = 0; i < course_count; i++) {
        if (courses[i].course_id == id) {
            for (int j = i; j < course_count - 1; j++) {
                courses[j] = courses[j + 1];
            }
            course_count--;
            printf("Course deleted.\n");
            return;
        }
    }
    printf("Course not found.\n");
}

void total_credits() {
    int total = 0;
    printf("\n%-5s | %-20s | %-7s\n", "ID", "Course Name", "Credits");
    printf("---------------------------------------------\n");

    for (int i = 0; i < course_count; i++) {
        printf("%-5d | %-20s | %-7d\n", courses[i].course_id, courses[i].name, courses[i].credits);
        total += courses[i].credits;
    }

    printf("---------------------------------------------\n");
    printf("Total credits: %d\n", total);
}

void print_vertical_timetable() {
    char timetable[DAYS][PERIODS + 1][MAX_NAME_LEN * 2] = {""};
    const char* days_of_week[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    for (int i = 0; i < course_count; i++) {
        for (int s = 0; s < courses[i].slot_count; s++) {
            int day = courses[i].slots[s].weekday - 1;
            for (int p = courses[i].slots[s].start_period; p <= courses[i].slots[s].end_period; p++) {
                snprintf(timetable[day][p], sizeof(timetable[day][p]), "%s(%d)", courses[i].name, courses[i].course_id);
            }
        }
    }

    printf("%-8s", "Period");
    for (int d = 0; d < DAYS; d++) {
        printf("| %-12s", days_of_week[d]);
    }
    printf("|\n");

    for (int i = 0; i < 1 + DAYS; i++) printf("-------------");
    printf("---\n");

    for (int p = 1; p <= PERIODS; p++) {
        printf("%-8d", p);
        for (int d = 0; d < DAYS; d++) {
            printf("| %-12s", timetable[d][p]);
        }
        printf("|\n");
    }
}

void save_to_csv(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Failed to open file.\n");
        return;
    }
    fprintf(fp, "course_id,name,credits,slot_weekday,slot_start,slot_end\n");
    for (int i = 0; i < course_count; i++) {
        for (int j = 0; j < courses[i].slot_count; j++) {
            fprintf(fp, "%d,%s,%d,%d,%d,%d\n", courses[i].course_id, courses[i].name, courses[i].credits,
                    courses[i].slots[j].weekday, courses[i].slots[j].start_period, courses[i].slots[j].end_period);
        }
    }
    fclose(fp);
    printf("Saved to %s\n", filename);
}

void load_from_csv(const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Failed to open file.\n");
        return;
    }

    char line[256];
    fgets(line, sizeof(line), fp); // skip header

    course_count = 0;
    while (fgets(line, sizeof(line), fp)) {
        int id, credits, weekday, start, end;
        char name[MAX_NAME_LEN];

        sscanf(line, "%d,%[^,],%d,%d,%d,%d", &id, name, &credits, &weekday, &start, &end);

        int found = -1;
        for (int i = 0; i < course_count; i++) {
            if (courses[i].course_id == id) {
                found = i;
                break;
            }
        }

        if (found == -1) {
            courses[course_count].course_id = id;
            strcpy(courses[course_count].name, name);
            courses[course_count].credits = credits;
            courses[course_count].slot_count = 0;
            found = course_count++;
        }

        int sc = courses[found].slot_count;
        courses[found].slots[sc].weekday = weekday;
        courses[found].slots[sc].start_period = start;
        courses[found].slots[sc].end_period = end;
        courses[found].slot_count++;
    }
    fclose(fp);
    printf("Loaded from %s\n", filename);
}

int main() {
    int choice;
    while (1) {
        printf("\n--- Course Scheduler Menu ---\n");
        printf("1. Add course\n");
        printf("2. Show total credits\n");
        printf("3. Print timetable\n");
        printf("4. Delete course\n");
        printf("5. Exit\n");
        printf("6. Save to CSV\n");
        printf("7. Load from CSV\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_course();
                break;
            case 2:
                total_credits();
                break;
            case 3:
                print_vertical_timetable();
                break;
            case 4:
                delete_course();
                break;
            case 5:
                printf("Exiting program.\n");
                return 0;
            case 6:
                save_to_csv("courses.csv");
                break;
            case 7:
                load_from_csv("courses.csv");
                break;
            default:
                printf("Invalid choice.\n");
        }
    }
    return 0;
}
