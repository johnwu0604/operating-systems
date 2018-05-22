# Operating Systems Assignments

The following repository holds all the source code for assignments demonstrating operating system implementations. All code is written in C. Below is a brief description for each assignment.

## Assignment 1 - Redirection, Interprocess Communication, Shell Interface

redirect.c : Redirection example that consists of changing the stdout to print to a text file and then switching it back to the regular console

command_piping.c : Interprocess communication example that consits of forking a process and using a pipe to print the output of the child process in the parent process.

shell_interface.c : Shell interface example that consists of implementing an interface that emulates standard shell commands.

## Assignment 2 - Producer/Consumer Simulation

air_taxi_sim.c : Produce/Consumer simulation example that consists of an airport taxi line modelled as the buffer where a group of taxis act as the consumer and a fleet of aircrafts act as the producer.

## Assignment 3 - Theoretical Questions, Disk Scheduling, Bankers Algorithm

theory.pdf - Basic theoretic questions regarding operating systems.

disk_scheduler.c - Disk scheduler that demonstrates different scheudling algorithms including C-LOOK, C-SCAN, FCFS, SSTF, SCAN, LOOK.

resource_request_simulator.c - Banker's algorithm implementation.

faulty_resource_request_simulator.c - Banker's algorithm implementation with deadlock detection caused by faulty resources.