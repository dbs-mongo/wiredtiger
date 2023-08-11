#include <wiredtiger.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

WT_CURSOR* create_table(WT_CONNECTION *conn, WT_SESSION *session, char *table_name, char *config)
{      
    char table_spec[200]; 
    sprintf(table_spec, "table:%s", table_name);
    int ret = session->create(session, table_spec, config);
    if (ret != 0) {
        fprintf(stderr, "Failed to create table: %s\n", wiredtiger_strerror(ret));
        return NULL;
    }

    WT_CURSOR *cursor;
    char cursor_spec[100];
    sprintf(cursor_spec, "table:%s", table_name);
    ret = session->open_cursor(session, cursor_spec, NULL, NULL, &cursor);
    if (ret != 0) {
        fprintf(stderr, "Failed to open cursor: %s\n", wiredtiger_strerror(ret));
    }

    printf("Table %s created successfully!\n", table_name);
    return cursor;
}

void show_tables(WT_CONNECTION *conn){
    WT_SESSION *session;
    WT_CURSOR *cursor;
    int ret;

    // Open a session handle for the database.
    conn->open_session(conn, NULL, NULL, &session);

    // Open a cursor to the "metadata:" table.
    session->open_cursor(session, "metadata:", NULL, NULL, &cursor);

    // Traverse the "metadata:" table to get the table names.
    const char *key, *value;
    while ((ret = cursor->next(cursor)) == 0) {
        cursor->get_key(cursor, &key);
        cursor->get_value(cursor, &value);
        if (strncmp(key, "table:", 6) == 0) {
            printf("Table: %s\n", key + 6);
        }
    }
    cursor->close(cursor);
    session->close(session, NULL);
}

void drop_table(WT_SESSION *session, char *table_name)
{
    char table_spec[100];
    sprintf(table_spec, "table:%s", table_name);
    int ret = session->drop(session, table_spec, NULL);
    if (ret != 0) {
        fprintf(stderr, "Failed to create table: %s\n", wiredtiger_strerror(ret));
        return;
    } else {
        printf("Table %s dropped successfully.\n", table_name);
    }
}
