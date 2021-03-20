#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <turtlesim/TeleportAbsolute.h>

int main(int argc, char **argv){
    double velocity, theta;
    double rad_theta;
    
    ros::init(argc, argv, "rotar");
    ros::NodeHandle nh;
    ros::Publisher pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1000);
    ros::ServiceClient teleportClient = nh.serviceClient<turtlesim::TeleportAbsolute>("turtle1/teleport_absolute");
    
    turtlesim::TeleportAbsolute::Request req;
    turtlesim::TeleportAbsolute::Response res;
    
    const std::string PARAM_THETA = "~theta";
    const std::string PARAM_VEL = "~velocity";
    
    bool ok = ros::param::get(PARAM_THETA,theta);
    if(!ok){
        ROS_FATAL_STREAM("Introduzca el parametro del angulo por favor");
        exit(1);
    }
    
    ok = ros::param::get(PARAM_VEL,velocity);
    if(!ok){
        ROS_FATAL_STREAM("Introduzca el parametro de la velocidad angular por favor");
        exit(1);
    }
    
    rad_theta = theta*(3.141516/180);

    
    ros::Rate rate(2);
    geometry_msgs::Twist msg;
    double current_theta = 0;
   
    msg.angular.z = velocity;
    
    double t0 = ros::Time::now().toSec();
    
    while(current_theta < rad_theta){
        pub.publish(msg);
        
        double t1 = ros::Time::now().toSec();
        current_theta = velocity * (t1 - t0);
        
        ROS_INFO_STREAM("va desde el angulo" << current_theta << "a el angulo" << rad_theta);
    }
    
  ROS_INFO_STREAM("Ya termino de rotar");
  msg.angular.z = 0;
  pub.publish(msg);
  ros::spinOnce();
  rate.sleep();
}
