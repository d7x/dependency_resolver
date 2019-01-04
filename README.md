# dependency_resolver
Dependency resolver is an application written in C which parses package dependencies from a .pms file which describes the dependencies in json format. I wrote it in 2016 as a task assignment for a job interview. In its current state it may or may not be of practical use but it can be easily modified. Feel free to use it or redistribute it as long as you keep the credits in the file and value my work as the original author. 

It uses the cJSON library (included within the present repistory) by Dave Gamble which parses JSON formatted strings and the most recent version can be found at https://github.com/DaveGamble/cJSON

It includes a makefile so to compile just run: 

$ make

Usage:
dependency_resolver <filename> <jsonfile dependency>

Example:

$ ./dependency_resolver packages.pms A

["B", "F", "D", "G", "E"]
  
where <filename> is the json file containning the dependencies and the second parameter is which group to show the dependencies for
  
  The jsonfile is json data in the following format:
  
[

{"name" : "A", "requires" : ["B", "D", "E"]},

{"name" : "B", "requires" : ["F"]},

{"name" : "C", "requires" : ["G", "H"]},

{"name" : "D", "requires" : ["G"]},

{"name" : "E", "requires" : []},

{"name" : "F", "requires" : []},

{"name" : "G", "requires" : []},

{"name" : "H", "requires" : []}

]
