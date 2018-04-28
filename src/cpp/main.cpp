#include "antlr4-runtime.h"
#include "BrightScriptLexer.h"
#include "BrightscriptEventListener.h"
#include "SyntaxErrorListener.h"
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

using namespace antlr4;
using namespace std;
using namespace emscripten;

EMSCRIPTEN_KEEPALIVE vector<SyntaxError> parse(string source)
{
    vector<SyntaxError> errors = {};
    ANTLRInputStream input(source);

    BrightScriptLexer lexer(&input);
    lexer.removeErrorListeners();

    SyntaxErrorListener errorListener(&errors);
    lexer.addErrorListener(&errorListener);

    CommonTokenStream tokens(&lexer);

    BrightScriptParser parser(&tokens);
    parser.removeErrorListeners();
    parser.addErrorListener(&errorListener);
    
    tree::ParseTree *tree = parser.startRule();

    BrightscriptEventListener listener;
    tree::ParseTreeWalker::DEFAULT.walk(&listener, tree);
    return errors;
}

int main()
{
    EM_ASM(
        FS.mkdir('/working');
        FS.mount(NODEFS, {root : '.'}, '/working'););
    return 0;
}

EMSCRIPTEN_BINDINGS(wist_module)
{
    emscripten::function("parse", &parse);
    emscripten::value_object<SyntaxError>("SyntaxError")
        .field("message", &SyntaxError::message)
        .field("line", &SyntaxError::line)
        .field("column", &SyntaxError::column);
    emscripten::register_vector<SyntaxError>("SyntaxErrorList");
};
