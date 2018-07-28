# CMeasure

CMeasure is C++ tool designed to interact and collect data produced by the
National Instruments DAQ Device [USB-6009](http://www.ni.com/pt-br/support/model.usb-6009.html).

This software can be used to collect and proccess thousands samples of 
instantaneous power measurements per second. By integrating over these
samples, we can calculate the total energy (in Joules) spent by a target
device. 

This branch contains code that may be compiled and executed in MS Windows and
linux. We migrated from National Instruments DAQmx to DAQmx-base Driver, which
contains the kernel modules to manipulate the usb 6009 device.


This repository also contains a simple R script that may be used to produce power charts
out of the output produced by CMeasure.

### Power chart examples:
![Sample 1](/docs/sample1.png)

![Sample 2](/docs/sample2.png)

![Sample 3](/docs/sample3.png)

![Sample 4](/docs/sample4.png)


### Requirements


#### Microsoft Windows
 
 * Tested on Windows 10 x64- previous version of the code (using DAQmx driver worked on Windows 7 and 8 as well, so the current code might work as well) -

  1. NI-DAQmx Base 15.0 
     Windows: http://www.ni.com/download/ni-daqmx-base-15.0/5649/en/ 

#### Linux
 
 *  Tested on [openSUSE 13.1 (32bits)](https://ftp5.gwdg.de/pub/opensuse/discontinued/distribution/13.1/iso/openSUSE-13.1-DVD-i586.iso) - linux kernel 3.12.67-64.1-noarch - 

  1. Linux kernel headers for kernel being used. 
       May be downlodade with (openSUSE) : sudo zypper install kernel-source-3.12.67-64.1.noarch   

  2. [NI-KAL 15.1](http://download.ni.com/support/softlib/kal/15.1/NIKAL151.iso)   

  3. [NI-VISA 15.5](http://download.ni.com/support/softlib/visa/NI-VISA/15.5/Linux/NI-VISA-15.5.0.iso)   

  4. NI-DAQmx Base 15.0  
     Linux  : http://www.ni.com/download/ni-daqmx-base-15.0/5644/en/ 
      

#### macOS
 
 * NOT TESTED

  1. NI-DAQmx Base 15.0  
     macOS  : http://www.ni.com/download/ni-daqmx-base-15.0/5648/en/ 




### Installation



#### Microsoft Windows
Remove any previous version of National Instrument software. Then, you just need to download the NI-DAQmx Base driver,
install it using the wizard and reboot the OS. After that, the device should recognized by the system. You can check
it by starting the NI software "list devices", which should list the "USB-6009" DAQ device. 

#### Linux (openSUSE 13.1 32bits)
It is quite complicated to make NI drivers work properly under linux. They support some very few rpm based distros, like
Red Hat, CentOS (tried but I couldn't make the driver work) and openSUSE 32 Bits. Also there is an issue with the 64bit 
archtectures, which I won't discuss here.

Guide:

1. Download Linux Kernel sources and build tools:
```bash
   sudo zypper install kernel-default-3.12.67-64.1.i586 kernel-source-3.12.67-64.1.noarch gcc gcc-c++
```

2. Reboot

3. Update Kernel Modules
   There is a bug in the NI-KAL updateNIdriver tool that doesn't allow some modules to compile
   and start properly:
   https://forums.ni.com/t5/Multifunction-DAQ/libnipalu-so-failed-to-initialize/m-p/1255496/highlight/true#M59785

   Fix:
```bash   
   sudo su
   cd /usr/src/linux
   version=$( uname -r )
   zcat /boot/symvers-$version.gz > Module.symvers
   make modules_prepare   
```

4. Download and install NI-KAL 15.1
   You just need to download the iso, extract it and run the script: ./INSTALL as root
   then reboot


5. Download and install NI-VISA 15.5
   You just need to download the iso, extract it and run the script: ./INSTALL as root
   then reboot


6. Download and install NI-DAQmx Base 15.0
   You just need to download the iso, extract it and run the script: ./INSTALL as root
   then reboot

7. Run lsdaq and see if it lists the USB 6009 usb device



