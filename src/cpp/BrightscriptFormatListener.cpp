#include "BrightscriptFormatListener.h"
#include "trim.h"

using namespace std;
using namespace antlr4;

string buildIndent(int indent)
{
  string stringIndent = "";
  for (int i = 0; i < indent * 4; i++)
  {
    stringIndent.append(" ");
  }
  return stringIndent;
}

void BrightscriptFormatListener::writeCarriageReturn()
{
  if (previousToken != "\n")
  {
    source.append("\n");
    previousToken = "\n";
  }
}

string noSpaceBeforeTokens[] = {".", ",", "(", ")"};
bool nextTokenShouldHaveSpaceBefore(string token)
{
  for (string character : noSpaceBeforeTokens)
  {
    if (token == character)
    {
      return false;
    }
  }
  return true;
}

string noSpaceAfterTokens[] = {".", "}", "(", ":"};
bool previousTokenShouldHaveSpace(string token)
{
  for (string character : noSpaceAfterTokens)
  {
    if (token == character)
    {
      return false;
    }
  }
  return true;
}

void BrightscriptFormatListener::writeNode(tree::TerminalNode *terminalNode)
{
  string text = terminalNode->getText();
  string node = trim(text);
  if (node == "")
  {
    return;
  }

  if (newLine == false && previousTokenShouldHaveSpace(previousToken) && nextTokenShouldHaveSpaceBefore(node))
  {
    source.append(" ");
  }

  if (currentIndent > 0 && newLine == true)
  {
    source.append(buildIndent(currentIndent));
  }

  source.append(trim(node));
  if (newLine == true)
  {
    newLine = false;
  }
  previousToken = node;
}

BrightscriptFormatListener::BrightscriptFormatListener(CommonTokenStream *_tokens)
{
  tokens = _tokens;
  source = "";
  currentIndent = 0;
  newLine = true;
}

void BrightscriptFormatListener::enterEveryRule(ParserRuleContext *ctx)
{
  if (ctx->getRuleIndex() == BrightScriptParser::RuleEndOfLine)
  {
    writeCarriageReturn();
    newLine = true;
  }
  else if (ctx->getRuleIndex() == BrightScriptParser::RuleBlock)
  {
    newLine = true;
    currentIndent++;
  }
  currentContext = ctx;
}

void BrightscriptFormatListener::exitEveryRule(ParserRuleContext *ctx)
{
  if (ctx->getRuleIndex() == BrightScriptParser::RuleEndOfLine)
  {
    newLine = true;
    writeCarriageReturn();
  }
  else if (ctx->getRuleIndex() == BrightScriptParser::RuleBlock)
  {
    if (currentIndent > 0)
    {
      currentIndent--;
    }
  }
}

void BrightscriptFormatListener::visitTerminal(tree::TerminalNode *node)
{
  writeNode(node);
}

void BrightscriptFormatListener::visitErrorNode(tree::ErrorNode *node)
{
}