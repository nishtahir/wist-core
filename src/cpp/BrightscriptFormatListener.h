#ifndef __BRIGHTSCRIPT_FORMAT_LISTENER_H__
#define __BRIGHTSCRIPT_FORMAT_LISTENER_H__

#include "parser/BrightScriptBaseListener.h"
#include <emscripten/val.h>

using namespace std;
using namespace antlr4;

class BrightscriptFormatListener : public BrightScriptBaseListener
{
  public:
    BrightscriptFormatListener(CommonTokenStream *);
    virtual void enterEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
    virtual void exitEveryRule(antlr4::ParserRuleContext * /*ctx*/) override;
    virtual void visitTerminal(antlr4::tree::TerminalNode * /*node*/) override;
    virtual void visitErrorNode(antlr4::tree::ErrorNode * /*node*/) override;
    string getFormattedSource() {
        return source;
    }

  private:
    void writeCarriageReturn();
    void writeNode(tree::TerminalNode *terminalNode);
    CommonTokenStream *tokens;
    ParserRuleContext *currentContext;
    string source;
    int currentIndent;
    bool newLine;
    string previousToken;
};
#endif
