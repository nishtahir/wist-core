#include "BrightscriptFormatVisitor.h"
#include "trim.h"
#include <emscripten/val.h>

using namespace std;
using namespace antlr4;
using namespace antlr4::tree;
using namespace antlrcpp;

string noSpaceBeforeTokens[] = {".", ",", "(", ")", ":", "[", "]"};
bool noSpaceBeforeTokensContainsString(string token)
{
  for (string character : noSpaceBeforeTokens)
  {
    if (character == token)
    {
      return true;
    }
  }
  return false;
}

string noSpaceAfterTokens[] = {".", "}", "(", "["};
bool noSpaceAfterTokensContainsString(string token)
{
  for (string character : noSpaceAfterTokens)
  {
    if (token == character)
    {
      return true;
    }
  }
  return false;
}

Any BrightscriptFormatVisitor::visitBlock(BrightScriptParser::BlockContext *ctx)
{
  currentIndent++;
  visitChildren(ctx);
  currentIndent--;
  return nullptr;
}

Any BrightscriptFormatVisitor::visitFunctionDeclaration(BrightScriptParser::FunctionDeclarationContext *ctx)
{
  setContext(ctx);
  return nullptr;
}

Any BrightscriptFormatVisitor::visitAssociativeArrayInitializer(BrightScriptParser::AssociativeArrayInitializerContext *ctx)
{
  for (int i = 0; i < ctx->children.size() - 1; i++)
  {
    auto current = ctx->children[i];
    auto next = ctx->children[i + 1];

    if (TerminalNode *node = dynamic_cast<TerminalNode *>(current))
    {
      if (node->getSymbol()->getType() == BrightScriptParser::OPEN_BRACE)
      {
        currentIndent++;
        writeNode(node);
        writeCarriageReturn();
      }
      else if (node->getSymbol()->getType() == BrightScriptParser::CLOSE_BRACE)
      {
        currentIndent--;
        writeNode(node);
      }
    }
    else if (BrightScriptParser::EndOfLineContext *endOfLineCtx = dynamic_cast<BrightScriptParser::EndOfLineContext *>(current))
    {
      visitChildren(endOfLineCtx);
    }
    else
    {
      visitChildren(current);
      // Write the next token if it's a comma
      if (TerminalNode *node = dynamic_cast<TerminalNode *>(next))
      {
        if (node->getSymbol()->getType() == BrightScriptParser::COMMA)
        {
          writeNode(node);
        }
      }
      writeCarriageReturn();
    }

    // Write next node if it's a closing brace
    if (TerminalNode *node = dynamic_cast<TerminalNode *>(next))
    {
      if (node->getSymbol()->getType() == BrightScriptParser::CLOSE_BRACE)
      {
        currentIndent--;
        writeNode(node);
      }
    }
  }
  return nullptr;
}

Any BrightscriptFormatVisitor::visitArrayInitializer(BrightScriptParser::ArrayInitializerContext *ctx)
{
  return nullptr;
}

Any BrightscriptFormatVisitor::visitEndOfLine(BrightScriptParser::EndOfLineContext *ctx)
{
  for (auto child : ctx->children)
  {
    if (TerminalNode *node = dynamic_cast<TerminalNode *>(child))
    {
      auto tokenPos = node->getSymbol()->getTokenIndex();
      if (tokenPos > hiddenTokenPosition)
      {
        auto ref = tokens->getHiddenTokensToLeft(tokenPos);
        hiddenTokenPosition = tokenPos;
        if (ref.size() > 0)
        {
          for (auto token : ref)
          {
            writeIndent();
            writeToken(token);
          }
        }
      }
      string text = node->getText();
      for (int i = 0; i < count(text.begin(), text.end(), '\n'); i++)
      {
        writeCarriageReturn();
      }
    }
  }
  return nullptr;
}

Any BrightscriptFormatVisitor::visitTerminal(TerminalNode *node)
{
  if (node->getSymbol()->getType() != BrightScriptParser::EOF)
  {
    writeNode(node);
  }
  return nullptr;
}

void BrightscriptFormatVisitor::writeCarriageReturn()
{
  source.append("\n");
  newLine = true;
}

void BrightscriptFormatVisitor::setContext(ParserRuleContext *ctx)
{
  currentContext = ctx;
  visitChildren(ctx);
  currentContext = ctx;
}

void BrightscriptFormatVisitor::writeIndent()
{
  for (int i = 0; i < currentIndent * indentSize; i++)
  {
    source.append(" ");
  }
}

void BrightscriptFormatVisitor::writeNode(tree::TerminalNode *terminalNode)
{
  string nodeString = terminalNode->getText();
  string text = trim(nodeString);
  if (text == "")
  {
    return;
  }

  if (!newLine &&
      !noSpaceAfterTokensContainsString(previousToken->getText()) &&
      !noSpaceBeforeTokensContainsString(text))
  {
    source.append(" ");
  }

  if (newLine)
  {
    writeIndent();
  }

  source.append(text);
  if (newLine == true)
  {
    newLine = false;
  }
  previousToken = terminalNode->getSymbol();
}

void BrightscriptFormatVisitor::writeToken(Token *token)
{
  string tokenString = token->getText();
  source.append(trim(tokenString));
  if (newLine == true)
  {
    newLine = false;
  }
  previousToken = token;
}