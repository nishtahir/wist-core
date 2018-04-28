#include "BrightscriptEventListener.h"

using namespace std;
using namespace antlr4;

void BrightscriptEventListener::enterFunctionDeclaration(BrightScriptParser::FunctionDeclarationContext *ctx)
{
    checkDeclaration(ctx->untypedIdentifier());
}

void BrightscriptEventListener::enterSubDeclaration(BrightScriptParser::SubDeclarationContext *ctx)
{
    checkDeclaration(ctx->untypedIdentifier());
}

bool BrightscriptEventListener::isFunction(ParserRuleContext *context)
{
    return isSub(context) ||
           context->getRuleIndex() == BrightScriptParser::RuleAnonymousFunctionDeclaration ||
           context->getRuleIndex() == BrightScriptParser::RuleFunctionDeclaration;
}

bool BrightscriptEventListener::isSub(ParserRuleContext *context)
{
    return context->getRuleIndex() == BrightScriptParser::RuleAnonymousSubDeclaration ||
           context->getRuleIndex() == BrightScriptParser::RuleSubDeclaration;
}

bool BrightscriptEventListener::functionNameExists(string nameToCheck)
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

void BrightscriptEventListener::checkDeclaration(BrightScriptParser::UntypedIdentifierContext *context)
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