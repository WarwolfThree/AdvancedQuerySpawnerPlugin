# AdvancedQuerySpawner
 Advanced query based spawn system for UE5

<!-- ABOUT THE PROJECT -->
## About The Project

AQS is a Battlefield-style spawn system that will do its best to make sure that any players spawn out of sight from any enemies, without the need to design and program a whole system yourself.

Quick breakdown:
* AQS is designed to be a time saver for your designers and programmers by doing a lot of the work for you on the backend and exposing easy to access parameters.
* It's all just a plugin, and easy to extend/modify completely seperately - no need to work old and/or messy code bases from previous projects!
* Different options for spawning in already built in

<p align="right">(<a href="#top">back to top</a>)</p>



### Built With

Built for Unreal Engine 5.

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

``` Unreal Engine 5 Early Access ```
``` Visual Studio 2019 ```
``` A UE5 project with at least one C++ class ```


### Installation

_Below is an example of how you can instruct your audience on installing and setting up your app. This template doesn't rely on any external dependencies or services._

1. Create a new project in Unreal
2. Open the project and go to ``` Edit -> Project Settings -> Collision ``` and create a new trace channel called `Spawner`
3. Close the project
4. Navigate to your project folder (where your .uproject files is)
5. Create a new folder called `Plugins` if it is not already there
6. Put the AQS plugin folder inside that new Plugins folder
7. Inside your `[ProjectName].build.cs` file, add the public module `AQS`
8. Make sure to build your project from visual studio
9. Open the project again and navigate to ``` Edit -> Plugins ```. You should see a category called `System`, under that you should find AQS. Make sure to enable it
10. Restart editor and you will be good to access the plugins features

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

### Quick Note

_The included GameMode and PlayerState base classes aren't absolutely essential. You're more than welcome to copy and paste these functions into your own GM/PS. I've just included them as the system requires a little more logic than just the component can offer.

### Using the Plugin

_Refer to the example content provided

<p align="right">(<a href="#top">back to top</a>)</p>