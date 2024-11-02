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
<br>

**Pointcloud.c**: At this point there was only one method in pointcloud.c which was stat1() this function uses a struct defined in pointcloud.h called Point which holds three double variables x, y, z. stat1() will be used to read a file that contains data in the format x-coordinate, y-coordiante, height. stat1() scans the file and assigns points to a variable Point and from there compares them to each other in a series of if statements to find the minimum point and the maximum point. After it has read through all the data in the file it will calculate the average height and print out the maxPoints Height and position, the minPoints Height and position and will print the average height of all the points.

**Pointcloud.h**: This holds the prototype for stat1() in pointcloud.c and it also holds the struct def for point.

**MakeFile**: <br>
stat: pointcloud.o <br>
	    gcc -g pointcloud.o -o stat <br>
pointcloud.o: pointcloud.c <br>
	gcc -g -c pointcloud.c <br>
clean: <br>
	rm *.o stat

<h3> Usage</h3>
To use the executable that you have created navigate to your WSL window and cd into your working repository. When you are in this repo you should be able to type make and it will create the stat exuectable. From here you can type the following comman "./stat < File_name which will run the executable with the given file and return the maximum height, minimum height and the average height of all the data points in the file.

<h2>Part B</h2>

**Description**: Project 1 part b includes multiple new files, util.c, util.h, display.c, parta.c, and updates were made to current files pointcloud.c, pointcloud.h and Makefile. We also added in two prewritten files bmp.c and bmp.h. There were multiple objectives of this part that would overall lead to creating an out.gif image that showed an elevation map. <br>

* **Util.h** contains a struct def on pcd_t which is like Point from part a in all aspects but will include some extra information that will be used for part c. It also contains a struct def List which will be used to implement the following methods <br>
int listInit(List* l, int max_elmt_size);<br>
void listAddEnd(List* l, void* elmt);<br>
void* listGet(List* l, int index);

* **Util.c** contained the list structure functions and an allocate array function. <br>
void *allocateArray(int rows, int columns): This function allocates a 2-d array using Calloc to initialize all points in said array to 0<br>
int listInit(List* l, int max_elmt_size): This function initializes the List array to be used. Defualted to 10<br>
void listAddEnd(List* l, void* elmt): This function is used to add an element to the end of the list. It uses a malloc call allocate double the space of the list in memory if there is no space and then uses memcpy to copy the old data to the new list<br>
void* listGet(List* l, int index): This function takes an index argument and will return the lists data at the given index<br>


* **display.c**: this file has a main method int main(int argc, char* argv[]). This then allows us to run the
  executable as follows ./display and entering the file_name which looks like ./display <file_name>.

* **parta.c** was a placeholder for the main method that we made for pointcloud.c in part a

* **bmp.c and bmp.h** are taken from https://github.com/wernsey/bitmap more info can be found here about them.

* **pointcloud.c** was given an update and the main method was removed. In turn we also added a ReadpointcloudData function which would read data from a file and we added our ImagepointCloud function. <br>
void readPointCloudData(FILE* stream, int* rasterWidth, List* pc): This function was designed very similar to stat1() but we read from a file that gives us a width of the data in the file. The function then Initializes a list given the list in the argument and reads the pcd_t data into the list <br>
void imagePointCloud(List* l, int width, char* filename): This function will iterate through the points in the list and find the minHeight and maxheight. Then we calculate the height of the files based on the width and create our bitmap. We then iterate throught the list again and assign each point with a RGB value and set the pixel and its color on the map. <br>

* **MakeFile** the makefile was also given an update to create two executables, display and stat. 

<h3> Usage</h3>

Again to get your executables navigate to your WSL and the working directory and type in make. You can still execute stat using ./stat < <file_name> but now you will also have the option to execute display using ./display <file_name> this should create you an out.gif that takes the data from the file you entered and shows a topographical map. If the file is invalid it will produce an error.

<h2>Part C</h2>

