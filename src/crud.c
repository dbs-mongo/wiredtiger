#include <wiredtiger.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_all_columns(WT_SESSION *session){

}

void update_one(WT_SESSION *session, char *table_name, char *key, void *value) {
    WT_CURSOR *cursor;
    char cursor_spec[100];
    int ret; 
    sprintf(cursor_spec, "table:%s", table_name);
    ret = session->open_cursor(session, cursor_spec, NULL, NULL, &cursor);
    if (ret != 0) {
        fprintf(stderr, "Failed to open cursor: %s\n", wiredtiger_strerror(ret));
    }

    cursor->set_key(cursor, key);
    cursor->set_value(cursor, value);

    ret = cursor->update(cursor);
    if (ret != 0) {
        fprintf(stderr, "Failed to update data: %s\n", wiredtiger_strerror(ret));
    } else {
        printf("Update successful!\n");
    }

    cursor->close(cursor);
}

int scan_forward(WT_SESSION *session, char *table_name)
{
    WT_CURSOR *cursor;
    char cursor_spec[100];
    sprintf(cursor_spec, "table:%s", table_name);
    int ret = session->open_cursor(session, cursor_spec, NULL, NULL, &cursor);
    if (ret != 0) {
        fprintf(stderr, "Failed to open cursor: %s\n", wiredtiger_strerror(ret));
    }

    char *key = NULL;
    char *value = NULL;

    while ((ret = cursor->next(cursor)) == 0) {
        if ((ret = cursor->get_key(cursor, &key)) != 0) {
            fprintf(stderr, "Failed to get key: %s\n", wiredtiger_strerror(ret));
            cursor->close(cursor);
        }

        if ((ret = cursor->get_value(cursor, &value)) != 0) {
            fprintf(stderr, "Failed to get value: %s\n", wiredtiger_strerror(ret));
            cursor->close(cursor);
        }

        printf("Key: %s, Value: %s\n", key, value);
    }

    if (ret != WT_NOTFOUND) {
        fprintf(stderr, "Failed to scan data: %s\n", wiredtiger_strerror(ret));
        cursor->close(cursor);
    }

    return 0;
}

int scan_reverse(WT_SESSION *session, char *table_name)
{
    const char *key, *value;
    WT_CURSOR *cursor;
    char cursor_spec[100];
    sprintf(cursor_spec, "table:%s", table_name);
    int ret = session->open_cursor(session, cursor_spec, NULL, NULL, &cursor);

    if (ret != 0) {
        fprintf(stderr, "Failed to open cursor: %s\n", wiredtiger_strerror(ret));
        cursor->close(cursor);
    }
 
    while ((ret = cursor->prev(cursor)) == 0) {
        if ((ret = cursor->get_key(cursor, &key)) != 0) {
        fprintf(stderr, "Failed to get key: %s\n", wiredtiger_strerror(ret));
        cursor->close(cursor);
        return ret;
        }

        if ((ret = cursor->get_value(cursor, &value)) != 0) {
        fprintf(stderr, "Failed to get value: %s\n", wiredtiger_strerror(ret));
        return ret;
        }

        printf("Key: %s, Value: %s\n", key, value); 
    }

    if (ret != WT_NOTFOUND) {
        fprintf(stderr, "Failed to scan data: %s\n", wiredtiger_strerror(ret));
        cursor->close(cursor);
        return ret;
    }

    return 0;
}

int find(WT_SESSION *session, char *table_name, int id) {
    WT_CURSOR *cursor;
    char cursor_spec[100];
    sprintf(cursor_spec, "table:%s", table_name);

    int ret = session->open_cursor(session, cursor_spec, NULL, NULL, &cursor);
    if (ret != 0) {
        fprintf(stderr, "Failed to open cursor: %s\n", wiredtiger_strerror(ret));
        return ret;
    }

    const char *value;
    cursor->set_key(cursor, &id);

    ret = cursor->search(cursor);
    if (ret == 0) {
        ret = cursor->get_value(cursor, &value);
        if (ret == 0) {
            printf("Found: %s\n", value);
        } else {
            fprintf(stderr, "Failed to get value: %s\n", wiredtiger_strerror(ret));
        }
    } else if (ret == WT_NOTFOUND) {
        printf("Data not found for id: %d\n", id);
    } else {
        fprintf(stderr, "Failed to search data: %s\n", wiredtiger_strerror(ret));
    }

    cursor->close(cursor);
    return ret;
}

