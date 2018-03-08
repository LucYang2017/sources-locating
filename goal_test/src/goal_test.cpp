#include <ros/ros.h>
#include <costmap_2d/cost_values.h>
#include <costmap_2d/costmap_2d_ros.h>
#include <tf/transform_listener.h>
char printableCost(unsigned char cost)
    {
    switch (cost)
        {
        case costmap_2d::NO_INFORMATION: return '?';
        case costmap_2d::LETHAL_OBSTACLE: return 'L';
        case costmap_2d::INSCRIBED_INFLATED_OBSTACLE: return 'I';
        case costmap_2d::FREE_SPACE: return '.';
        default: return '0' + (unsigned char) (10 * cost / 255);
        }
    }

int main(int argc, char *argv[])
{
    ros::init(argc, argv, "test_costmap");
    tf::TransformListener listener;
    try
    {
        listener.waitForTransform("/odom", "/base_footprint", ros::Time(), ros::Duration(2.0) );
    }
    catch(tf::TransformException& ex)
    {
        ROS_ERROR("%s", ex.what());
    }
    
    costmap_2d::Costmap2DROS* costmap_ros_ = new costmap_2d::Costmap2DROS("local_costmap",listener);
    costmap_2d::Costmap2D* costmap_;
    costmap_ = costmap_ros_->getCostmap();
    unsigned int mx,my;
    double x,y;
    x=-1.8;
    y=1.4;
    costmap_->worldToMap(x,y,mx,my);
    unsigned char c=costmap_->getCost(mx,my);
    char i='i';
    //i=printableCost(c);

    while(1){    
       //ROS_INFO("%d!",costmap->getSizeInCellsX());   
    }
  return 0;
}