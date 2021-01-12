#include "trigger.hpp"

namespace ichise::plugin::trigger {
  InvalidSyntaxException::InvalidSyntaxException(const std::string& expected,
                                                 const std::string& received)
      : message("Expected `" + expected + "` but received `" + received + "`.") {}
  InvalidSyntaxException::InvalidSyntaxException(const E_TOKEN& expected, const E_TOKEN& received) {
    this->message = "Expected `";

    switch (expected) {
    case E_TOKEN::ARROW:
      this->message += "ARROW OPERATOR";
      break;
    case E_TOKEN::INPUT:
      this->message += "INPUT";
      break;
    default:
      this->message += static_cast<std::underlying_type<E_TOKEN>::type>(expected);
      break;
    }

    this->message += "` but received `";

    switch (received) {
    case E_TOKEN::ARROW:
      this->message += "ARROW OPERATOR";
      break;
    case E_TOKEN::INPUT:
      this->message += "INPUT";
      break;
    default:
      this->message += static_cast<std::underlying_type<E_TOKEN>::type>(received);
      break;
    }

    this->message += "`.";
  }

  const char* InvalidSyntaxException::what() const noexcept { return message.c_str(); }

  LexicalException::LexicalException(const std::string& message) : message(message) {}

  const char* LexicalException::what() const noexcept { return message.c_str(); }

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

    while ((*input) >> std::noskipws >> input_char) {
      if (escape) {
        buffer << std::noskipws << input_char;
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

  std::variant<std::shared_ptr<CommandTrigger>, std::shared_ptr<SignalTrigger>> Parser::
  operator()(std::unique_ptr<std::stringstream>& input, E_TRIGGER_TYPE type) {
    lexer = std::make_unique<Lexer>(input);

    switch (type) {
    case E_TRIGGER_TYPE::COMMAND:
      result = std::make_shared<CommandTrigger>();
      break;
    case E_TRIGGER_TYPE::SIGNAL:
      result = std::make_shared<SignalTrigger>();
      break;
    }

    trigger();

    return result;
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

  void Parser::command_trigger() {
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
      std::visit([&](auto v) { v->binding = lexer->get_current_token_text(); }, result);
      break;
    case E_TRIGGER_SECTION::MODE:
      std::visit(
        [&](auto v) {
          if (lexer->get_current_token_text() == "ALWAYS") {
            v->mode = E_TRIGGER_MODE::ALWAYS;
          } else if (lexer->get_current_token_text() == "NORMAL") {
            v->mode = E_TRIGGER_MODE::NORMAL;
          } else if (lexer->get_current_token_text() == "REPEAT") {
            v->mode = E_TRIGGER_MODE::REPEAT;
          } else {
            throw InvalidSyntaxException("ALWAYS` or `NORMAL` or `REPEAT", lexer->get_current_token_text());
          }
        },
        result);
      break;
    case E_TRIGGER_SECTION::PAYLOAD:
      if (result.index() == 0) {
        std::get<0>(result)->payload = lexer->get_current_token_text();
      } else {
        std::get<1>(result)->payload = "{" + lexer->get_current_token_text() + "}";
      }
      break;
    case E_TRIGGER_SECTION::SIGNAL:
      if (result.index() == 1) {
        std::get<1>(result)->signal = lexer->get_current_token_text();
      }
      break;
    }

    lexer->advance();
  }

  void Parser::mode() {
    if (lexer->get_current_token() == E_TOKEN::ARROW) {
      lexer->advance();
      extract_section(E_TOKEN::LB, E_TOKEN::RB, E_TRIGGER_SECTION::MODE);
    } else {
      std::visit([&](auto v) { v->mode = E_TRIGGER_MODE::NORMAL; }, result);
    }
  }

  void Parser::payload() { extract_section(E_TOKEN::LC, E_TOKEN::RC, E_TRIGGER_SECTION::PAYLOAD); }

  void Parser::signal() {
    extract_section(E_TOKEN::PI, E_TOKEN::PI, E_TRIGGER_SECTION::SIGNAL);
    arrow();
  }

  void Parser::signal_trigger() {
    signal();
    command_trigger();
  }

  void Parser::trigger() {
    binding();

    if (result.index()) {
      signal_trigger();
    } else {
      command_trigger();
    }
  }
} // namespace ichise::plugin::trigger

