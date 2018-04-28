import * as wist from "../../dist/libwist";
import * as process from "process"
wist['onRuntimeInitialized'] = () => {
    const test = (wist as any).parse("function function");
    console.log(test.get(0))
};
