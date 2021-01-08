#include "trigger.hpp"

namespace ichise::plugin::trigger {
  InvalidSyntaxException::InvalidSyntaxException(const std::string& expected,
                                                 const std::string& received)
      : message("Expected `" + expected + "` but received `" + received + "`.") {}
  InvalidSyntaxException::InvalidSyntaxException(const E_TOKEN& expected, const E_TOKEN& received)
      : message(std::string("Expected `") +
                static_cast<std::underlying_type<E_TOKEN>::type>(expected) + "` but received `" +
                static_cast<std::underlying_type<E_TOKEN>::type>(received) + "`.") {}

  const char* InvalidSyntaxException::what() const noexcept { return message.c_str(); }

  Lexer::Lexer(std::unique_ptr<std::stringstream>& input) : escape(false), input(input) {
    advance();
  }

  void Lexer::advance() {
    if (input->peek() != EOF) {
      current_token = get_token();
      current_token_text = get_token_text();
    }
  }

  E_TOKEN Lexer::get_current_token() const { return current_token; }

  std::string Lexer::get_current_token_text() const { return current_token_text; }

  E_TOKEN Lexer::get_token() {
    buffer.str("");
    buffer.clear();

    char input_char;
    bool is_input = false;

    while ((*input) >> input_char) {
      if (escape) {
        buffer << input_char;
        escape = false;
      } else {
        switch (input_char) {
        case '-':
          char lookahead;
          (*input) >> lookahead;
          if (lookahead == '>') {
            if (is_input) {
              input->putback(lookahead);
              input->putback(input_char);
              return E_TOKEN::INPUT;
            } else {
              return E_TOKEN::ARROW;
            }
          } else {
            buffer << input_char;
            input->putback(lookahead);

            if (!is_input) {
              is_input = true;
            }
          }
          break;
        case '[':
        case '{':
        case '(':
        case '|':
        case ']':
        case '}':
        case ')':
          if (is_input) {
            input->putback(input_char);
            return E_TOKEN::INPUT;
          } else {
            return E_TOKEN(input_char);
          }
          break;
        case '\\':
          escape = true;
          break;
        default:
          buffer << input_char;
          if (!is_input) {
            is_input = true;
          }
          break;
        }
      }
    }

    throw LexicalException("Undefined Token.");
  }

  std::string Lexer::get_token_text() { return buffer.str(); }

  LexicalException::LexicalException(const std::string& message) : message(message) {}

  const char* LexicalException::what() const noexcept { return message.c_str(); }

  std::shared_ptr<const Trigger> Parser::operator()(std::unique_ptr<std::stringstream>& input) {
    lexer = std::make_unique<Lexer>(input);
    result = std::make_unique<Trigger>();

    trigger();

    return std::move(result);
  }

  void Parser::arrow() {
    if (lexer->get_current_token() != E_TOKEN::ARROW) {
      throw InvalidSyntaxException(E_TOKEN::ARROW, lexer->get_current_token());
    }

    lexer->advance();
  }

  void Parser::binding() {
    extract_section(E_TOKEN::LP, E_TOKEN::RP, E_TRIGGER_SECTION::BINDING);
    arrow();
  }

  void Parser::command() {
    payload();
    mode();
  }

  void Parser::extract_section(const E_TOKEN& start_token, const E_TOKEN& end_token,
                               const E_TRIGGER_SECTION& section) {
    if (lexer->get_current_token() != start_token) {
      throw InvalidSyntaxException(start_token, lexer->get_current_token());
    }

    lexer->advance();
    input(section);

    if (lexer->get_current_token() != end_token) {
      throw InvalidSyntaxException(end_token, lexer->get_current_token());
    }

    lexer->advance();
  }

  void Parser::input(const E_TRIGGER_SECTION& section) {
    if (lexer->get_current_token() != E_TOKEN::INPUT) {
      throw InvalidSyntaxException(E_TOKEN::INPUT, lexer->get_current_token());
    }

    switch (section) {
    case E_TRIGGER_SECTION::BINDING:
      result->binding = lexer->get_current_token_text();
      break;
    case E_TRIGGER_SECTION::MODE:
      if (lexer->get_current_token_text() == "ALWAYS") {
        result->mode = E_TRIGGER_MODE::ALWAYS;
      } else if (lexer->get_current_token_text() == "NORMAL") {
        result->mode = E_TRIGGER_MODE::NORMAL;
      } else if (lexer->get_current_token_text() == "REPEAT") {
        result->mode = E_TRIGGER_MODE::REPEAT;
      } else {
        throw InvalidSyntaxException("ALWAYS` or `NORMAL` or `REPEAT",
                                     lexer->get_current_token_text());
      }
      break;
    case E_TRIGGER_SECTION::PAYLOAD:
      result->payload = lexer->get_current_token_text();
      break;
    case E_TRIGGER_SECTION::SIGNAL:
      result->signal = lexer->get_current_token_text();
      break;
    }

    lexer->advance();
  }

  void Parser::mode() {
    if (lexer->get_current_token() == E_TOKEN::ARROW) {
      arrow();
      extract_section(E_TOKEN::LB, E_TOKEN::RB, E_TRIGGER_SECTION::MODE);
    } else {
      result->mode = E_TRIGGER_MODE::NORMAL;
    }
  }

  void Parser::payload() { extract_section(E_TOKEN::LC, E_TOKEN::RC, E_TRIGGER_SECTION::PAYLOAD); }

  void Parser::signal() {
    if (lexer->get_current_token() == E_TOKEN::PI) {
      extract_section(E_TOKEN::PI, E_TOKEN::PI, E_TRIGGER_SECTION::SIGNAL);
      arrow();
    }
  }

  void Parser::signal_or_command() {
    signal();
    command();
  }

  void Parser::trigger() {
    binding();
    signal_or_command();
  }
} // namespace ichise::plugin::trigger

