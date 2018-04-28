#include <iostream>
#include "antlr4-runtime.h"
#include "BrightScriptLexer.h"
#include "BrightScriptParser.h"
#include "BaseErrorListener.h"
#include "parser/BrightScriptBaseListener.h"

using namespace std;
using namespace antlr4;

class BrightscriptEventGenerator : public BrightScriptBaseListener
{
  public:
    virtual void enterStartRule(BrightScriptParser::StartRuleContext * /*ctx*/) override
    {
    }

    virtual void enterFunctionDeclaration(BrightScriptParser::FunctionDeclarationContext * /*ctx*/) override
    {
    }

  private:
    vector<string> functionNames = {};

    bool isFunction(ParserRuleContext *context)
    {
        return isSub(context) ||
               context->getRuleIndex == BrightScriptParser::RuleAnonymousFunctionDeclaration ||
               context->getRuleIndex == BrightScriptParser::RuleFunctionDeclaration;
    }

    bool isSub(ParserRuleContext *context)
    {
        return context->getRuleIndex == BrightScriptParser::RuleAnonymousSubDeclaration ||
               context->getRuleIndex == BrightScriptParser::RuleSubDeclaration;
    }

    bool functionNameExists(string nameToCheck)
    {
        for (auto name : functionNames)
        {
            if (name == nameToCheck)
            {
                return true;
            }
        }
        return false;
    }

    void checkDeclaration(BrightScriptParser::UntypedIdentifierContext *context)
    {
        auto name = context->getText;
        if (functionNameExists(name))
        {
            // notify error listener
        }
        else
        {
            functionNames.push_back(name);
        }
    }
};