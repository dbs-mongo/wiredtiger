void drop_table(WT_SESSION *session, char *table_name);
void show_tables(WT_CONNECTION *conn);
WT_CURSOR* create_table(WT_CONNECTION *conn, WT_SESSION *session, char *table_name, char *config);
