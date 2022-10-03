# ARM HPS

This directory contains the files used on the Linux ARM HPS.


### Table of contents

To run ClearBrew on the Linux HPS, there is a list of preliminaries:


1. Transferring files to the HPS

2. Programming the DE1-SoC Computer

3. Installing libcurl

4. Installing drivers

5. Making and running the project

### Transferring files to the DE1-SoC

In order to run our code, we need to transfer data from our PCs to the DE1-SoC.
There are a few ways to do this. The fastest way for individual files is to simply
open vim on the DE1 Linux CLI, paste the code copied from an editor on your PC, and save
the file. This works well if you only need to change one or two files, but this is slow
when first starting the project; it also does not work when transferring files other than
code or text.

Another solution, which is preferred for large transfers, is to copy the needed files or directories
onto the micro-SD card, which has already been flashed with the Linux image. More details
on this process can be found on [page 59 of this document](https://ftp.intel.com/Public/Pub/fpgaup/pub/Teaching_Materials/current/Tutorials/Linux_On_DE_Series_Boards.pdf).
After re-inserting the micro-SD card into the DE1 and powering on, the transferred files can
be found in `/media/fat-partition/`. From here, they can be moved or copied to another location in the
file system.

### Programming the DE1-SoC Computer

After you have compiled the modified DE1-SoC computer, you need to transfer it to the DE1
and program it. Transfer the rbf file to the `fat-partition` as described in the section above,
then copy it somewhere on the file system. The default rbf file is located at `/home/root`;
you may choose to put it here, but to avoid deleting the default while testing, I suggest moving it
somewhere like `/home/root/rbf`. Program the fpga by following [page 60 of this document](https://ftp.intel.com/Public/Pub/fpgaup/pub/Teaching_Materials/current/Tutorials/Linux_On_DE_Series_Boards.pdf).
If desired, modify the file at `/etc/init.d/programfpga` and set the path to the modified rbf file.
This will cause the modified rbf file to be programmed immediately on startup.

### Installing libcurl

This project requires libcurl in order to communicate with the backend APIs.
Download [curl-7.82.0.tar.gz](https://curl.se/download.html) and transfer it to the DE1,
again via the micro-SD card. Move the compressed file to `/home/root` and decompress it.
I followed [these instructions](https://goacademy.io/how-to-install-curl-from-source-on-linux/)
to install libcurl. 

### Installing drivers

This project requires the video drivers, which come already installed on the Linux image we used.
To install these drivers, go to `~/Linux_Libraries/drivers` and type `./load_drivers`.
This must be done each time the DE1 is rebooted.

### Making and running the project

This directory (`./clearbrew/`) is set up so that it can be transferred to the DE1 via the micro-SD card.
Simply copy the whole directory, move it to `~/clearbrew/`, and make.
Running `make clean` cleans the project, and running `make` builds the project.
To run the project, type `./clearbrew`.