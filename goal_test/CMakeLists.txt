cmake_minimum_required(VERSION 2.8.3)
project(goal_test)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  tf
  message_generation
  roscpp
  std_msgs
  geometry_msgs
  nav_msgs
  tf
  serial
  costmap_2d
  move_base_msgs

)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
add_message_files(FILES
  experiment_data.msg
  alcohol_concentration.msg
)

## Generate services in the 'srv' folder
add_service_files(FILES
  sensor_to_goal.srv
)

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
generate_messages(DEPENDENCIES
  std_msgs
  geometry_msgs
  move_base_msgs

)

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  CATKIN_DEPENDS
  message_runtime
#  INCLUDE_DIRS include
#  LIBRARIES goal_test
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
# add_library(goal_test
#   src/${PROJECT_NAME}/goal_test.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(goal_test ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
# add_executable(goal_test_node src/goal_test_node.cpp)

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(goal_test_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(goal_test_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS goal_test goal_test_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )



add_executable(goal_test
  src/goal_test.cpp
)
add_dependencies(goal_test ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(goal_test
  ${catkin_LIBRARIES}
)

add_executable(fake_sensor
  src/fake_sensor.cpp
)
add_dependencies(fake_sensor ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(fake_sensor
  ${catkin_LIBRARIES}
)

add_executable(temp_sensor_node
  src/temp_sensor_node.cpp
)
add_dependencies(temp_sensor_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(temp_sensor_node
  ${catkin_LIBRARIES}
)

add_executable(simple_navigation_goal
  src/simple_navigation_goal.cpp
)
add_dependencies(simple_navigation_goal ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(simple_navigation_goal
  ${catkin_LIBRARIES}
)

add_executable(goal_test3_with_actionlib
  src/goal_test3_with_actionlib.cpp
)
add_dependencies(goal_test3_with_actionlib ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(goal_test3_with_actionlib
  ${catkin_LIBRARIES}
)

add_executable(test_with_actionlib
  src/test_with_actionlib.cpp
)
add_dependencies(test_with_actionlib ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(test_with_actionlib
  ${catkin_LIBRARIES}
)


add_executable(test_with_actionlib_2_sensor
  src/test_with_actionlib_2_sensor.cpp
)
add_dependencies(test_with_actionlib_2_sensor ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(test_with_actionlib_2_sensor
  ${catkin_LIBRARIES}
)

add_executable(filerw
  src/filerw.cpp
)
add_dependencies(filerw ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(filerw
  ${catkin_LIBRARIES}
)

add_executable(collector
  src/collector.cpp
)
add_dependencies(collector ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(collector
  ${catkin_LIBRARIES}
)

add_executable(experiment_data_publisher
  src/experiment_data_publisher.cpp
)
add_dependencies(experiment_data_publisher ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(experiment_data_publisher
  ${catkin_LIBRARIES}
)