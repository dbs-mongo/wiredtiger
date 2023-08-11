void get_indexes(WT_SESSION *session, char *table_name);
void drop_index(WT_SESSION *session, char *table_name, char *index_name);
void create_index(WT_SESSION *session, char *table_name, char *index_name, char *column);