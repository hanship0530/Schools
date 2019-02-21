#include <mysql.h> 
#include <pthread.h>
#include <mutex> // mux
#include <stdlib.h> // rand
#include <time.h> // time
#include <thread>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <unistd.h> // sleep

#define NUMOFDATAS 100000

using namespace std;
std::mutex mtx;

MYSQL *conn, mysql;
MYSQL_RES *res;
MYSQL_ROW row;

struct Person {
    string id;
    string name;
    string age;
};

Person person[NUMOFDATAS];

int query_state;
bool keepGoing=true;

void makeArrayDataThread() {
    cout<<"Make Array Thread Start"<<endl;
    //pthread_setname_np(pthread_self(), "makeArrayDataThread");
    srand (time(NULL));
    for(int index=0; index<NUMOFDATAS; index+=10){
        for(int subIndex=index; subIndex<index+10; subIndex++) {
            person[subIndex].id = to_string(subIndex);
            person[subIndex].name = "testName"+to_string(subIndex);
            person[subIndex].age = to_string(rand()%20+20);
        }
        sleep(2);
    }
    cout<<"Make Array Thread End"<<endl;
}
void insertQueryThread() {
    cout<<"Insert Thread Start"<<endl;
    //pthread_setname_np(pthread_self(), "insertQueryThread");
    string query;
    for(int index=0; index<NUMOFDATAS; index+=10){
        for(int subIndex=index; subIndex<index+10; subIndex++) {
            while(person[subIndex].id.empty()) {
                
            }
            query = "insert into mydata values ('"+person[subIndex].id+"','"+person[subIndex].name+
            "','"+person[subIndex].age+"')";
            query_state=mysql_query(conn, query.c_str());
            if(query_state!=0) {
                cout<<"Insert Error: "<<mysql_error(conn)<<endl<<endl;
            }
            res=mysql_store_result(conn);
            sleep(0.5);
        }
    }
    cout<<"Insert Thread End"<<endl;
    keepGoing = false;
}
int main()
{
   const char *server="localhost";
   const char *user="kangwon";
   const char *password="";
   const char *database="testdb";
   
   mysql_init(&mysql);
   
   conn=mysql_real_connect(&mysql, server, user, password, database, 0, 0, 0);
   
   thread makeArrayThread(makeArrayDataThread);
   thread insertThread(insertQueryThread);
   
   if(conn==NULL)
   {
       cout<<"Connect Error: "<<mysql_error(&mysql)<<endl<<endl;
       return 1;
   }
   
//   query_state=mysql_query(conn, "select * from mydata");
//   
//   if(query_state!=0)
//   {
//      cout<<"Select Query Error: "<<mysql_error(conn)<<endl<<endl;
//   }
//   
//   res=mysql_store_result(conn);
//   
//   cout<<"MySQL Values in the Mydata Table."<<endl<<endl;
//   while((row=mysql_fetch_row(res))!=NULL)
//   {
//      cout<<left;
//      cout<<setw(18)<<row[0]
//          <<setw(18)<<row[1]
//          <<setw(18)<<row[2]<<endl;
//   }
//   cout<<endl<<endl;
   
   //res=mysql_store_result(conn);
   
   makeArrayThread.detach();
   insertThread.detach();
   
   while(keepGoing) {
       
   }
   
   mysql_free_result(res);
   mysql_close(conn);
   
   return 0;
}