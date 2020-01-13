open Webapi;
open Mario;

let groundHeight = 40.0; // px

let updatePosition: character => unit =
  c => {
    let left = c.x;
    let bottom = c.y +. groundHeight;

    c.node
    |> Dom.Element.asHtmlElement
    |> Js.Option.getExn
    |> Dom.HtmlElement.style
    |> Dom.CssStyleDeclaration.setProperty("left", {j|$(left)px|j}, "");

    c.node
    |> Dom.Element.asHtmlElement
    |> Js.Option.getExn
    |> Dom.HtmlElement.style
    |> Dom.CssStyleDeclaration.setProperty("bottom", {j|$(bottom)px|j}, "");
  };

let updateSprite: character => unit =
  c => {
    let sprite = charSpriteDescriptor(c);

    (c.node |> Dom.Element.asHtmlElement |> Js.Option.getExn)
    ->Dom.HtmlElement.setClassName(sprite);
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
    |> Webapi.Dom.Document.asHtmlDocument
    |> Js.Option.andThen((. htmlDocument) =>
         Dom.HtmlDocument.body(htmlDocument)
       )
    |> Js.Option.firstSome(
         Dom.document |> Dom.Document.getElementById("mario"),
       )
    |> Js.Option.getExn
  ) {
  | _ => Js.Exn.raiseError("Unable to find a suitable node for Mario")
  };
