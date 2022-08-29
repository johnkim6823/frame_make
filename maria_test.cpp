#include <stdio.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <string>
#include <mysql.h>
using namespace std;

#define DB_IP "127.0.0.1"

struct db_user {
  char *server;
  char *user;
  char *password;
  char *database;
  string table;
};

MYSQL* mysql_connection_setup(struct db_user sql_user){
  MYSQL *connection = mysql_init(NULL);

  if(!mysql_real_connect(connection, sql_user.server, sql_user.user, sql_user.password, sql_user.database, 0, NULL, 0)) {
    printf("Connection error : %s\n", mysql_error(connection));
    exit(1);
  }

  return connection;
}

MYSQL_RES* mysql_perform_query(MYSQL *connection, char *sql_query) {
  if(mysql_query(connection, sql_query)){
    printf("MYSQL query error : %s\n", mysql_error(connection));
    exit(1);
  }

  return mysql_use_result(connection);
}

int main(){
  MYSQL *conn;
  MYSQL_RES *res;
  MYSQL_ROW row;

  string CID = "test CID2";
  string Hash = "test Hash";
  string ver = "0";
  
  string sorder;

  char *order;

  struct db_user mysqlID;
  mysqlID.server = DB_IP;
  mysqlID.user = "hanium";
  mysqlID.password = "1234";
  mysqlID.database = "hanium";
  mysqlID.table = "CID_table";

  conn = mysql_connection_setup(mysqlID); //sql 연결

  sorder = "insert into " + mysqlID.table + " values('" + CID + "', '" + Hash + "', " + ver + ")";

  order = new char[sorder.length() + 1];
  strcpy(order, sorder.c_str());

  res = mysql_perform_query(conn, "show tables"); //sql에 2번째 인자를 실행하라고 전달
  printf("MySQL Tables in mysql database : ");
  while((row = mysql_fetch_row(res)) != NULL)
    printf("%s\n", row[0]);
  res = mysql_perform_query(conn, order);
  
  mysql_free_result(res);
  mysql_close(conn);
  return 0;
}