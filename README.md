# CS3113-Assignment-3

Project Overview

This project demonstrates the use of process synchronization techniques in C by implementing a producer-consumer model with semaphores. The program uses a shared circular buffer to facilitate communication between two threads: a producer and a consumer. The producer reads characters from a file and places them into the buffer, while the consumer retrieves and prints the characters. Synchronization mechanisms are implemented using semaphores to ensure proper communication between the threads and to prevent race conditions.

The buffer has a fixed size of 15, allowing continuous data flow by wrapping around when full. To signal the end of data, the producer uses a special EOF marker (*). The consumer processes each character with a delay to simulate slower operation, demonstrating the synchronization capabilities of the system.

System Requirements

The program is designed to run on a Linux operating system and requires GCC as the compiler. The development environment used for this project is Visual Studio Code, and GitHub is recommended for version control.

Directory Structure

cs3113-Assignment2/
├── CS3113-Project3.c       # Main implementation file
├── README.md               # Project documentation
├── mytest.dat              # Input file with up to 150 characters
└── CS3113 Project 3 Report.pdf  # Detailed project report

Project Details

This project uses a circular buffer of size 15 to allow the producer and consumer threads to exchange data. The producer reads characters from the input file and writes them to the buffer, while the consumer retrieves these characters and prints them to the screen. To ensure proper synchronization, three semaphores are used:

The first semaphore, sem_items, tracks the number of items currently in the buffer. It ensures that the consumer waits when the buffer is empty.
The second semaphore, sem_space, tracks the available space in the buffer. It ensures that the producer waits when the buffer is full.
The third semaphore, sem_mutex, protects the critical section, ensuring that only one thread accesses the buffer at a time.
The producer continues reading and writing characters until it reaches the end of the input file. At this point, it places the EOF marker (*) in the buffer to signal to the consumer that no more data will be added. The consumer stops processing data upon encountering this marker.
