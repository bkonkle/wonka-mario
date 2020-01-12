open Wonka.Types;

/**
 * Creates a past dependent source. The function takes the value of the input source and
 * the previous value of the output source to produce the new value of the output source.
 */
[@genType]
let foldp: (('a, 'b) => 'b, 'b, sourceT('a)) => sourceT('b) =
  (func, seed, source) => {
    let acc = ref(seed);

    source
    |> Wonka.onPush((. value) => acc := func(value, acc^))
    |> Wonka.map((. _) => acc^);
  };
