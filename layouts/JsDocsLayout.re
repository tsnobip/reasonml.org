%raw
"require('../styles/main.css')";

%raw
{|
let hljs = require('highlight.js/lib/highlight');
let reasonHighlightJs = require('reason-highlightjs');
hljs.registerLanguage('reason', reasonHighlightJs);
|};

module Link = Next.Link;

// Structure defined by `scripts/extract-indices.js`
let indexData:
  Js.Dict.t({
    .
    "moduleName": string,
    "headers": array(string),
  }) = [%raw
  "require('../index_data/js_api_index.json')"
];

// Retrieve package.json to access the version of bs-platform.
let package: {. "dependencies": {. "bs-platform": string}} = [%raw
  "require('../package.json')"
];

module Sidebar = SidebarLayout.Sidebar;
module UrlPath = SidebarLayout.UrlPath;
module NavItem = Sidebar.NavItem;
module Category = Sidebar.Category;

let overviewNavs = [|
  NavItem.{name: "Introduction", href: "/apis/javascript/latest/js"},
|];

let apiNavs = [|
  NavItem.{name: "Array2", href: "/apis/javascript/latest/js/array-2"},
  {name: "Array", href: "/apis/javascript/latest/js/array"},
  {name: "Console", href: "/apis/javascript/latest/js/console"},
  {name: "Date", href: "/apis/javascript/latest/js/date"},
  {name: "Dict", href: "/apis/javascript/latest/js/dict"},
  {name: "Exn", href: "/apis/javascript/latest/js/exn"},
  {name: "Float", href: "/apis/javascript/latest/js/float"},
  {name: "Global", href: "/apis/javascript/latest/js/global"},
  {name: "Int", href: "/apis/javascript/latest/js/int"},
  {name: "Json", href: "/apis/javascript/latest/js/json"},
  {name: "List", href: "/apis/javascript/latest/js/list"},
  {name: "Math", href: "/apis/javascript/latest/js/math"},
  {name: "NullUndefined", href: "/apis/javascript/latest/js/null-undefined"},
  {name: "Null", href: "/apis/javascript/latest/js/null"},
  {name: "Nullable", href: "/apis/javascript/latest/js/nullable"},
  {name: "Obj", href: "/apis/javascript/latest/js/obj"},
  {name: "Option", href: "/apis/javascript/latest/js/option"},
  {name: "Promise", href: "/apis/javascript/latest/js/promise"},
  {name: "Re", href: "/apis/javascript/latest/js/re"},
  {name: "Result", href: "/apis/javascript/latest/js/result"},
  {name: "String2", href: "/apis/javascript/latest/js/string-2"},
  {name: "String", href: "/apis/javascript/latest/js/string"},
  {
    name: "TypedArrayArrayBuffer",
    href: "/apis/javascript/latest/js/typed-array_array-buffer",
  },
  {
    name: "TypedArrayDataView",
    href: "/apis/javascript/latest/js/typed-array_data-view",
  },
  {
    name: "TypedArrayFloat32Array",
    href: "/apis/javascript/latest/js/typed-array_float-32-array",
  },
  {
    name: "TypedArrayFloat64Array",
    href: "/apis/javascript/latest/js/typed-array_float-64-array",
  },
  {
    name: "TypedArrayInt8Array",
    href: "/apis/javascript/latest/js/typed-array_int-8-array",
  },
  {
    name: "TypedArrayInt16Array",
    href: "/apis/javascript/latest/js/typed-array_int-16-array",
  },
  {
    name: "TypedArrayInt32Array",
    href: "/apis/javascript/latest/js/typed-array_int-32-array",
  },
  {
    name: "TypedArrayTypeS",
    href: "/apis/javascript/latest/js/typed-array_type-s",
  },
  {
    name: "TypedArrayUint8Array",
    href: "/apis/javascript/latest/js/typed-array_uint-8-array",
  },
  {
    name: "TypedArrayUint8ClampedArray",
    href: "/apis/javascript/latest/js/typed-array_uint-8-clamped-array",
  },
  {
    name: "TypedArrayUint16Array",
    href: "/apis/javascript/latest/js/typed-array_uint-16-array",
  },
  {
    name: "TypedArrayUint32Array",
    href: "/apis/javascript/latest/js/typed-array_uint-32-array",
  },
  {
    name: "TypedArray2ArrayBuffer",
    href: "/apis/javascript/latest/js/typed-array-2_array-buffer",
  },
  {
    name: "TypedArray2DataView",
    href: "/apis/javascript/latest/js/typed-array-2_data-view",
  },
  {
    name: "TypedArray2Float32Array",
    href: "/apis/javascript/latest/js/typed-array-2_float-32-array",
  },
  {
    name: "TypedArray2Float64Array",
    href: "/apis/javascript/latest/js/typed-array-2_float-64-array",
  },
  {
    name: "TypedArray2Int8Array",
    href: "/apis/javascript/latest/js/typed-array-2_int-8-array",
  },
  {
    name: "TypedArray2Int16Array",
    href: "/apis/javascript/latest/js/typed-array-2_int-16-array",
  },
  {
    name: "TypedArray2Int32Array",
    href: "/apis/javascript/latest/js/typed-array-2_int-32-array",
  },
  {
    name: "TypedArray2Uint8Array",
    href: "/apis/javascript/latest/js/typed-array-2_uint-8-array",
  },
  {
    name: "TypedArray2Uint8ClampedArray",
    href: "/apis/javascript/latest/js/typed-array-2_uint-8-clamped-array",
  },
  {
    name: "TypedArray2Uint16Array",
    href: "/apis/javascript/latest/js/typed-array-2_uint-16-array",
  },
  {
    name: "TypedArray2Uint32Array",
    href: "/apis/javascript/latest/js/typed-array-2_uint-32-array",
  },
  {name: "TypedArray2", href: "/apis/javascript/latest/js/typed-array-2"},
  {name: "TypedArray", href: "/apis/javascript/latest/js/typed-array"},
  {name: "Types", href: "/apis/javascript/latest/js/types"},
  {name: "Undefined", href: "/apis/javascript/latest/js/undefined"},
  {name: "Vector", href: "/apis/javascript/latest/js/vector"},
|];

