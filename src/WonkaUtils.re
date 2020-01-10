open Wonka.Types;

/**
 * Creates a source which yields the current value of the second source every time the first
 * source yields.
 */;
// sampleOn = <B>(s: Signal<B>): Signal<B> => {
//   const out = constant(s.get())

//   this.subscribe(() => {
//     out.set(s.get())
//   })

//   return out
// }
// let sampleOn: (sourceT('b), sourceT('a), sinkT('b)) => sourceT('b) =
//   (b, a, sink) => {
//     b((. signal) =>
//       switch (signal) {
//       | Start(talkback) => sink(. Start(talkback))
//       | Push(left) => sink(. Push(left))
//       | End => sink(. End)
//   };
