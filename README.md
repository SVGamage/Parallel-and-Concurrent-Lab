# Parallel-and-Concurrent-Lab

### Build the Program

To compile the program, simply run:

```bash
make
```


This will generate an executable named `main`.

### Running the Program

The program accepts four arguments:

- The mode of execution (`mutex`, `rwlock`, or `serial`).
- The fraction of `Member` operations.
- The fraction of `Insert` operations.
- The fraction of `Delete` operations.

./main [mutex|rwlock|serial] [mMember] [mInsert] [mDelete]

```bash
./main mutex 0.99 0.005 0.005
./main rwlock 0.90 0.05 0.05
./main serial 0.50 0.25 0.25
```
### Run all test cases

```bash
make run_all_cases
```