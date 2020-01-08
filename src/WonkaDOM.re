open Webapi;
open Wonka;
open Wonka.Types;

[@genType];
type mouseButton = [ | `Left | `Middle | `IE8Middle | `Right];

[@genType]
let fromWindowEvent = (event: string): sourceT('a) =>
  curry(sink => {
    let addEventListener: (string, 'a => unit) => unit = [%raw
      {|
        function (event, handler) {
          window.addEventListener(event, handler);
        }
      |}
    ];

    let removeEventListener: (string, 'a => unit) => unit = [%raw
      {|
        function (event, handler) {
          window.removeEventListener(event, handler);
        }
      |}
    ];

    let handler = event => sink(. Push(event));

    sink(.
      Start(
        (. signal) =>
          switch (signal) {
          | Close => removeEventListener(event, handler)
          | _ => ()
          },
      ),
    );

    addEventListener(event, handler);
  });

[@genType]
let isKey = (key: string): operatorT('a, 'a) =>
  curry(source =>
    curry(sink => {
      Wonka_helpers.captureTalkback(source, (. signal, talkback) =>
        switch (signal) {
        | Push(event) when Dom.KeyboardEvent.key(event) !== key =>
          talkback(. Pull)
        | _ => sink(. signal)
        }
      )
    })
  );

[@genType]
let isButton = (button: int): operatorT('a, 'a) =>
  curry(source =>
    curry(sink => {
      Wonka_helpers.captureTalkback(source, (. signal, talkback) => {
        switch (signal) {
        | Push(event) when Dom.MouseEvent.button(event) !== button =>
          talkback(. Pull)
        | _ => sink(. signal)
        }
      })
    })
  );

[@genType]
let distinct: operatorT('a, 'a) =
  curry(source => {
    let prev = ref(None);

    curry(sink => {
      Wonka_helpers.captureTalkback(source, (. signal, talkback) =>
        switch (signal) {
        | Push(event) =>
          switch (prev^) {
          | Some(prevEvent) when event === prevEvent => talkback(. Pull)
          | _ =>
            prev := Some(event);
            sink(. signal);
          }
        | _ => sink(. signal)
        }
      )
    });
  });

/**
 * Creates a source which will be `true` when the key matching the given key code is pressed, and
 * `false` when it's released.
 */
[@genType]
let keyPressed: (string, sinkT(bool)) => unit =
  key => {
    merge([|
      fromWindowEvent("keydown") |> isKey(key) |> Wonka.map((. _) => true),
      fromWindowEvent("keyup") |> isKey(key) |> Wonka.map((. _) => false),
    |])
    |> distinct;
  };

/**
 * Creates a source which will be `true` when the given mouse button is pressed, and `false` when
 * it's released.
 */
[@genType]
let mouseButton: (int, sinkT(bool)) => unit =
  button => {
    merge([|
      fromWindowEvent("mousedown")
      |> isButton(button)
      |> Wonka.map((. _) => true),
      fromWindowEvent("mouseup")
      |> isButton(button)
      |> Wonka.map((. _) => false),
    |])
    |> distinct;
  };

/**
 * Creates a source which will be `true` when the given mouse button is pressed, and `false` when
 * it's released. Note: in IE8 and earlier you need to use MouseIE8MiddleButton if you want to query
 * the middle button.
 */
[@genType]
let mouseButtonPressed = (button: mouseButton) =>
  switch (button) {
  | `Left => mouseButton(0)
  | `Right => mouseButton(2)
  | `Middle => mouseButton(2)
  | `IE8Middle => mouseButton(4)
  };

/**
 * A source containing the current state of the touch device.
 */
[@genType]
let touch: Wonka.Types.sinkT(Dom.TouchEvent.touchList) => unit =
  merge([|
    fromWindowEvent("touchstart")
    |> Wonka.map((. event) => Dom.TouchEvent.touches(event)),
    fromWindowEvent("touchend")
    |> Wonka.map((. event) => Dom.TouchEvent.touches(event)),
    fromWindowEvent("touchmove")
    |> Wonka.map((. event) => Dom.TouchEvent.touches(event)),
    fromWindowEvent("touchcancel")
    |> Wonka.map((. event) => Dom.TouchEvent.touches(event)),
  |]);

[@bs.get] external touchLength: Dom.TouchEvent.touchList => int = "length";

/**
 * A source which will be `true` when at least one finger is touching the touch device, and `false`
 * otherwise.
 */
let tap: Wonka.Types.sinkT(bool) => unit =
  touch |> Wonka.map((. touches) => touchLength(touches) > 0) |> distinct;

/**
 * A signal containing the current mouse position.
 */
[@genType]
let mousePos =
  fromWindowEvent("mousemove")
  |> Wonka.map((. event) => {
       let handler:
         'a =>
         Js.t({
           .
           "x": float,
           "y": float,
         }) = [%raw
         {|
          function (e) {
            if (e.pageX && e.pageY) {
              return {x: e.pageX, y: e.pageY};
            } else if (e.clientX && e.clientY) {
              return {
                x: e.clientX + document.body.scrollLeft + document.documentElement.scrollLeft,
                y: e.clientY + document.body.scrollTop + document.documentElement.scrollTop
              };
            } else {
              throw new Error('Mouse event has no recognizable coordinates');
            }
          }
        |}
       ];

       handler(event);
     });
