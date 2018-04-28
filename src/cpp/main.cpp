#include <iostream>
#include "antlr4-runtime.h"
#include "BrightScriptLexer.h"
#include "BrightScriptParser.h"
#include "BaseErrorListener.h"
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>

using namespace antlr4;
using namespace std;
using namespace emscripten;

struct SyntaxError
{
    string message;
    uint32_t line;
    uint32_t column;
};

class SyntaxErrorListener : public BaseErrorListener
{
  public:
    vector<SyntaxError> *errors;

    SyntaxErrorListener(vector<SyntaxError> *error_list) : errors(error_list) {}

    virtual void syntaxError(Recognizer *recognizer, Token *offendingSymbol, size_t line,
                             size_t charPositionInLine, const std::string &msg, std::exception_ptr e)
    {
        errors->push_back(SyntaxError{msg, line, charPositionInLine});
    }
};

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
    parser.startRule();

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
