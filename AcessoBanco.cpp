#include "ros/ros.h"
#include "std_msgs/String.h"
#include <mysql/mysql.h>
#include <sstream>
#include<string>


  MYSQL *connection; 
  MYSQL_RES *result;  
  MYSQL_ROW row; 
  std_msgs::String saida;

  std_msgs::String msg;
	ros::Publisher chatter_pub;

	using namespace std;
void chatterCallback(const std_msgs::String::ConstPtr& msg)
{
 
  string sql;
	sql="SELECT * FROM placas where placa='";
	sql+=msg->data;
	sql+="'";

  if(mysql_query(connection, sql.c_str()))  
    {  
        ROS_INFO("Query Error: %s", mysql_error(connection));  
        exit(1);  
    } 
   else  
    {  

        result = mysql_use_result(connection); 
				ROS_INFO("%d", mysql_field_count(connection));
        for(int i=0; i < mysql_field_count(connection); ++i)  
        {
            
	    std::stringstream ss;  
            row = mysql_fetch_row(result);  
            if(row <= 0)  	
            {  
		saida.data="F";
		ROS_INFO("%s", "F");
		
		chatter_pub.publish(saida);
                break;  
            }
              
            for(int j=0; j < mysql_num_fields(result); ++j)  
            { 
		ROS_INFO("%s", "A");
		saida.data="A";
		chatter_pub.publish(saida);
                ss << row[j] << " ";
		//msg.data = ss.str().c_str();  
            }  
            ROS_INFO("%s", ss.str().c_str());  
        }
        mysql_free_result(result);  
    } 
ROS_INFO("%s", msg->data.c_str());
}

int main(int argc, char **argv)
{


  ros::init(argc, argv, "noBanco");  
  ros::NodeHandle n;  
  chatter_pub = n.advertise<std_msgs::String>("saidaBanco", 1000);

  connection = mysql_init(NULL);
  connection = mysql_real_connect(connection, "localhost", "root", "root", "placas_BD", 0, NULL, 0);
  
	//string sql;
	//sql="SELECT * FROM placas where placa=";
	//sql+=;
	//ROS_INFO("Query Error: %s", sql.c_str()); 

	//chatter_pub.publish(msg);
        ros::Subscriber sub = n.subscribe("entradaBanco", 1000, chatterCallback);

  ros::Rate loop_rate(10);
	ros::spin();  
  return 0;
}


