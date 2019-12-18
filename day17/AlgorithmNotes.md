Algorithm Notes
===============

- The master movement routine ("master" from now on) can only contain calls to subroutines
  - One of the subroutines must start with whatever action is first in the string.
  - One of the subroutines must end with whatever action is last in the string.
  - If we split the path at the contents of subroutine A, then the same rule applies
    recursively to each of the substrings that produces.

- Subroutines cannot call other subroutines.
- If we replace all occurrences of a subroutine in the orginial path with the subroutine's
  name, then:
  - None of the remaining subroutines can be longer than the largest subsequence without
    an A, B or C.
  - Can use this to quickly check whether it's possible to encode the entire string using 
    the remaining subroutine(s).
    - If we replaced this subsequence with A

- Maximum possible instructions in a subroutine is 10 (max length = 10, but
  commas and the trailing newline count against this).
  - Two digit numbers count as two characters (numbers must be stringified), so the 
    maximum is less if you have any of those.
  - Width and height of the map are both less than 100, so numbers can only be 1 or two digits.
- Maximum possible subroutine calls in master is 10 (same reason).

- In the input I got, you start at one end of a path and can reach the other
  without backtracking by ignoring intersections and turning the only way
  possible at corners. 
  - This may not work for all inputs. It wouldn't work if you started on a loop
    of scaffolding and the only way off it was to turn at an intersection along
    one of the sides, for example, but I don't know whether that's something
    the author allows.
  - It means the path will always alternate between numbers and turns. You'll 
    never get two turns in a row, or two numbers in a row.
