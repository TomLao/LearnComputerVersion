cmake_minimum_required(VERSION 3.12)
project(LearnComputerVersion)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

#lab1_demo1
#add_executable(LearnComputerVersion Course_Lab_1/openCVDemo1/main.cpp)
#lab1_demo2
#add_executable(LearnComputerVersion Course_Lab_1/openCVDemo2/main.cpp Course_Lab_1/openCVDemo2/opencvworker.cpp Course_Lab_1/openCVDemo2/opencvworker.h Course_Lab_1/openCVDemo2/qcvwidget.cpp Course_Lab_1/openCVDemo2/qcvwidget.h Course_Lab_1/openCVDemo2/qcvwidget.ui)

#lab2_demo1
#add_executable(LearnComputerVersion Course_Lab_2/Assignment1/main.cpp)
#lab2_assignament2
#add_executable(LearnComputerVersion Course_Lab_2/Assignment2/main.cpp Course_Lab_2/Assignment2/mainwindow.cpp Course_Lab_2/Assignment2/mainwindow.h Course_Lab_2/Assignment2/mainwindow.ui)

#calibration
#add_executable(LearnComputerVersion zhang-s-method/zhang-method/zhang-method.cpp)
#add_executable(LearnComputerVersion Camera_Calidration/calibration.cpp)
add_executable(LearnComputerVersion Camera_Calidration/lab3_calibration.cpp)

        find_package(Qt5Widgets)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(LearnComputerVersion ${OpenCV_LIBS}  Qt5::Widgets)