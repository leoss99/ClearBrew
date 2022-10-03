# CPEN 391 L2B-13

## ClearBrew Smart Coffee Scale

### Project Description

The ClearBrew is a smart coffee scale, for use in at-home coffee brewing.
It measures weight like any other scale, but provides specific functions for coffee brewing.
When brewing a cup of coffee, the user performs multiple steps of weighing and timing;
these collections of steps are called recipes.
On the ClearBrew, the user can select a recipe using the touchscreen display, then follow
the steps as they brew. During a timing step, a timer is displayed on the LCD touch screen;
during a weighing step, the weight in grams is displayed.
The user can also connect to their ClearBrew through their smartphone by going to the ClearBrew
website. On the website, the user can see their existing recipes in a list format on the main page.
They can follow a wizard which guides them through making a new recipe, as well as edit or delete
existing recipes; these recipes are stored in the cloud, connected to the user’s ClearBrew.
On a profile page, the user can set their preferred coffee type, roast, and brew methods;
these preferences are used to recommend new recipes to the user. Each time the ClearBrew starts up,
it fetches the most recent recipes from the cloud.

### Main Components

- DE1-SoC FPGA Board

- DE1-SoC Computer, running Linux on the ARM HPS

- 7.0" 40-pin TFT Display - 800x480 with Touchscreen

- AR1100 Resistive USB and RS-232 Touch Screen Controller

- TAL220B load cell and Hx711 load cell amplifier

- AWS Cloud Architecture, including Cognito, Amplify, API Gateway, Lambda, and DocumentDB

### Repository Structure

Under the [project](./project) directory, the repository is divided into [hardware](./project/hardware)
and [software](./project/software). 

Within the hardware directory, [DE1-SoC_Computer](./project/hardware/DE1-SoC_Computer) contains
the DE1-SoC Computer, [arm-hps](./project/hardware/arm-hps) contains the files running on Linux
on the ARM HPS, and [load-cell](./project/hardware/load-cell) contains some documentation about
the load cell.

Within the software directory, more details about the AWS Cloud architecture can be found.


### Authors

[Tom Nguyen]()

[Leo Sun]()

[Sean Garvey]()

[Andrew Forde](https://github.com/awforde)



### Acknowledgements

Much of the work done on this project built on top of previous projects.
As such, we want to give credit to the following projects:

[Translaite](https://github.com/ShreyansK2000/Translaite)

[cJSON](https://github.com/DaveGamble/cJSON)

[libcurl](https://curl.se/libcurl/)

And the many people scattered about forums whose little contributions made a big difference.