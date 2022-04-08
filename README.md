# Bankers-Algorithm
A resource-allocation and deadlock avoidance algorithm that tests for saftey by simulating the allocation of all of the predetermined maximum possible amounts of resources. It then makes an "s-state"check to test for all possible activities before deciding if the resource allocation is safe or not.

# How it Works
Within the algorithm, there are 4 different types of arrays specified:
- Available array: This is a one dimensional array that used to contain the number of available resources of each type
- Max array: A two dimensional array that is used to define the maximum demand of each process within a system
- Allocation array: A two dimensional array that defines the number of resources of each type that are currently allocated to each process
- Need Array: A two dimensional array that indicates the remaining resource needs of each proecess type

Upon the definition of the arrays, the overall banker algorithm can be subdivided into two major sub algorithms
- `resource_request`: This method requests a random number of resources
- `resource_release`: This method releases a random number of resources

# How to run
In order to run the bankers algorithm simply type in the command: gcc -Wall -Wextra -std=c99 -pthread banker.c -o banker

This will create a banker object file, which we can then simply call and pass in three parameters (sperated via white space) which are used to specify the number of instances of each resource type: ./banker 10 5 7


