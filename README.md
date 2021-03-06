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
	
		`$ apt-get update`
		
		`$ apt-get install -y g++ build-essential`
	
	3. Compile (using instructions above)
		
		`$ cd /app`
		
		`$ make`
	
	4. Rerun the container (after you stop the container)
	
		`$ docker start -i test2048`
	
	5. (Optional) Save the container using export
	
		`$ docker export test2048 > test2048.tar`

    > Pros: The package is small and clean (only about 158 MB for g++ compiler and 62.7MB for build-essential)
    > 
    > Cons: Too many manual steps (we need to install everything and run the container and then run the program by ourself)
    >
    > To Test:
    > 	1. Commit the changes of container to build an new image
    >	2. Make a Dockerfile (i.e. Build an image only include our essential c compile tools) based on our manual installation
    >   (Act like my second method but take less space)
    


* Build a image based on gcc and compile it automatically in it and save the image
	1. Dockerfile: 
	
		* AI version
		
			```
			FROM gcc:4.9
			COPY . /2048-AI
			WORKDIR /2048-AI
			RUN make
			CMD ["./bot2048"]
			```
	
		* Manual play version
		
			```
			FROM gcc:4.9
			COPY . /2048-AI
			WORKDIR /2048-AI
			RUN make manual
			CMD ["./manual2048"]
			```
	
	2. Build image
	
		`$ docker build -t ai2048:v1 .`

	3. Run image
	
		`$ docker run -it --rm --name test ai2048:v1`

    > Pros: Very quick (after build an image, each time we want to execute program just need to run the container)
    > 
    > Cons: The new image we built is as big as gcc image (1.37GB) (so it take double space in computer because of two images exist)

* Cross compile using gcc image and run it in a clean ubuntu image
	1. Compile using gcc image (it will save in the current directory)
	
		`$ docker run --rm -v "$PWD":/usr/src/ai2048 -w /usr/src/ai2048 gcc:4.9 make`
	
	2. Run in ubuntu container
	
		`$ docker run -it --rm --name 2048AI -v "$PWD:/app" -w /app ubuntu:latest ./bot2048`

    > Pros: Very small and easy to maintain
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
