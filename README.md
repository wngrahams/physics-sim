# physics-sim
Simple physics simulator

steps to set up and run (linux ubuntu):

1. Install GLEW:
    * Download requirements:
        `sudo apt-get install libXmu-dev libXi-dev libgl-dev dos2unix git wget`
    * Clone source:
        `git clone https://github.com/nigels-com/glew.git glew`
    * Build and install:
        ```
        cd glew
        make all
        sudo make install.all
        ```
    * May need to copy files to other library directory:
        `sudo cp /usr/lib64/libGLEW.* /usr/lib/x86_64-linux-gnu/` 

2. Install GLFW:

