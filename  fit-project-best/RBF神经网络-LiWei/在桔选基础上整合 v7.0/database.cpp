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
    // 初始化驱动  
    driver = sql::mysql::get_mysql_driver_instance(); 

	con=driver->connect("tcp://127.0.0.1:3306","root","123456");
	state = con->createStatement();  
    state->execute("use fitdb");  
    // 查询  
    result = state->executeQuery("select * from user");  
    // 输出查询  
    while(result->next())  
    {  
        string id = result->getString("userName");   
		cout << id  << endl;  
    }
	state->execute("insert into user values('rf',123,00092,888888,10000.0,10000.0)");
    delete state;  
    delete con;

}*/