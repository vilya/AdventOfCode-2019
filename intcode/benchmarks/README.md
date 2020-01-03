Intcode VM Benchmarks
=====================

This is a suite of intcode VM benchmarks from here:

https://www.reddit.com/r/adventofcode/comments/egq9xn/2019_day_9_intcode_benchmarking_suite/


Benchmark Results
-----------------

| Program       | Input         | Output                       | Runtime       |
| ------------- | ------------- | ---------------------------- | ------------- |
| sum-of-primes | 100000        | 454396537                    |      3.7 ms   |
| sum-of-primes | 2000000       | 142913828922                 |    103.2 ms   |
| sum-of-primes | 100000000     | 279209790387276              |   7801.6 ms   |
| ackermann     | 3,6           | 509                          |      2.5 ms   |
| ackermann     | 4,1           | 65533                        |  34081.2 ms   |
| ackermann     | 3,14          | 131069                       | 137390.7 ms   |
| isqrt         | 130           | 11                           |      0.005 ms |
| isqrt         | 1000000000    | 31622                        |   8524.5 ms   |
| divmod        | 1024,3        | 341,1                        |      0.005 ms |
| factor        | 19338240      | 2,2,2,2,2,2,2,2,2,2,3,5,1259 |      0.448 ms |
| factor        | 2147483647    | 2147483647                   |     10.0 ms   |
| factor        | 1689259081189 | 1299709,1299721              |    209.5 ms   |

Taken on a 2015 MacBook Pro with a 2.5 GHz Intel Core i7.

The runtimes above are the time between starting the intcode program and it
halting, as reported by the intcode-benchmark binary. They do not include time
spent loading the program, initialising the VM or cleaning up afterwards.

