
var QmlPseudoModule = {
    color: dummyBaseType
}

function dummyBaseType(string) {
    if (!string)
        return [];
    var r = parseInt(string.substr(1,2), 16);
    var g = parseInt(string.substr(3,2), 16);
    var b = parseInt(string.substr(5,2), 16);
    return [r,g,b];
}
