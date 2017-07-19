# tfunc—a library *for* functional programming in C++17

This is a personal garage to experiment with, benchmark and develop a C++17 library for functional programming. I'm not interested in writing Haskell code with C++ spelling. Rather, I'm exploring functional idioms and patterms that can be used to make *better* C++ code. As Scott Mayers

Development efforts will be largely driven by unit tests targeting semantic
 and compositional laws. Unit testing will be facilitated by Phil Nash's [Catch](https://github.com/philsquared/Catch) framework.

In the earliest stages, basic facilities for currying, function composition and algebraic data structures will be worked out. The `bench` folder will be a shambolic mess of different implementations, competing under Google's [benchmarking library](https://github.com/google/benchmark). The simplest, but best performing of these will be copied into library sources in `/include` and `/src`.

Next, monads will be written for lists, optionals, read/writer and maybe IO. Implementations of those will be benchmarked and tested as well.

Around this point, I'm going to focus on getting the passing/returning semantics right in terms of forwarding/moving/copying. This is going to require a bit of extra attention in the case of lambdas. (see [here]( https://vittorioromeo.info/index/blog/capturing_perfectly_forwarded_objects_in_lambdas.html).)

In the final stages, I want to investigate implementations for the continuation monad. Then I want to integrate with a functional reactive programming library, such as Sodium or SFRP.