void index_scan(WT_SESSION *session, char *table_name, char *index_name, int id){
    WT_CURSOR *cursor;
    char index_spec[100];
    sprintf(index_spec, "index:%s:%s", table_name, index_name);

    int ret = session->open_cursor(session, index_spec, NULL, NULL, &cursor);
    if (ret != 0) {
        fprintf(stderr, "Failed to open cursor: %s\n", wiredtiger_strerror(ret));
        return ret;
    }

    const char *value;
    cursor->set_key(cursor, &id);

    ret = cursor->search(cursor);
    if (ret == 0) {
        ret = cursor->get_value(cursor, &value);
        if (ret == 0) {
            printf("Found: %s\n", value);
        } else {
            fprintf(stderr, "Failed to get value: %s\n", wiredtiger_strerror(ret));
        }
    } else if (ret == WT_NOTFOUND) {
        printf("Data not found for id: %d\n", id);
    } else {
        fprintf(stderr, "Failed to search data: %s\n", wiredtiger_strerror(ret));
    }

    cursor->close(cursor);
    return ret;
}

void insert_one(WT_SESSION *session, char *table_name, int id, char *value) {
    WT_CURSOR *cursor;
    char cursor_spec[100];
    int ret;
    sprintf(cursor_spec, "table:%s", table_name);
    ret = session->open_cursor(session, cursor_spec, NULL, NULL, &cursor);
    if (ret != 0) {
        fprintf(stderr, "Failed to open cursor: %s\n", wiredtiger_strerror(ret));
        return;
    }

    cursor->set_key(cursor, &id);
    cursor->set_value(cursor, value);

    ret = cursor->insert(cursor); // Reuse 'ret' variable here
    if (ret != 0) {
        fprintf(stderr, "Failed to insert data: %s\n", wiredtiger_strerror(ret));
    } else {
        printf("Data inserted successfully! Key: %d, Value: %s\n", id, value);
    }

    cursor->close(cursor);
}

void insert_many(WT_SESSION *session, char *table_name, int num_rows) {
    WT_CURSOR *cursor;
    char cursor_spec[100];
    sprintf(cursor_spec, "table:%s", table_name);

    int ret = session->open_cursor(session, cursor_spec, NULL, NULL, &cursor);
    if (ret != 0) {
        fprintf(stderr, "Failed to open cursor: %s\n", wiredtiger_strerror(ret));
        return;
    }

    for (int i = 0; i < num_rows; i++) {
        int id;
        char value[100];
        printf("Enter id for row %d: ", i + 1);
        scanf("%d", &id);

        printf("Enter value for row %d: ", i + 1);
        scanf("%s", value);

        insert_one(session, table_name, id, value);
    }

    cursor->close(cursor); // Close the cursor after the loop
}


void delete_one(WT_SESSION *session, char *table_name, char *key) {
    WT_CURSOR *cursor;
    char cursor_spec[100];
    int ret; 
    sprintf(cursor_spec, "table:%s", table_name);
    ret = session->open_cursor(session, cursor_spec, NULL, NULL, &cursor);
    if (ret != 0) {
        fprintf(stderr, "Failed to open cursor: %s\n", wiredtiger_strerror(ret));
    }
    
    cursor->set_key(cursor, key);
    ret = cursor->remove(cursor);
    if (ret != 0) {
        fprintf(stderr, "Failed to delete data: %s\n", wiredtiger_strerror(ret));
    } else {
        printf("Delete successful!\n");
    }

    cursor->close(cursor);
}

