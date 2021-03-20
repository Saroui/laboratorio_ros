#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/TeleportAbsolute.h>
#include <turtlesim/Pose.h>
#include <math.h>
#include <stdlib.h>
#include <iomanip>
#define  KV 1.5 //Constante de velocidad
#define  KO 3  //Constante de angulo
#define  PI 3.14159265359
//#define  TOL 0.1  //Tolerancia 

turtlesim::Pose pose;
turtlesim::Pose goal;
geometry_msgs::Twist velocity;
ros::Publisher *vel_pub;

void move_to_goal(turtlesim::Pose goal, double distance_tolerance);
double norm(turtlesim::Pose current, turtlesim::Pose goal);


void poseMessageReceived(const turtlesim::Pose &msg){
   pose.x = msg.x;
   pose.y = msg.y;
   pose.theta = msg.theta;
}


int main(int argc, char **argv) {
  
  double TOL = 1;

 // Inicialización y manejo del nodo
  ros::init(argc, argv, "navigation");
  ros::NodeHandle nh;
//Suscripción a la posición y publicación de la velocidad	
  ros::Subscriber sub =nh.subscribe("turtle1/pose", 1000, &poseMessageReceived);
  vel_pub = new ros::Publisher(nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel",1000));
  ros::Rate rate(0.5);
// Recibimiento de los parámetros de consola	
  const std::string Posx = "~posx";
  double posx;
  ros::Duration(1).sleep();
  bool ok= ros::param::get(Posx, posx);
  const std::string Posy = "~posy";
  double posy;
//Verificación del recibimiento de los parámetros
  if(!ok){
    ROS_FATAL_STREAM("ERROR (x)");
    exit(1);    
  }
  ok = ros::param::get(Posy, posy);
  
  if(!ok){
    ROS_FATAL_STREAM("ERROR (y)");
    exit(1);    
  }
	
 	
  //Almacenamiento y envío de la posición de meta.
  
  goal.x = posx;
  goal.y = posy;
 // goal.theta = 0;
  //ros::Rate rate(10);
  move_to_goal(goal, TOL);
  while(ros::ok()){
  	ros::spinOnce();	
  	rate.sleep();
  }
}



double norm(turtlesim::Pose current, turtlesim::Pose goal){ 
  return (sqrt(pow((goal.x - current.x),2) +pow((goal.y - current.y),2)));
}

void move_to_goal(turtlesim::Pose goal, double distance_tolerance){
  
  geometry_msgs::Twist vel_msg;
    
  ros::Rate loop_rate(10);
	
	
  //The while condition is to confirm if the turtle is close enough to the goal
  while((KV*norm(pose, goal))>(distance_tolerance)){
	  	//linear velocity
   	 	vel_msg.linear.x = KV*norm(pose, goal);
		//vel_msg.linear.y = 0;
		//vel_msg.linear.z = 0;
		//angular velocity
		//vel_msg.angular.x = 0;
		//vel_msg.angular.y = 0;
		vel_msg.angular.z = KO*(atan2(goal.y - pose.y, goal.x - pose.x)-pose.theta);
	  
	  	//turtle movement
		vel_pub->publish(vel_msg);
		ros::spinOnce();
		loop_rate.sleep();
  }
	//Reseting velocity
	vel_msg.linear.x = 0;
	vel_msg.angular.z = 0;
	//vel_pub->publish(vel_msg);
	//Showing position
	ROS_INFO_STREAM(std::setprecision(2) << std::fixed << "Posicion: (" << pose.x << "," << pose.y << ")" << "Angulo=" << pose.theta);
	exit(1);
}
