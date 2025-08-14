#Main Code is in Temperature_Sensor.c ==> the code is developed in C Language
#Library File out is in tempsensor_ncurses.c ==> this code is developed in C language with ncurses library
#Makefile is used to build the files 

# Temperature Sensor Linux Kernel Module

A Linux kernel module that simulates a temperature sensor device with a colorful ncurses-based user interface. This project demonstrates kernel module development, character device drivers, and user-space interaction with kernel modules.

## 🌡️ Overview

This project consists of three main components:
1. **Kernel Module** (`Temperature_Sensor.c`) - A character device driver that generates random temperature readings
2. **User Interface** (`tempsensor_ncurses.c`) - An ncurses-based application that displays temperature data with color-coded weather conditions
3. **Makefile** - Build system for compiling both kernel module and user application

## ✨ Features

- **Real-time Temperature Simulation**: Generates random temperatures between -10°C and 50°C
- **Weather Condition Classification**: Categorizes temperatures into 7 different weather conditions
- **Colorful Display**: Color-coded temperature display based on weather conditions
- **Kernel-User Space Communication**: Demonstrates proper character device implementation
- **Professional UI**: Clean, centered ncurses interface with real-time updates

### Weather Conditions & Color Coding

| Temperature Range | Condition | Color |
|-------------------|-----------|-------|
| ≥ 35°C | Extreme Hot | Red |
| 25°C - 34°C | Sunny | Yellow |
| 18°C - 24°C | Warm | White |
| 10°C - 17°C | Cloudy | Cyan |
| 5°C - 9°C | Cool | Green |
| 0°C - 4°C | Cold | Blue |
| < 0°C | Freezing | Magenta |

## 📁 Project Structure

```
Temperature_Sensor_Module/
├── Temperature_Sensor.c      # Kernel module source code
├── tempsensor_ncurses.c      # User interface application
├── Makefile                  # Build configuration
└── README.md                 # This file
```

## 🛠️ Prerequisites

Before building and running this project, ensure you have:

### System Requirements
- Linux operating system (Ubuntu/Debian/CentOS/RHEL)
- Kernel headers for your current kernel version
- GCC compiler
- Make utility
- ncurses development library
- Root/sudo privileges (for module loading)

### Install Dependencies

#### Ubuntu/Debian:
```bash
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r) libncurses5-dev
```

#### CentOS/RHEL/Fedora:
```bash
sudo yum install gcc make kernel-devel ncurses-devel
# or for newer versions:
sudo dnf install gcc make kernel-devel ncurses-devel
```

## 🚀 Build Instructions

1. **Clone the repository** (if applicable):
```bash
git clone <repository-url>
cd Temperature_Sensor_Module
```

2. **Build the project**:
```bash
make all
```

This command will:
- Compile the kernel module (`Temperature_Sensor.ko`)
- Compile the user interface application (`tempsensor_ui`)

### Individual Build Commands

Build only the kernel module:
```bash
make kernel
```

Build only the user application:
```bash
make user
```

## 📖 Installation and Usage Guide

### Step 1: Load the Kernel Module

```bash
sudo insmod Temperature_Sensor.ko
```

**Verify module loading:**
```bash
lsmod | grep Temperature_Sensor
dmesg | tail -5
```

You should see output similar to:
```
Temperature Sensor: Module loaded with major number 240
```

### Step 2: Create Device Node (if not auto-created)

Check if device exists:
```bash
ls -l /dev/Temperature_Sensor
```

If the device doesn't exist, create it manually:
```bash
# Find the major number from dmesg output
MAJOR_NUM=$(dmesg | grep "Temperature Sensor: Module loaded" | tail -1 | grep -o 'number [0-9]*' | grep -o '[0-9]*')
sudo mknod /dev/Temperature_Sensor c $MAJOR_NUM 0
sudo chmod 666 /dev/Temperature_Sensor
```

### Step 3: Run the Application

```bash
./tempsensor_ui
```

### Step 4: Interact with the Application

- The application will display real-time temperature readings
- Each reading updates every 2 seconds
- Colors change based on temperature conditions
- Press `Ctrl+C` to exit the application

