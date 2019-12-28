Intcode VM Benchmarks
=====================

This is a suite of intcode VM benchmarks from here:

https://www.reddit.com/r/adventofcode/comments/egq9xn/2019_day_9_intcode_benchmarking_suite/


Benchmark Results
-----------------

| Program       | Input      | Output                       | Runtime       |
| ------------- | ---------- | ---------------------------- | ------------- |
| sum-of-primes | 100000     | 454396537                    |     14.6 ms   |
| ackermann     | 3,6        | 509                          |      8.3 ms   |
| ackermann     | 4,1        | 65533                        | 129847.2 ms   |
| isqrt         | 130        | 11                           |      0.012 ms |
| divmod        | 1024,3     | 341,1                        |      0.012 ms |
| factor        | 19338240   | 2,2,2,2,2,2,2,2,2,2,3,5,1259 |      1.8 ms   |
| factor        | 2147483647 | 2147483647                   |     35.3 ms   |

Taken on a 2015 MacBook Pro with a 2.5 GHz Intel Core i7.

The runtimes above are the time between starting the intcode program and it
halting, as reported by the intcode-benchmark binary. They do not include time
spent loading the program, initialising the VM or cleaning up afterwards.

