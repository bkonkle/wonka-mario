open Mario;
open MarioDOM;
open WonkaDOM;

type state = {mario: character};

let gameLogic: (. state, inputs) => state =
  (. state, inputs) => {mario: marioLogic(inputs, state.mario)};

let render: (. state) => unit =
  (. state) => {
    updatePosition(state.mario);
    updateSprite(state.mario);
  };

let getInputs = (left: bool, right: bool, jump: bool) => {left, right, jump};

let main = () =>
  onDomContentLoaded(() => {
    let initialState = {
      mario: {
        node: getMarioNode(),
        x: (-50.0),
        y: 0.0,
        dx: 3.0,
        dy: 6.0,
        dir: `Right,
      },
    };

    let leftInputs = keyPressed("ArrowLeft");
    let rightInputs = keyPressed("ArrowRight");
    let jumpInputs = keyPressed(" ");

    let inputs =
      jumpInputs
      |> Wonka.combine(rightInputs)
      |> Wonka.combine(leftInputs)
      |> Wonka.map((. (left, (right, jump))) =>
           getInputs(left, right, jump)
         );

    inputs
    |> Wonka.combine(fromAnimationFrame)
    |> Wonka.map((. (_, inputs)) => inputs)
    |> Wonka.scan(gameLogic, initialState)
    |> Wonka.onPush(render)
    |> Wonka.publish
    |> ignore;
  });

if ([%raw {| require.main === module |}]) {
  main();
};