## 🧹 Cleanup and Removal

### Step 1: Stop the Application
Press `Ctrl+C` to terminate the user interface.

### Step 2: Remove the Kernel Module
```bash
sudo rmmod Temperature_Sensor
```

### Step 3: Clean Build Files
```bash
make clean
```

### Step 4: Remove Device Node (if manually created)
```bash
sudo rm /dev/Temperature_Sensor
```

## 🔧 Technical Details

### Kernel Module Architecture

The kernel module implements a character device driver with the following operations:
- **Open**: Prevents multiple simultaneous opens
- **Read**: Generates random temperature and returns formatted string
- **Write**: Disabled (returns -EINVAL)
- **Release**: Decrements device open counter

### Key Functions

| Function | Description |
|----------|-------------|
| `generate_random_temperature()` | Generates random temperature in specified range |
| `dev_open()` | Handles device open operations |
| `dev_read()` | Provides temperature data to user space |
| `dev_release()` | Handles device close operations |

### User Application Features

- **ncurses Interface**: Professional terminal-based UI
- **Color Support**: Dynamic color coding based on temperature
- **Real-time Updates**: Continuous temperature monitoring
- **Error Handling**: Proper device communication error handling

## 🐛 Troubleshooting

### Common Issues and Solutions

#### 1. Module fails to load
```bash
# Check kernel logs
dmesg | tail -10

# Common solutions:
sudo modprobe configs  # Enable module loading
sudo sysctl kernel.modules_disabled=0
```

#### 2. Device not found
```bash
# Manually create device node
sudo mknod /dev/Temperature_Sensor c <major_number> 0
sudo chmod 666 /dev/Temperature_Sensor
```

#### 3. Permission denied
```bash
# Fix device permissions
sudo chmod 666 /dev/Temperature_Sensor
# or run with sudo
sudo ./tempsensor_ui
```

#### 4. ncurses display issues
```bash
# Reset terminal
reset
# or
tput reset
```

#### 5. Module already exists
```bash
# Remove existing module first
sudo rmmod Temperature_Sensor
```

## 📊 Example Output

```
===============================
     Temperature Monitor        
===============================

Temperature: 23°C
Condition  : Warm

===============================

Press Ctrl+C to exit...
```

## 🔍 Debugging

### Enable Debug Output
```bash
# View kernel messages in real-time
tail -f /var/log/kern.log

# or
dmesg -w
```

### Check Module Status
```bash
# List loaded modules
lsmod | grep Temperature

# Module information
modinfo Temperature_Sensor.ko

# Device information
cat /proc/devices | grep Temperature
```

## 📝 Code Explanation

### Kernel Module Components

1. **Headers**: Include necessary Linux kernel headers
2. **Device Registration**: Register character device with the kernel
3. **File Operations**: Implement device file operations (open, read, write, release)
4. **Class/Device Creation**: Create device class and device node
5. **Module Init/Exit**: Handle module loading and unloading

### User Application Components

1. **Device Communication**: Open and read from kernel device
2. **ncurses Setup**: Initialize color terminal interface
3. **Data Processing**: Parse temperature data from kernel
4. **Display Logic**: Show formatted, colored output
5. **Main Loop**: Continuous temperature monitoring

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## 📄 License

This project is licensed under the GPL License - see the module source code for details.

## 👨‍💻 Author

**Tharun Kumar M**
- GitHub: [@TharunKumarMothukuri](https://github.com/TharunKumarMothukuri)
- Gmail: venkattharunkumar@gmail.com
- Phone Number: 8142993793

## 📞 Support

If you encounter any issues or have questions:
1. Check the troubleshooting section
2. Review kernel logs with `dmesg`
3. Open an issue on GitHub
4. Contact the author

## 🎯 Future Enhancements

- [ ] Add temperature history logging
- [ ] Implement configurable temperature ranges
- [ ] Add support for multiple sensor instances
- [ ] Create web interface
- [ ] Add temperature alerts/notifications
- [ ] Implement data export functionality

---
