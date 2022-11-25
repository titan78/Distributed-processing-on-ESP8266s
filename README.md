# Distributed-processing-on-ESP8266s
##### This is an academic project related to advanced computer architecture and interconnection networks (NOC) courses. the fall of 2022 at the University of Tehran.

### Distributed processing on a network of low-power processors
###### Due to the fact that the processors in embedded systems have low processing power, it is not possible to perform heavy processing on these types of processors in a reasonable time.

### But in the other hand...

###### In general, we have a large number of these low-power processors in embedded systems, and we know that in many places, strength is in numbers! 
###### Therefore, we are creating a platform to connect these low-power processors to the WIFI platform so that they work as a unified processor so that we can take advantage of their total power when necessary.


## What are we going to do?

### 1- We build a platform that simulates a series of operating system tasks for us!
##### Things like...
#### ........................ 1- Multi-threaded processing, each thread a CPU!
#### ........................ 2- Mutual exclusion management
#### ........................ 3- Syscalls to access specific hardware capabilities
#### ........................ 4- An interpreter for running programs written in ***Patrick*** scripting language.

#### Why do we need an interpreter?

###### We implement the platform with CPP and since microcontrollers do not have an operating system and this platform that we write will be the only software that will run on these hardwares, hence a light, optimized and suitable scripting language for Multi-threaded programming is needed and we decided to develop our own language for this.

### 2- We're building a new programming language... In honor of the tired starfish, Patrick!
##### which features such as:
#### ........................ 1- Branch_to for execute code blocks on other cores
#### ........................ 2- Cloud Linker To transfer and execute functions between nodes optimally
#### ........................ 3- Numerical variables and mathematical expressions for computational tasks
#### ........................ 4- and System variables and functions to access platform capabilities
##### will have.

### 3- A syntax highlighter plugin for Patrick language, installable on vscode.

