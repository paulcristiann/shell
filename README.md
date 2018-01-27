# shell
Linux Shell written in C featuring pipeline, output/input redirection, history and logical operators.

To compile, GNU Readline software library is needed. 

To compile: "make"
To clean and compile: "clean && make"

History is accesible by pressing the up key
Pipeline is used: command1 | comand2
File redirection: command1 > file.out
Logical operators: command1 && command2;  command1 || command2; these operators have clasic shell functionality
