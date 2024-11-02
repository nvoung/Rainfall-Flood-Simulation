COMS3270 <br>
Nathan Voung <br>
nvoung@iastate.edu <br>
September 20th, 2024 <br>
<br>
<br>

<h1>Rainfall Flood Simulation</h1>

**Project Overview**: The Rainfall Flood Simulation has been developed in 3 different parts. These parts will come together in the end to create a simulation of rainfall on a given area. The project exists to simulate what would happen in certain areas of land given the amount of rain, topography of the land and many other factors that will be mentioned in the descriptions below. 

<h2>Getting Started</h2>
To get started you will have to install a few things. You will firstly need to download some type of linux system.

* Heres a link to download WSL for windows https://learn.microsoft.com/en-us/windows/wsl/install <br>

After that you will need some type of IDE to run the code on, I used Visual Studio.

* Heres a link to download Visual Studio https://visualstudio.microsoft.com/vs/

After that you will be ready to set up the coding environment.

<h2>Environment Setup</h2>
You will need to setup the coding environment in order to run the code for this simulation. I followed a tutorial on this setup written by my Professor James Lathrop. Here is a link to his tutorial that will help you get everything set up. 

* https://canvas.iastate.edu/courses/113309/pages/visual-studio-remote-debugging?module_item_id=6644627 


<h2>Part A</h2>

**Description**: Project 1 part a includes three main files, pointcloud.c, pointcloud.h and Makefile. The objective of this part was to write a program that will read three tuples from example files that represent the grid position x and y and the elevation. The goal of the program is to read this file through standard input and return text stating the location and height of the minimum and maximum points. The code also returns the average height between all the data points that were read from the file. The Makefile produces an output named stat which is used to execute the code using the command ./stat < filename. 

<h3> Usage</h3>
To use the executable that you have created navigate to your WSL window and cd into your working repository. When you are in this repo you should be able to type make and it will create the stat exuectable. From here you can type the following comman "./stat < File_name which will run the executable with the given file and return the maximum height, minimum height and the average height of all the data points in the file.

<h2>Part B</h2>

**Description**: Project 1 part b includes multiple new files, util.c, util.h, display.c, parta.c, and updates were made to current files pointcloud.c, pointcloud.h and Makefile. We also added in two prewritten files bmp.c and bmp.h. There were multiple objectives of this part that would overall lead to creating an out.gif image that showed an elevation map. <br>
* **Util.c** contained the list structure functions and an allocate array function. This file can be used to find how the listinit, listaddend and listget functions work.
* **Util.h** contained the prototypes for util.c and it also contained a typdef struct for list and pcd_t.
* **display.c** included the main function for calling the executable ./display and entering the file_name which looks like ./display <file_name>.
* **parta.c** was a placeholder for the main method that we made for pointcloud.c in part a
* **bmp.c and bmp.h** are taken from https://github.com/wernsey/bitmap more info can be found here about them.
* **pointcloud.c** was given an update and the main method was removed. In turn we also added a ReadpointcloudData function which would read data from a file and we added our ImagepointCloud function which would take our list structure and read data into it from a file. It then uses that data to calculate RGB values for the pixels of the individual x y coordinates of the file and uses the bmp.c funtions to create an image. 
* **MakeFile** the makefile was also given an update to create two executables, display and stat. 

<h3> Usage</h3>

Again to get your executables navigate to your WSL and the working directory and type in make. You can still execute stat using ./stat < <file_name> but now you will also have the option to execute display using ./display <file_name> this should create you an out.gif that takes the data from the file you entered and shows a topographical map. If the file is invalid it will produce an error.

<h2>Part C</h2>

BEST RUN I HAD WHEN TESTING CODE: ./watershed testb.xyz 10000 2.0 0.1 0.2 output 1.0 1000

