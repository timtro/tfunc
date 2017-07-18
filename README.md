# tfunc: A Library *for* Functional Programming *in* C++17

---

This is a personal garage to experiment with, benchmark and develop a C++17
library for functional programming.

In the earliest stages, basic facilities for currying, function composition and
algebraic data structures will be worked out. The `bench` folder will be a
shambolic mess of different implementations, competing under Google's [benchmarking library](https://github.com/google/benchmark).

Next, monads will be written for lists, optionals, read/writer and maybe IO.
(NB: I'm not interesting in writing Haskell code with C++ spelling) and
implementations of those will be benchmarked as well. I'm going to be interested
in figuring out how much TMP/SFINAE will be required for efficiency.

Around this point, I'm going to focus on getting the passing/returning semantics
right in terms of forwarding/moving/copying. This is going to require a bit of
extra attention in the case of lambdas. (see [here](
https://vittorioromeo.info/index/blog/capturing_perfectly_forwarded_objects_in_lambdas.html).)

In the final stages, I want to investigate implementations for the continuation
monad. Then I want to integrate with a functional reactive programming library,
such as Sodium or SFRP.

Development efforts will be largely driven by unit tests targeting semantic
 and compositional laws. Unit testing will be facilitated by Phil Nash's [Catch](https://github.com/philsquared/Catch) framework.
