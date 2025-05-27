#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COURSES 100
#define MAX_NAME_LEN 50
#define DAYS 7
#define PERIODS 14

struct course {
    int course_id;
    int credits;
    char name[MAX_NAME_LEN];
    int weekday;       // 1 = Monday, ..., 7 = Sunday
    int start_period;
    int end_period;
};

struct course courses[MAX_COURSES];
int course_count = 0;

// 檢查是否衝堂
int check_conflict(int weekday, int start, int end) {
    for (int i = 0; i < course_count; i++) {
        if (courses[i].weekday == weekday) {
            if (!(end < courses[i].start_period || start > courses[i].end_period)) {
                return 1; // 有衝堂
            }
        }
    }
    return 0;
}


// 加入課程
void add_course() {
    if (course_count >= MAX_COURSES) {
        printf("Course list is full.\n");
        return;
    }

    struct course c;
    printf("Enter course ID: ");
    scanf("%d", &c.course_id);

    // 檢查課程 ID 是否重複
    for (int i = 0; i < course_count; i++) {
        if (courses[i].course_id == c.course_id) {
            printf("Error: Course ID %d already exists. Course not added.\n", c.course_id);
            return;
        }
    }

    printf("Enter course name: ");
    scanf("%s", c.name);
    printf("Enter credits: ");
    scanf("%d", &c.credits);

    // 驗證 weekday
    printf("Enter weekday (1=Mon, ..., 7=Sun): ");
    scanf("%d", &c.weekday);
    if (c.weekday < 1 || c.weekday > 7) {
        printf("Error: Weekday must be between 1 and 7.\n");
        return;
    }

    // 驗證 start_period
    printf("Enter start period (1~14): ");
    scanf("%d", &c.start_period);
    if (c.start_period < 1 || c.start_period > 14) {
        printf("Error: Start period must be between 1 and 14.\n");
        return;
    }

    // 驗證 end_period
    printf("Enter end period (1~14): ");
    scanf("%d", &c.end_period);
    if (c.end_period < 1 || c.end_period > 14) {
        printf("Error: End period must be between 1 and 14.\n");
        return;
    }
    if (c.end_period < c.start_period) {
        printf("Error: End period cannot be earlier than start period.\n");
        return;
    }

    if (check_conflict(c.weekday, c.start_period, c.end_period)) {
        printf("Conflict detected! Course not added.\n");
        return;
    }

    courses[course_count++] = c;
    printf("Course added successfully.\n");
}



// 刪除課程
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
            printf("Course deleted successfully.\n");
            return;
        }
    }
    printf("Course with ID %d not found.\n", id);
}

// 學分加總，並顯示每門課程學分
void total_credits() {
    int total = 0;

    if (course_count == 0) {
        printf("No courses available.\n");
        return;
    }

    printf("\n%-5s | %-20s | %-7s\n", "ID", "Course Name", "Credits");
    printf("---------------------------------------------\n");

    for (int i = 0; i < course_count; i++) {
        printf("%-5d | %-20s | %-7d\n", courses[i].course_id, courses[i].name, courses[i].credits);
        total += courses[i].credits;
    }

    printf("---------------------------------------------\n");
    printf("Total credits: %d\n", total);
}


//輸出課表
void print_vertical_timetable() {
    char timetable[DAYS][PERIODS + 1][MAX_NAME_LEN * 2] = {""};  // 擴大大小以容納名稱+ID
    const char* days_of_week[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

    for (int i = 0; i < course_count; i++) {
        int day = courses[i].weekday % 7;
        for (int p = courses[i].start_period; p <= courses[i].end_period; p++) {
            snprintf(timetable[day][p], sizeof(timetable[day][p]), "%s(%d)", courses[i].name, courses[i].course_id);
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
            if (strlen(timetable[d][p]) > 0)
                printf("| %-12s", timetable[d][p]);
            else
                printf("| %-12s", "");
        }
        printf("|\n");
    }
}


// 主功能選單
int main() {
    int choice;
    while (1) {
        printf("\n--- Course Scheduler Menu ---\n");
        printf("1. Add course\n");
        printf("2. Show total credits\n");
        printf("3. Print timetable\n");
        printf("4. Delete course\n");
        printf("5. Exit\n");
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
            case 69:
                printf("不可以色色\n");
                break;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}