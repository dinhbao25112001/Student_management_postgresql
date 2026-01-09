#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"

/* ================= CONNECTION ================= */

PGconn* db_connect(void) {
    PGconn *conn = PQconnectdb(
        "host=localhost dbname=student_db user=student_user password=123456");

    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "DB connect error: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        exit(EXIT_FAILURE);
    }
    return conn;
}

void db_disconnect(PGconn *conn) {
    PQfinish(conn);
}

/* ================= INIT ================= */

void db_init(PGconn *conn) {
    PGresult *res = PQexec(conn,
        "CREATE TABLE IF NOT EXISTS students ("
        "id SERIAL PRIMARY KEY,"
        "name VARCHAR(100) NOT NULL,"
        "age INT NOT NULL,"
        "major VARCHAR(100))");

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Create table failed: %s\n", PQerrorMessage(conn));
    }
    PQclear(res);
}

/* ================= PREPARED STATEMENTS ================= */

void db_prepare_statements(PGconn *conn) {
    PGresult *res;

    res = PQprepare(conn, "create_student",
        "INSERT INTO students (name, age, major) VALUES ($1, $2, $3)",
        3, NULL);
    PQclear(res);

    res = PQprepare(conn, "get_all_students",
        "SELECT id, name, age, major FROM students",
        0, NULL);
    PQclear(res);

    res = PQprepare(conn, "get_student_by_id",
        "SELECT id, name, age, major FROM students WHERE id = $1",
        1, NULL);
    PQclear(res);

    res = PQprepare(conn, "update_student",
        "UPDATE students SET name=$1, age=$2, major=$3 WHERE id=$4",
        4, NULL);
    PQclear(res);

    res = PQprepare(conn, "delete_student",
        "DELETE FROM students WHERE id=$1",
        1, NULL);
    PQclear(res);
}

/* ================= CRUD ================= */

/* CREATE */
void create_student(PGconn *conn, const char *name, int age, const char *major) {
    const char *params[3];
    char age_str[12];

    snprintf(age_str, sizeof(age_str), "%d", age);
    params[0] = name;
    params[1] = age_str;
    params[2] = major;

    PGresult *res = PQexecPrepared(conn,
        "create_student",
        3,
        params,
        NULL,
        NULL,
        0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Insert failed: %s\n", PQerrorMessage(conn));
    } else {
        printf("Student created\n");
    }
    PQclear(res);
}

/* READ ALL */
void get_all_students(PGconn *conn) {
    PGresult *res = PQexecPrepared(conn,
        "get_all_students",
        0,
        NULL,
        NULL,
        NULL,
        0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Select failed: %s\n", PQerrorMessage(conn));
        PQclear(res);
        return;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        printf("ID:%s | Name:%s | Age:%s | Major:%s\n",
            PQgetvalue(res, i, 0),
            PQgetvalue(res, i, 1),
            PQgetvalue(res, i, 2),
            PQgetvalue(res, i, 3));
    }
    PQclear(res);
}

/* READ BY ID */
void get_student_by_id(PGconn *conn, int id) {
    const char *params[1];
    char id_str[12];

    snprintf(id_str, sizeof(id_str), "%d", id);
    params[0] = id_str;

    PGresult *res = PQexecPrepared(conn,
        "get_student_by_id",
        1,
        params,
        NULL,
        NULL,
        0);

    if (PQntuples(res) == 0) {
        printf("Student not found\n");
    } else {
        printf("ID:%s | Name:%s | Age:%s | Major:%s\n",
            PQgetvalue(res, 0, 0),
            PQgetvalue(res, 0, 1),
            PQgetvalue(res, 0, 2),
            PQgetvalue(res, 0, 3));
    }
    PQclear(res);
}

/* UPDATE */
void update_student(PGconn *conn, int id,
                    const char *name, int age, const char *major) {
    const char *params[4];
    char id_str[12], age_str[12];

    snprintf(id_str, sizeof(id_str), "%d", id);
    snprintf(age_str, sizeof(age_str), "%d", age);

    params[0] = name;
    params[1] = age_str;
    params[2] = major;
    params[3] = id_str;

    PGresult *res = PQexecPrepared(conn,
        "update_student",
        4,
        params,
        NULL,
        NULL,
        0);

    if (PQcmdTuples(res)[0] == '0') {
        printf("No student updated\n");
    } else {
        printf("Student updated\n");
    }
    PQclear(res);
}

/* DELETE */
void delete_student(PGconn *conn, int id) {
    const char *params[1];
    char id_str[12];

    snprintf(id_str, sizeof(id_str), "%d", id);
    params[0] = id_str;

    PGresult *res = PQexecPrepared(conn,
        "delete_student",
        1,
        params,
        NULL,
        NULL,
        0);

    if (PQcmdTuples(res)[0] == '0') {
        printf("No student deleted\n");
    } else {
        printf("Student deleted\n");
    }
    PQclear(res);
}
