// Copyright (c) 2021-2023 ChilliBits. All rights reserved.

#include "ParserHelper.h"

#include <typechecker/OpRuleManager.h>
#include <exception/ParserError.h>

#include "SpiceParser.h"

namespace spice::compiler {

void ParserHelper::replaceEscapeChars(std::string &string) {
  CommonUtil::replaceAll(string, "\\a", "\a");
  CommonUtil::replaceAll(string, "\\b", "\b");
  CommonUtil::replaceAll(string, "\\f", "\f");
  CommonUtil::replaceAll(string, "\\n", "\n");
  CommonUtil::replaceAll(string, "\\r", "\r");
  CommonUtil::replaceAll(string, "\\t", "\t");
  CommonUtil::replaceAll(string, "\\v", "\v");
  CommonUtil::replaceAll(string, "\\'", "\'");
  CommonUtil::replaceAll(string, "\\?", "\?");
}

int32_t ParserHelper::parseInt(ConstantNode *constantNode, antlr4::tree::TerminalNode *terminal) {
  std::function<int32_t(const std::string &, int)> cb = [](const std::string &substr, int base) {
    return std::stoi(substr, nullptr, base);
  };
  return parseNumeric(constantNode, terminal, cb);
}
int16_t ParserHelper::parseShort(ConstantNode *constantNode, antlr4::tree::TerminalNode *terminal) {
  std::function<int16_t(const std::string &, int)> cb = [](const std::string &substr, int base) {
    return (int16_t)std::stoi(substr, nullptr, base);
  };
  return parseNumeric(constantNode, terminal, cb);
}

int64_t ParserHelper::parseLong(ConstantNode *constantNode, antlr4::tree::TerminalNode *terminal) {
  std::function<int64_t(const std::string &, int)> cb = [](const std::string &substr, int base) {
    return std::stoll(substr, nullptr, base);
  };
  return parseNumeric(constantNode, terminal, cb);
}

int8_t ParserHelper::parseChar(ConstantNode *constantNode, antlr4::tree::TerminalNode *terminal) {
  const std::string input = terminal->toString();
  if (input.length() == 3) { // Normal char literals
    return input[1];
  } else if (input.length() == 4 && input[1] == '\\') { // Char literals with escape sequence
    switch (input[2]) {
    case '\'':
      return '\'';
    case '"':
      return '\"';
    case '\\':
      return '\\';
    case 'n':
      return '\n';
    case 'r':
      return '\r';
    case 't':
      return '\t';
    case 'b':
      return '\b';
    case 'f':
      return '\f';
    case 'v':
      return '\v';
    case '0':
      return '\0';
    default:
      throw ParserError(constantNode->codeLoc, INVALID_CHAR_LITERAL, "Invalid escape sequence " + input);
    }
  } else {
    throw ParserError(constantNode->codeLoc, INVALID_CHAR_LITERAL, "Invalid char literal " + input);
  }
}

std::string ParserHelper::parseString(std::string input) {
  input = input.substr(1, input.size() - 2);
  replaceEscapeChars(input);
  return input;
}

template <typename T>
T ParserHelper::parseNumeric(ConstantNode *constantNode, antlr4::tree::TerminalNode *terminal,
                             std::function<T(const std::string &, int)> cb) {
  const std::string input = terminal->toString();

  // Set to signed if the input string ends with 'u'
  if (constantNode)
    constantNode->isSigned = input.ends_with('u');

  try {
    if (input.length() >= 3) {
      const char c1 = input[0];
      const char c2 = input[1];
      const std::string subStr = input.substr(2);
      if (c1 == '0') {
        switch (c2) {
        case 'd':
        case 'D':
          return cb(subStr, 10);
        case 'b':
        case 'B':
          return cb(subStr, 2);
        case 'h':
        case 'H':
        case 'x':
        case 'X':
          return cb(subStr, 16);
        case 'o':
        case 'O':
          return cb(subStr, 8);
        default:
          return cb(input, 10);
        }
      }
    }
    return cb(input, 10);
  } catch (std::out_of_range &e) {
    throw ParserError(constantNode->codeLoc, NUMBER_OUT_OF_RANGE, "The provided number is out of range");
  } catch (std::invalid_argument &e) {
    throw ParserError(constantNode->codeLoc, NUMBER_OUT_OF_RANGE,
                      "You tried to parse '" + input + "' as an integer, but it was no integer");
  }
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

std::string ParserHelper::getIdentifier(antlr4::tree::TerminalNode *terminal) {
  std::string identifier = terminal->getText();

  // Check if the list of reserved keywords contains the given identifier
  bool isReserved =
      std::find(std::begin(RESERVED_KEYWORDS), std::end(RESERVED_KEYWORDS), identifier) != std::end(RESERVED_KEYWORDS);
  // Print error message
  if (isReserved) {
    const CodeLoc codeLoc(terminal->getSymbol(), terminal->getSymbol()->getInputStream()->getSourceName());
    throw ParserError(codeLoc, RESERVED_KEYWORD, "'" + identifier + "' is a reserved keyword. Please use another name instead");
  }

  return identifier;
}

} // namespace spice::compiler