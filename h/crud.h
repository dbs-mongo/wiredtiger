void update_one(WT_SESSION *session, char *table_name, char *key, void *value);
int scan_forward(WT_SESSION *session, char *table_name);
int scan_reverse(WT_SESSION *session, char *table_name);
int find(WT_SESSION *session, char *table_name, int id);
void insert_many(WT_SESSION *session, char *table_name, int num_rows);
void insert_one(WT_SESSION *session, char *table_name, int id, char *value);
void delete_one(WT_SESSION *session, char *table_name, char *key);