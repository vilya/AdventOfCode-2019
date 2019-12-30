Intcode VM Benchmarks
=====================

This is a suite of intcode VM benchmarks from here:

https://www.reddit.com/r/adventofcode/comments/egq9xn/2019_day_9_intcode_benchmarking_suite/


Benchmark Results
-----------------

| Program       | Input         | Output                       | Runtime       |
| ------------- | ------------- | ---------------------------- | ------------- |
| sum-of-primes | 100000        | 454396537                    |     14.9 ms   |
| sum-of-primes | 2000000       | 142913828922                 |    351.4 ms   |
| sum-of-primes | 100000000     | 279209790387276              |  21326.7 ms   |
| ackermann     | 3,6           | 509                          |      8.4 ms   |
| ackermann     | 4,1           | 65533                        | 131578.4 ms   |
| ackermann     | 3,14          | 131069                       | 526883.3 ms   |
| isqrt         | 130           | 11                           |      0.012 ms |
| divmod        | 1024,3        | 341,1                        |      0.013 ms |
| factor        | 19338240      | 2,2,2,2,2,2,2,2,2,2,3,5,1259 |      1.9 ms   |
| factor        | 2147483647    | 2147483647                   |     37.5 ms   |
| factor        | 1689259081189 | 1299709,1299721              |    822.9 ms   |

Taken on a 2015 MacBook Pro with a 2.5 GHz Intel Core i7.

The runtimes above are the time between starting the intcode program and it
halting, as reported by the intcode-benchmark binary. They do not include time
spent loading the program, initialising the VM or cleaning up afterwards.

