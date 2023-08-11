#include <wiredtiger.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void create_index(WT_SESSION *session, char *table_name, char *index_name, char *column)
{
    /* Create an index */
    char index_spec[100];
    sprintf(index_spec, "index:%s:%s", table_name, index_name);

    char index_options[100];
    sprintf(index_options, "columns=(%s)", column);

    int ret = session->create(session, index_spec, index_options);
    if (ret != 0) {
        fprintf(stderr, "Failed to create index: %s\n", wiredtiger_strerror(ret));
        return;
    } else {
        printf("Index %s created successfully on table %s for column %s.\n", index_name, table_name, column);
    }
}

void drop_index(WT_SESSION *session, char *table_name, char *index_name)
{
    /* Drop an index */
    char index_spec[100];
    sprintf(index_spec, "index:%s:%s", table_name, index_name);

    int ret = session->drop(session, index_spec, NULL);
    if (ret != 0) {
        fprintf(stderr, "Failed to drop index: %s\n", wiredtiger_strerror(ret));
    } else {
        printf("Index %s dropped successfully from table %s.\n", index_name, table_name);
    }
}

void get_indexes(WT_SESSION *session, char *table_name)
{
    /* Get all indexes for a table */
    WT_CURSOR *cursor;
    char index_spec[100];
    sprintf(index_spec, "index:%s:", table_name);

    int ret = session->open_cursor(session, "metadata:", NULL, NULL, &cursor);
    if (ret != 0) {
        fprintf(stderr, "Failed to open cursor: %s\n", wiredtiger_strerror(ret));
        return;
    }

    cursor->set_key(cursor, index_spec);
    ret = cursor->search(cursor);
    if (ret == 0) {
        do {
            const char *index_name;
            cursor->get_value(cursor, &index_name);
            printf("Index: %s\n", index_name);
        } while ((ret = cursor->next(cursor)) == 0);
    }

    if (ret != 0 && ret != WT_NOTFOUND) {
        fprintf(stderr, "Failed to get indexes: %s\n", wiredtiger_strerror(ret));
    }

    cursor->close(cursor);
}
