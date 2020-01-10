type activity = [ | `Walking | `Standing | `Jumping];

type direction = [ | `Left | `Right];

type character = {
  node: Webapi.Dom.Element.t,
  x: float,
  y: float,
  dx: float,
  dy: float,
  dir: direction,
};

type inputs = {
  left: bool,
  right: bool,
  jump: bool,
};

let gravity = 0.15; // px / frame^2

let maxMoveSpeed = 2.5; // px / frame

let groundAccel = 0.06; // px / frame^2

let airAccel = 0.04; // px / frame^2

let groundFriction = 0.1; // px / frame^2

let airFriction = 0.02; // px / frame^2

let jumpCoefficient = 0.8; // px / frame^3

let minJumpSpeed = 4.0; // px / frame

let isAirborne = (c: character) => c.y > 0.0;

let isStanding = (c: character) => c.dx === 0.0;

let currentActivity = (c: character): activity =>
  isAirborne(c) ? `Jumping : isStanding(c) ? `Standing : `Walking;

let charSpriteDescriptor = (c: character) => {
  let activity = currentActivity(c);

  {j|$activity|j};
};

let accel = (c: character) => isAirborne(c) ? airAccel : groundAccel;

let friction = (c: character) => isAirborne(c) ? airFriction : groundAccel;

/**
 * When Mario is in motion, his position changes
 */
let velocity = (c: character): character => {
  ...c,
  x: c.x +. c.dx,
  y: c.y +. c.dy,
};

/**
 * When Mario is above the ground, he is continuously pulled downward
 */
let applyGravity = (c: character): character =>
  c.y <= -. c.dy ? {...c, y: 0.0, dy: 0.0} : {...c, dy: c.dy -. gravity};

let applyFriction = (c: character): character =>
  c.dx === 0.0
    ? c
    : Js.Math.abs_float(c.dx) <= friction(c)
        ? {...c, dx: 0.0}
        : c.dx > 0.0
            ? {...c, dx: c.dx -. friction(c)}
            : {...c, dx: c.dx +. friction(c)};

/**
 * Mario can move himself left/right with a fixed acceleration
 */
let walk = (left: bool, right: bool, c: character) =>
  left && !right
    ? {
      ...c,
      dx: Js.Math.max_float(-. maxMoveSpeed, c.dx -. accel(c)),
      dir: `Left,
    }
    : right && !left
        ? {
          ...c,
          dx: Js.Math.min_float(maxMoveSpeed, c.dx +. accel(c)),
          dir: `Right,
        }
        : applyFriction(c);

let jump = (jmp: bool, c: character): character =>
  jmp && !isAirborne(c)
    ? {...c, dy: minJumpSpeed +. jumpCoefficient *. Js.Math.abs_float(c.dx)}
    : !jmp && isAirborne(c) && c.dy > 0.0 ? {...c, dy: c.dy -. gravity} : c;

let marioLogic = (inputs: inputs, c: character): character =>
  c
  |> velocity
  |> applyGravity
  |> walk(inputs.left, inputs.right)
  |> jump(inputs.jump);
