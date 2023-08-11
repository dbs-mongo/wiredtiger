#include <wiredtiger.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void count_rows(WT_SESSION *session, char *table_name)
{
    WT_CURSOR *cursor;
    char cursor_spec[100];
    sprintf(cursor_spec, "table:%s", table_name);

    int ret = session->open_cursor(session, cursor_spec, NULL, NULL, &cursor);
    if (ret != 0) {
        fprintf(stderr, "Failed to open cursor: %s\n", wiredtiger_strerror(ret));
        return;
    }

    uint64_t row_count = 0;
    while ((ret = cursor->next(cursor)) == 0) {
        row_count++;
    }

    if (ret != WT_NOTFOUND) {
        fprintf(stderr, "Failed to scan data: %s\n", wiredtiger_strerror(ret));
    }

    printf("Number of rows in table %s: %" PRIu64 "\n", table_name, row_count);

    cursor->close(cursor);
}