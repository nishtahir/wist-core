const wist = require("./libwist");
wist['onRuntimeInitialized'] = function() {
    console.log("loaded...");
    var test = wist.parse("function function");
    console.log(test.get(0).message);
};
