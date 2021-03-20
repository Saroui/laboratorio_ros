#include <ros/ros.h>
#include <std_srvs/Empty.h>

int main(int argc,char **argv){

  ros::init(argc, argv, "color");
  ros::NodeHandle nh;
  
  ros::service::waitForService("clear");
  
  ros::param::set("background_r", 39);
  ros::param::set("background_g", 174);
  ros::param::set("background_b", 96);
  
  ros::ServiceClient clearClient = nh.serviceClient<std_srvs::Empty>("/clear");
  std_srvs::Empty srv;
  clearClient.call(srv);
}
