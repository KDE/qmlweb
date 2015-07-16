# A QML engine in a web browser

The QML language is well-suited for building web applications. Goal is
to develop the required technologies for that. This is a runtime
environment in JavaScript and the required QML modules to render in
web browsers. Other applications can be considered. Main focus is on
developing hybrid-applications using native (Qt) and web technologies
of any scale, not forgetting the creation of sole web applications.
QmlWeb is not intended to run native QtQuick applications using QmlWeb
without migration effort.

### Parts

QmlWeb contains serveral modules:
* A QML to JavaScript compiler QmlJSc
* A JavaScript runtime to run compiled QML projects
* A set of QML modules written in JavaScript to provide API for it. Among them is an implementation of QtQuick for rendering QtQuick GUIs to a web browser environment using HTML and JavaScript/DOM.

### Installation

//--Not documented yet--//

### Get started

To use QmlWeb, you need a simple html file to load and start QmlWeb.
This simple file will do:

```
<!DOCTYPE html>
<html>
    <head>
        <title>QmlWeb Demo</title>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <script type="text/javascript" src="qml.js"></script>
    </head>
    <body style="margin: 0;">
        <script type="text/javascript">
            var engine = new QWQmlEngine();
            var component = new QWQmlComponent(engine, "main.qml");
            var object = component.create();
        </script>
    </body>
</html>
```

The next thing you need is a QML-file:

```
import QtQuick 2.0

Item {

    Text {
        anchors.centerIn: parent
        text: "Hello World!"
        font.pointSize: 12
        color: grey
    }

}
```

#### Compile

If you want it quick and dirty:

* Create a build directory and enter it
* call the Qml.js compiler
* move your compiled file to your server

```
mkdir build && cd build
qmljsc main.qml
mv main.qml.js /srv/www/htdocs # change path to wherever you want it copied
```

If you want it done right:

//--Not documented yet--//


_Hint: To get the root Item fill the whole screen, just don't set it's size._