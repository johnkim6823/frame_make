
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <cstring>

struct db_user {
	char *server;
	char *user;
	char *password;
	char *database;
	std::string table;
}mysqlID;

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
string table_name;

void insert_database(char* CID, char* Hash, char* Signed_Hash);
MYSQL* mysql_connection_setup(struct db_user sql_user);
MYSQL_RES* mysql_perform_query(MYSQL *connection, char *sql_query);
void create_table();

void initDatabase(struct db_user *db_info){
	db_info->server = DB_IP;
	db_info->user = DB_user;
	db_info->password = DB_password;
	db_info->database = DB_database;
	db_info->table = table_name;
}

MYSQL* mysql_connection_setup(struct db_user sql_user){
  MYSQL *connection = mysql_init(NULL);

  if(!mysql_real_connect(connection, sql_user.server, sql_user.user, sql_user.password, sql_user.database, 0, NULL, 0)) {
    printf("Connection error : %s\n", mysql_error(connection));
    exit(1);
  }

  return connection;
}

MYSQL_RES* mysql_perform_query(MYSQL *connection, char *sql_query) {
	int retry_cnt = 5;
	while(mysql_query(connection, sql_query) != 0){
		if(retry_cnt-- == 0)
			break;
		create_table();
	}
  return mysql_use_result(connection);
}

void insert_database(char* CID, char* Hash, char* Signed_Hash){
	string sorder = "INSERT INTO " + mysqlID.table + " values('" + CID + "', '" + Hash + "', '" + Signed_Hash + "' ,0);";	
	cout << sorder << endl;
	char *order = new char[sorder.length() + 1];
	strcpy(order, sorder.c_str());
	res = mysql_perform_query(conn, order);
}



void create_table(){
	string sorder = "CREATE TABLE " + table_name + "(CID VARCHAR(24), Hash VARCHAR(64), Signed_Hash VARCHAR(350), Verified INTEGER);";
	char *order = new char[sorder.length() + 1];
	strcpy(order, sorder.c_str());
	mysql_query(conn, order);
	res = mysql_use_result(conn);
}

void init_DB(){
    initDatabase(&mysqlID);
	conn = mysql_connection_setup(mysqlID);
}

void term_database(){
    mysql_free_result(res);
	mysql_close(conn);
}
