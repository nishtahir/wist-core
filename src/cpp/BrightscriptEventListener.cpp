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

bool isSub(ParserRuleContext *context)
{
    return context->getRuleIndex() == BrightScriptParser::RuleAnonymousSubDeclaration ||
           context->getRuleIndex() == BrightScriptParser::RuleSubDeclaration;
}

bool isFunction(ParserRuleContext *context)
{
    return isSub(context) ||
           context->getRuleIndex() == BrightScriptParser::RuleAnonymousFunctionDeclaration ||
           context->getRuleIndex() == BrightScriptParser::RuleFunctionDeclaration;
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

ParserRuleContext *getParentFunctionContext(ParserRuleContext *ctx)
{
    if (ParserRuleContext *parent = dynamic_cast<ParserRuleContext *>(ctx->parent))
    {
        return isFunction(parent) ? parent : getParentFunctionContext(parent);
    }
    return nullptr;
}

void BrightscriptEventListener::exitReturnStatement(BrightScriptParser::ReturnStatementContext *ctx)
{
    auto functionParent = getParentFunctionContext(ctx);
    if (functionParent == nullptr)
    {
        return;
    }

    // if we can't figure out the type, it's
    // probably dynamic
    // TODO - Replace string constants with enums for performance
    string type = "DYNAMIC";
    if (isSub(functionParent))
    {
        type = "VOID";
    }
    else
    {
        for (auto child : functionParent->children)
        {
            if (BrightScriptParser::BaseTypeContext *childCtx = dynamic_cast<BrightScriptParser::BaseTypeContext *>(child))
            {
                type = string_upper(childCtx->getText());
            }
        }
    }

    bool hasExpression = ctx->children.size() > 1;
    if (type == "VOID" && hasExpression)
    {
        parser->notifyErrorListeners(ctx->start, "Unexpected return value for function with return type " + type, nullptr);
    }
    else if (type != "VOID" && !hasExpression)
    {
        parser->notifyErrorListeners(ctx->start, "Expected return value for function with return type " + type, nullptr);
    }
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
