#ifndef __BRIGHTSCRIPT_EVENT_LISTENER_H__
#define __BRIGHTSCRIPT_EVENT_LISTENER_H__

#include "parser/BrightScriptBaseListener.h"

using namespace std;
using namespace antlr4;

class BrightscriptEventListener : public BrightScriptBaseListener
{
  public:
    BrightscriptEventListener(BrightScriptParser *parser);
    virtual void enterFunctionDeclaration(BrightScriptParser::FunctionDeclarationContext *ctx) override;
    virtual void enterSubDeclaration(BrightScriptParser::SubDeclarationContext *ctx) override;

  private:
    BrightScriptParser *parser;
    vector<string> functionNames = {};
    bool isFunction(ParserRuleContext *context);
    bool isSub(ParserRuleContext *context);
    bool functionNameExists(string nameToCheck);
    void checkDeclaration(BrightScriptParser::UntypedIdentifierContext *context);
};
#endif
