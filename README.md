# 3203 Assignment 2

Members:
 - Matt Mayer  (100969802)
 - Matt Drouin (100790860)


# Installation
--------------------------------------------------------------------------------
In order to run this program you must have a graphics card that supports openGL.

You must also have the Microsoft Visual C++ Redistributable for Visual Studio
2015 installed. Should you not have this installed, the exectuables are 
available in the /redist folder. They are also available to download from 
Microsoft's website at: 
    https://www.microsoft.com/en-us/download/details.aspx?id=48145

The 32-bit version must be installed, but both 32 and 64 are included.


# Running
--------------------------------------------------------------------------------
Once the program is run (3203-A2.exe, in the /bin folder), you can manipulate
the program as you like.

The GUI in the upper left allows the user to modify the parameters of the
simulation. 

Under Simulation Parameters:

	Number of Sensors - allows you to change the number of sensors (0-100), 
						represented by the white dots
	
	Diameter - the range of the sensor (0- 1.0), represented by the red circles
	
	Speed - the speed at which the simulation will run (0-100), not percentile
			1-5 is a decent speed to watch the simulation. 100 is for running
			multiple simulations.

Other Parameters:

	Refresh - resets the sensors to their original positions (the positions
			  from the last time Reset was called)
			  
	Reset - resets the sensors and chooses new random initial positions
	
	Run Simulation - this will run the simulation once, until the area is under
					 complete coverage, sum of movements is output to the
					 console.
	Run Simulation MULTIPLE - this will run the simulation multiple times (as
							  set by Num. Sims.), it will output the sum of
							  movements for each simulation, as well as the
							  average sum of movements over all simulations run.
	Number of Simulations - this sets the number of simulations to run for the
							Run Simulation MULTIPLE option


# Misc.
--------------------------------------------------------------------------------
The Green bar traveling across the screen represents the finished "scan" 
location, whereas the blue bar represents the current "scan" location. This
shows where the algorithm is looking as well as the place that it believes is
under complete coverage.


# Editing the source code
--------------------------------------------------------------------------------
For editing the source:
  - Install openFrameworks: http://openframeworks.cc/download/
  - (optional) Install the openFrameworks VS plugin.
  - Create an openFrameworks project.
  - Add and overwrite the source from this repository to the src/ folder 
    of your project.
  - Compile & run.
