// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ParserHelper.h"

namespace spice::compiler {

CodeLoc ParserHelper::getCodeLoc(const antlr4::ParserRuleContext *ctx) {
  return CodeLoc(ctx->start, ctx->start->getInputStream()->getSourceName());
}

void ParserHelper::saveErrorMessage(ASTNode *node, const antlr4::ParserRuleContext *ctx) {
  const antlr4::misc::Interval sourceInterval(ctx->start->getStartIndex(), ctx->stop->getStopIndex());
  antlr4::misc::Interval extendedSourceInterval(sourceInterval);

  auto inputStream = static_cast<antlr4::ANTLRInputStream *>(ctx->start->getInputStream());
  if (inputStream->getText(extendedSourceInterval).find('\n') != std::string::npos) {
    extendedSourceInterval.b = extendedSourceInterval.a;
    while (inputStream->getText(extendedSourceInterval).find('\n') == std::string::npos)
      extendedSourceInterval.b++;
  }

  size_t indentation = 0;
  for (; indentation < ERROR_MESSAGE_CONTEXT; indentation++) {
    extendedSourceInterval.a--;
    if (extendedSourceInterval.a < 0 || inputStream->getText(extendedSourceInterval).find('\n') != std::string::npos) {
      extendedSourceInterval.a++;
      break;
    }
  }
  for (size_t suffixContext = 0; suffixContext < ERROR_MESSAGE_CONTEXT; suffixContext++) {
    extendedSourceInterval.b++;
    if (extendedSourceInterval.b > inputStream->size() ||
        inputStream->getText(extendedSourceInterval).find('\n') != std::string::npos) {
      extendedSourceInterval.b--;
      break;
    }
  }

  // Trim start
  while (inputStream->getText(extendedSourceInterval)[0] == ' ') {
    extendedSourceInterval.a++;
    indentation--;
  }

  // Trim end
  if (inputStream->getText(extendedSourceInterval)[extendedSourceInterval.length() - 1] == '\n')
    extendedSourceInterval.b--;

  const std::string lineNumberStr = std::to_string(ctx->start->getLine());
  indentation += lineNumberStr.length() + 2;

  std::stringstream ss;
  ss << lineNumberStr << "  " << inputStream->getText(extendedSourceInterval) << "\n";
  for (size_t i = 0; i < indentation; i++)
    ss << " ";
  for (size_t i = 0; i < std::min(sourceInterval.length(), extendedSourceInterval.length()); i++)
    ss << "^";
  node->errorMessage = ss.str();
}

} // namespace spice::compiler