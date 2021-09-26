# contracts
This is an implementation of contracts for functions in C++.
I tried to make a simpler solution to use from the ones that we have today.
I may have missed that goal but it was fun making it.


one of the goals of this library is to improve on existing solutions:
1. it is much simpler to use than boost's. much less boiler-plate.
2. it is more modern and more customizable than alexeiz' solution.
3. be more error centred than both.

the library comes in two flavors:
1. Manual
2. Throwing

## Manual condition handling
in case you are not using exception for error handling you can manually call the checking methods. like so:
```c++
```

## Throwing condition handling
for normal use use it like this:

## deriving contracts
