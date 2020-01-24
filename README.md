# Wonka Mario

![Mario](https://user-images.githubusercontent.com/30199/56088170-97d4dc80-5e38-11e9-945b-293123d4fca7.gif)

An example of a Mario game screen using the excellent [Wonka](https://github.com/kitten/wonka) FRP library by [Phil Plückthun](https://github.com/kitten) to manage user input and game state.

Adapted from [Michael Ficarra's](https://github.com/michaelficarra) example [here](https://github.com/michaelficarra/purescript-demo-mario).

## Play a Demo

Play a live demo [here](https://bkonkle.github.io/wonka-mario/)!

## What libraries does it use?

* [Wonka](https://github.com/kitten/wonka) v4
* [WonkaDOM](https://github.com/bkonkle/wonka-dom)

## How does it work?

In the [Main.re](src/Main.re) module, the `fromAnimationFrame` Wonka event source emits an event for each available frame. On each animation frame tick, the inputs are sampled. (The code is currently using `combine` and `map` as a workaround until I can identify an issue with using `sample` instead.)

```re
inputs
|> Wonka.combine(fromAnimationFrame)
|> Wonka.map((. (_, inputs)) => inputs)
```

The main game loop is handled by a simple [`scan`](https://wonka.kitten.sh/api/operators#scan) function called `gameLogic` that takes the inputs (left, right, and jump) and the current character state, and combines a number of operations to apply friction and update the character with a new state. This gives us an efficient and type-safe way to manage state and events over time, which I use to evolve the game state on each animation frame.

```re
// Main.re
let gameLogic: (. state, inputs) => state =
  (. state, inputs) => {mario: marioLogic(inputs, state.mario)};
```

```re
|> Wonka.scan(gameLogic, initialState)
```

Finally, the sprite style and class names are updated, rendering the current state to the browser.

```re
let render: (. state) => unit =
  (. state) => {
    updatePosition(state.mario);
    updateSprite(state.mario);
  };
```

```re
|> Wonka.onPush(render)
```

## To-Do

* [ ] Unit tests
* [ ] More sources
