# MovieDatabase

A C++ application that interfaces with MongoDB to manage and search a movie database. The application allows users to search for movies by title, genre, and actor names.

## Prerequisites

### Windows
- C++20 compatible compiler
- vcpkg package manager (recommended installation path: `%USERPROFILE%\Dev\vcpkg`)
- CMake 3.15 or later
- MongoDB Community Server

### Linux
- GCC 10 or later with C++20 support
- CMake 3.15 or later
- MongoDB Community Server
- libmongocxx driver
- libbsoncxx driver

## Installation Instructions

### Windows Setup

1. **Install vcpkg** (if not already installed)
   ```powershell
   # Create development directory in your user profile
   mkdir "%USERPROFILE%\Dev"
   cd "%USERPROFILE%\Dev"
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   bootstrap-vcpkg.bat
   vcpkg integrate install
   ```

2. **Set up Environment Variables**
   1. Add vcpkg root to System PATH:
      - Open System Properties > Advanced > Environment Variables
      - Under System Variables, find and select "Path"
      - Click "Edit" and add your vcpkg installation path (e.g., `%USERPROFILE%\Dev\vcpkg`)
      - Click "OK" to save

   2. Create VCPKG_DEFAULT_TOOLCHAIN variable:
      - Under System Variables, click "New"
      - Variable name: `VCPKG_DEFAULT_TOOLCHAIN`
      - Variable value: Path to your vcpkg toolchain file (e.g., `%USERPROFILE%\Dev\vcpkg\scripts\buildsystems\vcpkg.cmake`)
      - Click "OK" to save

   3. **Important**: Close and reopen any command prompts or IDEs for the changes to take effect

3. **Install MongoDB Driver using vcpkg**
   ```powershell
   vcpkg install mongo-cxx-driver:x64-windows
   ```

4. **Install MongoDB Community Server**
   - Download MongoDB Community Server from the official website
   - Follow the installation wizard
   - Make sure the MongoDB service is running

5. **Build the Project**
   ```powershell
   mkdir build
   cd build
   # Since we set VCPKG_DEFAULT_TOOLCHAIN, we can use simpler cmake command:
   cmake -B . -S ..
   cmake --build . --config Release
   ```

   Alternative build command (if environment variable isn't working):
   ```powershell
   cmake -B . -S .. -DCMAKE_TOOLCHAIN_FILE=[path_to_vcpkg]/scripts/buildsystems/vcpkg.cmake
   ```

### Linux Setup

1. **Install Required Packages**
   ```bash
   # Ubuntu/Debian
   sudo apt update
   sudo apt install cmake gcc g++ pkg-config libmongocxx-dev libbsoncxx-dev

   # Fedora
   sudo dnf install cmake gcc-c++ pkg-config mongo-cxx-driver mongo-cxx-driver-devel
   ```

2. **Install MongoDB**
   ```bash
   # Ubuntu/Debian
   sudo apt install mongodb

   # Fedora
   sudo dnf install mongodb mongodb-server
   ```

3. **Start MongoDB Service**
   ```bash
   sudo systemctl start mongodb
   ```

4. **Build the Project**
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

## Running the Application

1. **Ensure MongoDB is Running**
   - Windows: Check Services app for "MongoDB Server"
   - Linux: `sudo systemctl status mongodb`

2. **Run the Application**
   - Windows: `.\build\Release\MovieDatabase.exe`
   - Linux: `./build/MovieDatabase`

## Usage

The application provides a simple menu interface with the following options:
1. Search by title
2. Search by genre
3. Search by actor
4. Exit

Enter the corresponding number and follow the prompts to search the database.

## Troubleshooting

1. **Connection Issues**
   - Verify MongoDB is running
   - Check if MongoDB is listening on default port (27017)
   - Ensure no firewall rules are blocking the connection

2. **Build Issues**
   - Verify environment variables are set correctly (check Path and VCPKG_DEFAULT_TOOLCHAIN)
   - Try opening a new command prompt after setting environment variables
   - Check if all dependencies are properly installed
   - Ensure compiler supports C++20

## Note

The application expects MongoDB to be running on `localhost:27017`. If your MongoDB instance is running on a different host or port, modify the connection string in `main.cpp`:

```cpp
mongocxx::uri uri("mongodb://localhost:27017");
```