#include <iostream>
#include <mysql_connection.h>  
#include <mysql_driver.h>  
#include <statement.h>
using namespace sql; 
using namespace std;
/*
int main()
{
    mysql::MySQL_Driver *driver;  
    Connection *con;  
    Statement *state;  
    ResultSet *result;  
    // ��ʼ������  
    driver = sql::mysql::get_mysql_driver_instance(); 

	con=driver->connect("tcp://127.0.0.1:3306","root","123456");
	state = con->createStatement();  
    state->execute("use fitdb");  
    // ��ѯ  
    result = state->executeQuery("select * from user");  
    // �����ѯ  
    while(result->next())  
    {  
        string id = result->getString("userName");   
		cout << id  << endl;  
    }
	state->execute("insert into user values('rf',123,00092,888888,10000.0,10000.0)");
    delete state;  
    delete con;

}*/