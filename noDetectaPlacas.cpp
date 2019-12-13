#include "ros/ros.h"
#include "std_msgs/String.h"
#include "DetectarPlacas.h"

DetectarPlaca dp;
ros::Publisher entradaArduino;
int count = 0;
bool feliz=false;
ros::Publisher entradaBanco;
std_msgs::String saidaArduino;
/**
 * This tutorial demonstrates simple receipt of messages over the ROS system.
 */
void detectaPlaca()
{
	std_msgs::String msg2;
	std::stringstream ss;
	std::string s;
        
	if(dp.detectarPlaca(s)){	
		msg2.data = s;
		ROS_INFO("%s", msg2.data.c_str());
		entradaBanco.publish(msg2);	
	}	
}

void abreCancela(const std_msgs::String::ConstPtr& msg){
	
  ROS_INFO("I heard: [%s]", msg->data.c_str());
  	if (msg->data.compare("A")==0){
		saidaArduino.data="A";
		ROS_INFO("I heard: [%s]", saidaArduino.data.c_str());		
 		entradaArduino.publish(saidaArduino);
	}
 	if(msg->data.compare("F")==0){
    		saidaArduino.data="F";
		entradaArduino.publish(saidaArduino);
	}	

}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "noIdentifica_sinalBanco_sinalArduino");
  ros::NodeHandle n;
  ros::Subscriber sub = n.subscribe("saidaBanco", 1000, abreCancela);
//  ros::Subscriber sub2 = n.subscribe("entradaImagem", 1000, detectaPlaca);

  entradaArduino = n.advertise<std_msgs::String>("entradaArduino", 1000);
  entradaBanco = n.advertise<std_msgs::String>("entradaBanco", 1000);
//  ros::spin();

  ros::Rate loop_rate(1);

  while (ros::ok())
  {
    std_msgs::String msg;

    detectaPlaca();
    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}

