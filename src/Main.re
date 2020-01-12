open Mario;
open MarioDOM;
open WonkaDOM;

type state = {mario: character};

let gameLogic = (inputs: inputs, state: state): state => {
  mario: marioLogic(inputs, state.mario),
};

let render = (. state: state) => updateSprite(updatePosition(state.mario));

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

    let game =
      inputs
      |> Wonka.combine(fromAnimationFrame)
      |> Wonka.map((. (_, inputs)) => inputs);

    game
    |> WonkaUtils.foldp(gameLogic, initialState)
    |> Wonka.map(render)
    |> Wonka.subscribe((. _) => ())
    |> ignore;
  });

if ([%raw {| require.main === module |}]) {
  main();
};
