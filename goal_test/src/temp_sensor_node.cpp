#include <ros/ros.h> 
#include <serial/serial.h>  //ROS已经内置了的串口包 
#include <std_msgs/String.h> 
#include <std_msgs/Empty.h>
#include <std_msgs/Float32.h> 
#include <string>
#include<iostream>
using namespace std;


  
serial::Serial ser; //声明串口对象 

/*static string hexStr2Str(string hexStr)    
{      
    string str = "0123456789ABCDEF";      
    char hexs[] = hexStr.toCharArray();      
    byte bytes[] = new byte[hexStr.length() / 2];      
    int n;      
  
    for (int i = 0; i < bytes.length; i++)    
    {      
        n = str.indexOf(hexs[2 * i]) * 16;      
        n += str.indexOf(hexs[2 * i + 1]);      
        bytes[i] = (byte) (n & 0xff);      
    }      
    return new String(bytes);      
} 
*/
//回调函数 
void write_callback(const std_msgs::String::ConstPtr& msg) 
{ 
    uint8_t request[8]={0x01,0x03,0x00,0x00,0x00,0x02,0xC4,0x0B};
    ROS_INFO_STREAM("Writing to serial port" <<msg->data); 
    ser.write(request,8);   //发送串口数据 
} 
  
int main (int argc, char** argv) 
{ 
    //初始化节点 
    ros::init(argc, argv, "serial_example_node"); 
    //声明节点句柄 
    ros::NodeHandle nh; 
  
    //订阅主题，并配置回调函数 
    ros::Subscriber write_sub = nh.subscribe("write", 1000, write_callback); 
    //发布主题 
    ros::Publisher temperature_pub = nh.advertise<std_msgs::Float32>("temperature", 10);
  
    try 
    { 
    //设置串口属性，并打开串口 
        ser.setPort("/dev/ttyUSB0"); 
        ser.setBaudrate(9600); 
        serial::Timeout to = serial::Timeout::simpleTimeout(1000); 
        ser.setTimeout(to); 
        ser.open(); 
    } 
    catch (serial::IOException& e) 
    { 
        ROS_ERROR_STREAM("Unable to open port "); 
        return -1; 
    } 
  
    //检测串口是否已经打开，并给出提示信息 
    if(ser.isOpen()) 
    { 
        ROS_INFO_STREAM("Serial Port initialized"); 
    } 
    else 
    { 
        return -1; 
    } 
  
    //指定循环的频率 
    ros::Rate loop_rate(1); 
    while(ros::ok()) 
    { 
        uint8_t request[8]={0x01,0x03,0x00,0x00,0x00,0x02,0xC4,0x0B};
        ROS_INFO_STREAM("Writing to serial port" <<request); 
        ser.write(request,8);   //发送串口数据 
        if(ser.available()){ 
            ROS_INFO_STREAM("Reading from serial port\n"); 
            std_msgs::String result; 
            result.data = ser.read(ser.available());
            int a[8];
            int i=0;
            for(i=0;i<8;i++)
               {
                   a[i]=(unsigned char)result.data[i];

                   //ROS_INFO_STREAM("Read: " << result.data[i]-result.data[i+1]);                   
               }
            //printf("%x\n",result.data[i]);
            //ROS_INFO_STREAM("Read: " << a[i]);
            std_msgs::Float32 temperature_msg;
            ROS_INFO_STREAM("Read:a5 " << a[5]);
            ROS_INFO_STREAM("Read:a6 " << a[6]);
            ROS_INFO_STREAM("1 " << (a[5]%100-a[5]%10)*16*16*16);
            ROS_INFO_STREAM("2 " << (a[5]%10)*16*16);
            ROS_INFO_STREAM("3 " << (a[6]%100-a[6]%10)*16);
            ROS_INFO_STREAM("4 " << a[6]%10);
            
            //aa=(a[5]%100-a[5]%10)*16*16*16/10+(a[5]%10)*16*16+(a[6]%100-a[6]%10)*16/10+a[6]%10;
            
            temperature_msg.data=a[5]*16*16+a[6]; 
            //ROS_INFO_STREAM("Read: " << aa);
            temperature_pub.publish(temperature_msg); 
        } 

        //处理ROS的信息，比如订阅消息,并调用回调函数 
        ros::spinOnce(); 
        loop_rate.sleep(); 
  
    } 
    
    int t=0x11,b;
    b=t;
    cout<<b;
} 