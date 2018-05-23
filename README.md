# 2048 AI BOT

An AI bot for the game 2048.

Made for the competition of **NTU Taiwan 2048 BOT Competition** in my second half semester of first grade.

![poster](poster.jpg)

## Build

(current support windows and linux)

### AI version

`$ make`

`$ ./bot2048`


### Manual play version (Use W,A,S,D to control)

`$ make manual`

`$ ./manual2048`

## Build with docker

* Compile in Ubuntu docker manually
	1. [Mount physical directory](https://docs.docker.com/storage/bind-mounts/)
	
		`$ docker run -it --name test2048 -v "$(pwd)":/app ubuntu:latest`
	
	2. Install compilation tool
	
		`$ apt-get install g++`
	
	3. Compile (using instructions above)
	4. (Optional) Save the container using export
	
		`$ docker export test2048 > test2048.tar`

    > Pros: The package is small and clean (only about 200 MB for g++ compiler)
    > 
    > Cons: To many manual steps


* Build a image based on gcc and compile it automatically in it and save the image
	1. Dockerfile: 
	
        ```
        FROM gcc:4.9
        COPY . /2048-AI
        WORKDIR /2048-AI
        RUN g++ -o 2048-AI main.cpp 2048.cpp
        CMD ["./2048-AI"]
        ```
	
	2. Build image
	
		`$ docker build -t ai2048:v1`

	3. Run image
	
		`$ docker run -it --rm --name test ai2048:v1`

    > Pros: Very quick
    > 
    > Cons: The new package is as big as gcc image (1.37GB)

* Cross compile using gcc image and run it in a clean ubuntu image
	1. Compile using gcc image (it will save in the current directory)
	
		`$ docker run --rm -v "$PWD":/usr/src/ai2048 -w /usr/src/ai2048 gcc:4.9 make`
	
	2. Run in ubuntu container
	
		`$ docker run -it --rm --name 2048AI -v "$PWD:/app" -w /app ubuntu:latest ./bot2048`

    > Pros: Very small
    > 
    > Cons: Haven't thought one


## Author

* David Lee
* Tom Chang

## Notes

### Doxygen

* [Where should doxygen comment blocks place](https://stackoverflow.com/questions/355619/where-to-put-the-doxygen-comment-blocks-for-an-internal-library-in-h-or-in-cpp)

### Docker

* [Using C++ with docker engine](https://www.codeguru.com/cpp/cpp/algorithms/using-c-with-docker-engine.html)
* [Difference between docker save and export](https://blog.hinablue.me/docker-bi-jiao-save-export-dui-yu-ying-xiang-dang-cao-zuo-chai-yi/)
* [Export and import docker](https://philipzheng.gitbooks.io/docker_practice/content/container/import_export.html)

### VS Code

* [C/C++ for VS Code](https://code.visualstudio.com/docs/languages/cpp)
