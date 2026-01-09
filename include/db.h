#ifndef DB_H
#define DB_H

#include <libpq-fe.h>

/* Connection */
PGconn* db_connect(void);
void db_disconnect(PGconn *conn);

/* Init */
void db_init(PGconn *conn);
void db_prepare_statements(PGconn *conn);

/* CRUD */
void create_student(PGconn *conn, const char *name, int age, const char *major);
void get_all_students(PGconn *conn);
void get_student_by_id(PGconn *conn, int id);
void update_student(PGconn *conn, int id, const char *name, int age, const char *major);
void delete_student(PGconn *conn, int id);

#endif