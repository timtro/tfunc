# tfunc

***a library *for* functional programming in C++17***

This is a sandbox to experiment with, benchmark and develop a C++17 library for functional programming. As Scott Meyers puts it, C++ is suited for *demanding systems* applications. That's certainly what I use it for. I'm primarily interested in discovering good functional patterns and idioms for C++, and not writing good Haskell code with an overly verbose C++ spelling. If the research leads to C++\-flavoured Haskell, so be it. I'm certainly happy to pursue it that far. However, I doubt that removing all concepts of the bare metal architecture is going to lead to performant systems code, especially in the embedded domain.

The C++ language benefits from a firm relationship to the underlying architecture of the computer. Those of us driven by performance will be quite accustomed to comparing fragments of program code to the resulting assembly language instructions, looking for slowdowns and possible tweaks that only make sense with an operational model of the hardware. On the other hand, functional languages have a less obvious relationship to corresponding machine instructions, but benefit from the lawful structure, declarative expressiveness and denotational semantics. There are many possibilities for combining these two styles, leading to various balances and trade-offs that almost certainly vary from application to application.

I want to understand how I can maintain a relationship with the bare metal, while using functional programming to make C++ code safer and more generic, especially for parallel, distributed and embedded systems. My initial efforts will focus on composition of structures. Functional programming obviously focuses on composition of functions and data. In C++, I can also look at class composition. (Perhaps I'll end up with something like Alexandrescu's policies.) I expect this to lead to an abstraction hierarchy wherein programming at the top level will have very concise Haskell-ish looking blocks. However, allowing the underlying C++ to influence the abstraction hierarchy will no doubt lead to a unique C++\-functional style.

The suitable level of abstraction for a program will certainly be application dependant, so I want the API at each level of abstraction to be rich enough to support development. Basing abstraction on category-theoretical concepts should lead to broad coverage of the solution space for a large set of problems, while presenting a narrow and orthogonal API at each abstraction layer.

Development efforts will be largely driven by unit tests targeting mathematical and semantic laws. Unit testing will be facilitated by Phil Nash's [Catch](https://github.com/philsquared/Catch) framework. That said, I'm not sure TDD is called for here, so let's not call it TDD. Let's call it co-developing an implementation of an interface and unit tests which carefully delineate the laws and semantics, or CDIIUTWCDLS. (Nice right to it, eh?)

The following list contains some loose road-map planning that I'll try to keep updated relative to the current progress.

* The `bench` folder is a shambolic mess of different implementations, competing * under Google's [benchmarking library](https://github.com/google/benchmark). In * writing this, I am undertaking a massive review of publicly available code * where people have implemented functional patterns in C++, or even some subset * of Prelude. The headers in this directory will have links and attributions, * with each code fragment in it's own namespace. The cpp source files will * contain unit tests for the functions in each name space.

* The *best* (whatever best happens to mean) will be copied into library sources * in `/include` and `/src`, and will be unit tested by sources in `/test`.

* After completing some basics, I'm going to focus on getting the passing/returning semantics right in terms of forwarding/moving/copying in the captures. This apparently requires a bit of [extra attention](https://vittorioromeo.info/index/blog/capturing_perfectly_forwarded_objects_in_lambdas.html).

* Monads will be written for lists, optionals, read/writer and maybe IO. * Implementations of those will be benchmarked and tested as well.

* I want to investigate implementations for the continuation monad. Then I want * to integrate with a functional reactive programming library, such as Sodium or * SFRP. At that point, this project will become part of a a signals and control * systems software framework that is part of my PhD project.

At the time of this writing the project is very new. I am still early in the
code review, and building up benchmarks for currying and composition.


## Notes

* The variadic flavours of functions are identified with three trailing middle-dots (`···`). For example, the variadic flavour of `curry` is named `curry···`.
  - The compose-key sequence for the middle-dot (·) is .^ or ^. (a period and a circumflex.)
  - Yes, the middle dot is allowed in the C++ standard (at least as far back as C++14).
  - Yes, I know what a stupid idea it is to use special characters in identifiers. However, I think this warrants an exception. This convention precludes the user from having to remember new function names for variadic flavours, and is approximately homogeneous with the variadic syntax in the language, which is three ASCII periods.

* In `compose`, `pipe`, `curry` and friends, arguments are perfectly forwarded, but function arguments are copied. If they are stateless, this costs as much as copying a pointer. If you want to manipulate function objects with large state (or lambdas with large captures), then I recommend you implement [Vittorio Romeo's](https: // vittorioromeo.info/index/blog/cpp17_curry.html) Curry and partial application. He uses a technique for perfect capturing.

  ## TODO

  * Replace is_nullary with std::is_invocable when clang implements it.
