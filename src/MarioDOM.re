open Webapi;
open Mario;

let (andThen, firstSome, getExn) = Js.Option.(andThen, firstSome, getExn);
let asHtmlElement = Dom.Element.(asHtmlElement);
let (asHtmlDocument, getElementById) =
  Dom.Document.(asHtmlDocument, getElementById);
let (style, setClassName) = Dom.HtmlElement.(style, setClassName);
let body = Dom.HtmlDocument.body;
let setProperty = Dom.CssStyleDeclaration.setProperty;

let groundHeight = 40.0; // px

let updatePosition: character => unit =
  c => {
    let left = c.x;
    let bottom = c.y +. groundHeight;

    c.node
    |> asHtmlElement
    |> getExn
    |> style
    |> setProperty("left", {j|$(left)px|j}, "");

    c.node
    |> asHtmlElement
    |> getExn
    |> style
    |> setProperty("bottom", {j|$(bottom)px|j}, "");
  };

let updateSprite: character => unit =
  c => {
    let sprite = charSpriteDescriptor(c);

    (c.node |> asHtmlElement |> getExn)->setClassName(sprite);
  };

let onDomContentLoaded: (unit => unit) => unit =
  action => {
    let readyState = [%bs.raw {| document.readyState |}];

    let addEventListener: ('a => unit) => unit = [%raw
      {|
      function (handler) {
        window.addEventListener('DOMContentLoaded', handler);
      }
    |}
    ];

    readyState === "interactive" ? action() : addEventListener(action);
  };

let getMarioNode = () =>
  try(
    Dom.document
    |> asHtmlDocument
    |> andThen((. htmlDocument) => body(htmlDocument))
    |> firstSome(Dom.document |> getElementById("mario"))
    |> getExn
  ) {
  | _ => Js.Exn.raiseError("Unable to find a suitable node for Mario")
  };
