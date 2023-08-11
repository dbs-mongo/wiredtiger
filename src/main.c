#include <wiredtiger.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "crud.h"
#include "table.h"
#include "index.h"
#include "querying.h"

static const char *home = "~/rat-wt/wt";

int main(int argc, char *argv[]){
    WT_CONNECTION *conn;
    WT_CURSOR *cursor;
    WT_SESSION *session;

    /* Open a connection to the database, creating it if necessary. */
    int ret = wiredtiger_open(home, NULL, "create,statistics=(all)", &conn);
    if (ret != 0) {
        fprintf(stderr, "Failed to open connection: %s\n", wiredtiger_strerror(ret));
        return ret;
    }

    /* Open a session for the current thread's work. */
    ret = conn->open_session(conn, NULL, NULL, &session);
    if (ret != 0) {
        fprintf(stderr, "Failed to open session: %s\n", wiredtiger_strerror(ret));
        conn->close(conn, NULL);
        return ret;
    }

    /* Create a table */
    char table_name[100];
    printf("Enter table name to create: ");
    scanf("%s", table_name);

    char config[200];
    printf("Enter configuration string: ");
    scanf("%s", config);

    cursor = create_table(conn, session, table_name, config); // create wiredtiger table

    if (cursor == NULL) {
        fprintf(stderr, "Failed to create table and get cursor.\n");
        return EXIT_FAILURE;
    }

    /* Show tables */
    show_tables(conn);

    /* Insert */
    int id;
    char value[100];
    printf("Enter id to insert: ");
    scanf("%d", &id);

    printf("Enter value to insert: ");
    scanf("%s", value);

    insert_one(session, table_name, id, value);

    int num_rows;
    printf("Enter the number of rows to insert: ");
    scanf("%d", &num_rows);

    insert_many(session, table_name, num_rows);

    /* Search */
    int search_id;
    printf("Enter id to search: ");
    scanf("%d", &search_id);

    find(session, table_name, search_id);

    /* Update */
    char update_key[100];
    char new_value[100];
    printf("Enter key to update: ");
    scanf("%s", update_key);

    printf("Enter new value: ");
    scanf("%s", new_value);

    update_one(session, table_name, update_key, new_value);

    /* Delete */
    char delete_key[100];
    printf("Enter key to delete: ");
    scanf("%s", delete_key);

    delete_one(session, table_name, delete_key);

    /* Scan */
    char scan_table_name[100];
    printf("Enter the name of the table to scan: ");
    scanf("%s", scan_table_name);

    ret = scan_forward(session, scan_table_name);

    /* Scan reverse */
    ret = scan_reverse(session, scan_table_name);

    /* Count Rows */
    char count_table_name[100];
    printf("Enter the name of the table to count rows: ");
    scanf("%s", count_table_name);

    count_rows(session, count_table_name);

    /* Create an index */
    char index_table_name[100];
    char index_name[100];
    char column[100];

    printf("Enter table name to create an index: ");
    scanf("%s", index_table_name);

    printf("Enter index name: ");
    scanf("%s", index_name);

    printf("Enter column name for the index: ");
    scanf("%s", column);

    create_index(session, index_table_name, index_name, column);

    /* Get Indexes */
    get_indexes(session, index_table_name);

    /* Drop a Index */
    drop_index(session, index_table_name, index_name);
    
    /* Drop a table*/
    char drop_table_name[100];
    printf("Enter table name to drop: ");
    scanf("%s", drop_table_name);

    drop_table(session, drop_table_name);

    conn->close(conn, NULL);
 
    return (EXIT_SUCCESS);
}