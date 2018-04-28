#include "BrightscriptEventListener.h"

using namespace std;
using namespace antlr4;

static inline char string_toupper_functional(char c)
{
    return std::toupper(c);
}

static inline std::string string_upper(const std::string &str)
{
    //TODO - replace strcopy
    std::string strcopy(str.size(), 0);
    std::transform(str.begin(), str.end(), strcopy.begin(), string_toupper_functional);
    return strcopy;
}

BrightscriptEventListener::BrightscriptEventListener(BrightScriptParser *_parser) : parser(_parser) {}

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
    auto name = string_upper(context->getText());
    if (functionNameExists(name))
    {
        parser->notifyErrorListeners(context->start, "function \"" + name + "\" is declared multiple times", nullptr);
    }
    else
    {
        functionNames.push_back(name);
    }
}
