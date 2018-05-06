#include "BrightscriptFormatListener.h"

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
  source.append("\n");
  newLine = true;
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

string noSpaceAfterTokens[] = {".", "}", "("};
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
  string node = terminalNode->getText();
  
  if (previousToken == "\n")
  {
    newLine = true;
  }

  if (newLine == false && previousTokenShouldHaveSpace(previousToken) && nextTokenShouldHaveSpaceBefore(node))
  {
    source.append(" ");
  }

  if (currentIndent > 0 && newLine == true)
  {
    source.append(buildIndent(currentIndent));
  }
  source.append(node);
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
  if (ctx->getRuleIndex() == BrightScriptParser::RuleBlock)
  {
    currentIndent++;
    writeCarriageReturn();
  }
  currentContext = ctx;
}

void BrightscriptFormatListener::exitEveryRule(ParserRuleContext *ctx)
{
  if (ctx->getRuleIndex() == BrightScriptParser::RuleBlock)
  {
    if (currentIndent > 0)
    {
      currentIndent--;
    }
    writeCarriageReturn();
  }
}

void BrightscriptFormatListener::visitTerminal(tree::TerminalNode *node)
{
  writeNode(node);
}

void BrightscriptFormatListener::visitErrorNode(tree::ErrorNode *node)
{
}