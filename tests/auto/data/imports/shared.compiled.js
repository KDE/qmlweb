new (function Shared() {

this.xProp = 7;

this.add = function(summand1, summand2) {
    try {
        __.karl.ok.set(false);
    } catch(e) {
    }

    return summand1 + summand2;
}

})()

