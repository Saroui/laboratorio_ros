#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/TeleportAbsolute.h>
#include "turtlesim/Pose.h"//para la posicion
#include <iomanip>


ros::Publisher pub;

int main(int argc, char **argv){
	//Velocidad con la que se movera la tortuga
	double velocity;
	ros::init(argc, argv, "Square");
    ros::NodeHandle nh;
	//Publica la velocidad
    pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);
    ros::ServiceClient teleportClient = nh.serviceClient<turtlesim::TeleportAbsolute>("turtle1/teleport_absolute");
	//Se suscribe al tema pose

	
    	turtlesim::TeleportAbsolute::Request req;
    	turtlesim::TeleportAbsolute::Response res;
    	
    	req.x = 1;
    	req.y = 1;
    	req.theta = 0;
    	
    	bool success = teleportClient.call(req, res);
    
	if(success){
	        ROS_INFO_STREAM("Da orientiacion");    
	}else {
        	ROS_INFO_STREAM("error");
    	}
   
   	const std::string PARAM_Velocity = "~velocity";

    	bool ok = ros::param::get(PARAM_Velocity,velocity);
   	if(!ok){
        	ROS_FATAL_STREAM("No se pudo recibir el parametro velocity");
        	exit(1);
    	}
		

    
    	ros::Rate rate(2);
    	geometry_msgs::Twist msg;
		
		//Distancia angular que va recorriendo al momento de girar
    	double current_theta = 0;
    	//Se define la velocidad con la que el robot se movera
		//Esta es recibida desde el terminal
    	
		double distancia = 0;
  		pub.publish(msg);
		
while(ros::ok()){
		double t0 = ros::Time::now().toSec();
		msg.linear.x= velocity;
	while(distancia<3){
		
		pub.publish(msg);
		double t1 = ros::Time::now().toSec();
    	distancia = velocity * (t1-t0);
	}
	
//Condicionales temporales para que gire a tenerminadas distancias
	//if(distancia>=3){
		//Instante de tiempo en el que comienza a girar
		
		
		
		//Como esta girando hacemos que sea 0 la velocidad lineal
		msg.linear.x=0;
		msg.angular.z = 1;
	    double t3 = ros::Time::now().toSec();
	    pub.publish(msg);
		
		while(current_theta <(1.57)){
				
	        	pub.publish(msg);
	        	double t2 = ros::Time::now().toSec();
	        	current_theta =(1)*(t2 - t3);
				ros::spinOnce();
				
		}//End while(current..
		//Al terminar de rotar reinicia la distancia angular
		current_theta=0;
		distancia=0;
		//ROS_INFO_STREAM("Ya termino de rotar");
		//Se detiene el giro igualando la velocidad angular a 0
  		msg.angular.z = 0;
		msg.linear.x=velocity;
        pub.publish(msg);
    	//}//ENd If(distancia..
  		//Nuevamente se da la señal de avanzar 
    	

		
		  
 }//End while(ok)
	ros::spin();
  
 
 }//End main
