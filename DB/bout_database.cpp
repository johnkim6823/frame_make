
#include <stdio.h>
#include <stdlib.h>
#include <mysql.h>
#include <cstring>
#include <vector>

MYSQL *conn;
MYSQL_RES *res;
MYSQL_ROW row;
string table_name;

struct db_user {
	char *server;
	char *user;
	char *password;
	char *database;
	std::string table;
};

struct db_user mysqlID;

void insert_database(char* CID, char* Hash, char* Signed_Hash);
void insert_pk_database(string key_ID, char* key_value);
void get_CID_list(vector<string> &CID_list, string first_cid, string last_cid);
string get_latest_key_ID(char* order);
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
	string sorder = "INSERT INTO " + table_name + " values('" + CID + "', '" + Hash + "', '" + Signed_Hash + "' ,0);";
	char *order = new char[sorder.length() + 1];
	strcpy(order, sorder.c_str());
	res = mysql_perform_query(conn, order);
}

void insert_pk_database(string key_ID, char* key_value){
	string sorder = "INSERT INTO public_key values('" + key_ID + "', '" + key_value + "', 1);";
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

string get_latest_key_ID(char* order){
	res = mysql_perform_query(conn, order);
	string key_ID;
	while((row = mysql_fetch_row(res)) != NULL){
		key_ID = row[0];
		cout << "key_ID : " << key_ID << endl;
	}
	
	return key_ID;
}

void update_database(char* order){
	res = mysql_perform_query(conn, order);
	cout << endl << "---------------------------------------------------" << endl;
	while((row = mysql_fetch_row(res)) != NULL){
		string x = row[0];
		cout << x << endl;
	}
}

void get_CID_list(vector<string> &CID_list, string table, string first_cid, string last_cid){
	char *order;
	string sorder = "select CID from " + table + " where key_ID < '" + first_cid + "' order by key_ID desc limit 1;";
	order = new char[sorder.length() + 1];
	strcpy(order, sorder.c_str());
	res = mysql_perform_query(conn, order);
	while((row = mysql_fetch_row(res)) != NULL){
		CID_list.push_back(row[0]);
	}
}

void init_DB(struct db_user mysqlID){
    initDatabase(&mysqlID);
	conn = mysql_connection_setup(mysqlID);
}

void term_database(){
    mysql_free_result(res);
	mysql_close(conn);
}
