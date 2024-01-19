## CS380C Labs

CS380C: Advanced Compiler Techniques (utexas)

[course link](https://www.cs.utexas.edu/users/mckinley/380C/labs/labs.html)

### Lab1: Three-Address-Code to C translator

### Lab2: Dataflow Analysis and Optimizations 

1. Construct Control Flow Graph;
2. Reaching Definitions -> Constant Propagation; 
3. Live Variables -> Dead Statement Elimination;

### Lab3: Single Static Assignment and Optimizations

1. Build SSA Form;
2. Translate back to non-SSA Form;

### Usage Example

```shell
mkdir build && cd build
cmake ..
make
./csc-opt --backend=ssa,3addr --opt=ssa -o ../test/ssa/gcd-ssa.tac ../test/tac/gcd.tac
```