let categories = [|
  Category.{name: "Overview", items: overviewNavs},
  {name: "API", items: apiNavs},
|];

module Docs = {
  [@react.component]
  let make = (~components=SidebarLayout.ApiMd.components, ~children) => {
    let router = Next.Router.useRouter();
    let route = router##route;

    // Gather data for the CollapsibleSection
    let headers =
      Belt.Option.(
        Js.Dict.get(indexData, route)
        ->map(data => data##headers)
        ->getWithDefault([||])
      );

    let moduleName =
      Belt.Option.(
        Js.Dict.get(indexData, route)
        ->map(data => data##moduleName)
        ->getWithDefault("?")
      );

    let (isSidebarOpen, setSidebarOpen) = React.useState(_ => false);
    let toggleSidebar = () => setSidebarOpen(prev => !prev);
    let urlPath = UrlPath.parse(~base="/apis/javascript", route);

    let breadcrumbs =
      Belt.Option.map(
        urlPath,
        v => {
          let {UrlPath.version} = v;
          let prefix =
            UrlPath.[
              {name: "API", href: "/apis"},
              {name: "JavaScript", href: "/apis/javascript/" ++ version},
            ];
          UrlPath.toBreadCrumbs(~prefix, v);
        },
      );

    let toplevelNav =
      switch (urlPath) {
      | Some(urlPath) =>
        let version = UrlPath.(urlPath.version);
        let backHref = Some(UrlPath.fullUpLink(urlPath));
        <Sidebar.ToplevelNav title="Js Module" version ?backHref />;
      | None => React.null
      };

    // Todo: We need to introduce router state to be able to
    //       listen to anchor changes (#get, #map,...)
    let preludeSection =
      route !== "/apis/javascript/latest/js"
        ? <Sidebar.CollapsibleSection
            onHeaderClick={_ => setSidebarOpen(_ => false)}
            headers
            moduleName
          />
        : React.null;

    let sidebar =
      <Sidebar
        isOpen=isSidebarOpen
        toggle=toggleSidebar
        categories
        route={
          router##route;
        }
        preludeSection
        toplevelNav
      />;

    <SidebarLayout
      theme=`Js
      components
      sidebar
      breadcrumbs=?breadcrumbs
      route={
        router##route;
      }>
      children
    </SidebarLayout>;
  };
};

module Prose = {
  [@react.component]
  let make = (~children) => {
    <Docs components=SidebarLayout.ProseMd.components> children </Docs>;
  };
};
