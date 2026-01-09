#include <stdio.h>
#include "db.h"

int main(void) {
    PGconn *conn = db_connect();

    db_init(conn);
    db_prepare_statements(conn);

    int choice, id, age;
    char name[100], major[100];

    while (1) {
        printf("\n1.Add 2.List 3.Get 4.Update 5.Delete 0.Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Name: "); scanf("%s", name);
            printf("Age: "); scanf("%d", &age);
            printf("Major: "); scanf("%s", major);
            create_student(conn, name, age, major);
            break;
        case 2:
            get_all_students(conn);
            break;
        case 3:
            printf("ID: "); scanf("%d", &id);
            get_student_by_id(conn, id);
            break;
        case 4:
            printf("ID: "); scanf("%d", &id);
            printf("Name: "); scanf("%s", name);
            printf("Age: "); scanf("%d", &age);
            printf("Major: "); scanf("%s", major);
            update_student(conn, id, name, age, major);
            break;
        case 5:
            printf("ID: "); scanf("%d", &id);
            delete_student(conn, id);
            break;
        case 0:
            db_disconnect(conn);
            return 0;
        default:
            printf("Invalid choice\n");
        }
    }
}
