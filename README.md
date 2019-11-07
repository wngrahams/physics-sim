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
    * Download requirements:

        `sudo apt-get install xorg-dev libosmesa6-dev`

    * Clone source:

        `git clone https://github.com/glfw/glfw.git glfw`

    * Build and install:

        ```
        mkdir glfw-build
        cd glfw-build
        cmake ../glfw
        make
        sudo make install        
        ```

3. Compile and link with the following flags:

    `-L/usr/local/lib -L/usr/lib64 -L/usr/lib/x86_64-linux-gnu -lGL -lGLU \
    -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lGLEW`


