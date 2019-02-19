#include <mysql.h>
#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

MYSQL *conn, mysql;
MYSQL_RES *res;
MYSQL_ROW row;

int query_state;

int main()
{
   const char *server="localhost";
   const char *user="kangwon";
   const char *password="";
   const char *database="testdb";

   mysql_init(&mysql);
   conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
   if(conn==NULL)
   {
       cout<<mysql_error(&mysql)<<endl<<endl;
      return 1;
   }
   query_state=mysql_query(conn, "select * from mydata");
   if(query_state!=0)
   {
      cout<<mysql_error(conn)<<endl<<endl;
      return 1;
   }
   
   res=mysql_store_result(conn);

   string instrument="INSERT INTO mydata VALUES (1,'hen',22)";
   
   query_state=mysql_query(conn, instrument.c_str());
   

   if(query_state!=0)
   {
      cout<<mysql_error(conn)<<endl<<endl;
      return 1;
   }
   cout<<endl<<endl;

   mysql_free_result(res);
   mysql_close(conn);

   return 0;
}