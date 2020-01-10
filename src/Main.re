open Mario;
open MarioDOM;
open WonkaDOM;
open Wonka.Types;

type state = {mario: character};

let gameLogic = (inputs: inputs, state: state): state => {
  mario: marioLogic(inputs, state.mario),
};

let render = (state: state, ()) =>
  updateSprite(updatePosition(state.mario));

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

    let leftInputs: sourceT(bool) = keyPressed("ArrowLeft");
    let rightInputs: sourceT(bool) = keyPressed("ArrowRight");
    let jumpInputs: sourceT(bool) = keyPressed(" ");

    let inputs: sinkT(inputs) => unit =
      sink => {
        leftInputs((. signal) =>
          switch (signal) {
          | Start(talkback) => sink(. Start(talkback))
          | Push(left) =>
            rightInputs((. signal) =>
              switch (signal) {
              | Start(talkback) => sink(. Start(talkback))
              | Push(right) =>
                jumpInputs((. signal) =>
                  switch (signal) {
                  | Start(talkback) => sink(. Start(talkback))
                  | Push(jump) =>
                    sink(. Push(getInputs(left, right, jump)))
                  | End => sink(. End)
                  }
                )
              | End => sink(. End)
              }
            )
          | End => sink(. End)
          }
        );
      };

    let game = fromAnimationFrame |> Wonka.subscribe((. event) => ());

    ();
  });
