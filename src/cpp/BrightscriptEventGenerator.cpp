#include "BrightscriptEventGenerator.h"

using namespace std;
using namespace antlr4;

class BrightscriptEventGenerator : public BrightScriptBaseListener
{
  public:
    virtual void enterFunctionDeclaration(BrightScriptParser::FunctionDeclarationContext *ctx) override
    {
        checkDeclaration(ctx->untypedIdentifier());
    }

    virtual void enterSubDeclaration(BrightScriptParser::SubDeclarationContext *ctx) override
    {
        checkDeclaration(ctx->untypedIdentifier());
    }

  private:
    vector<string> functionNames = {};

    bool isFunction(ParserRuleContext *context)
    {
        return isSub(context) ||
               context->getRuleIndex() == BrightScriptParser::RuleAnonymousFunctionDeclaration ||
               context->getRuleIndex() == BrightScriptParser::RuleFunctionDeclaration;
    }

    bool isSub(ParserRuleContext *context)
    {
        return context->getRuleIndex() == BrightScriptParser::RuleAnonymousSubDeclaration ||
               context->getRuleIndex() == BrightScriptParser::RuleSubDeclaration;
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
        auto name = context->getText();
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