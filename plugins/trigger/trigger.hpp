#pragma once

#include <exception>
#include <memory>
#include <sstream>
#include <string>
#include <variant>

namespace ichise::plugin::trigger {
  enum class E_TOKEN : int {
    ARROW = '>',
    ESCAPE = '\\',
    INPUT = '.',
    LB = '[',
    LC = '{',
    LP = '(',
    PI = '|',
    RB = ']',
    RC = '}',
    RP = ')',
  };

  enum class E_TRIGGER_MODE : int {
    ALWAYS,
    NORMAL,
    REPEAT,
  };

  enum class E_TRIGGER_SECTION : int {
    BINDING,
    MODE,
    PAYLOAD,
    SIGNAL,
  };

  enum class E_TRIGGER_TYPE : int {
    COMMAND,
    SIGNAL,
  };

  class InvalidSyntaxException : public std::exception {
  private:
    std::string message;

  public:
    InvalidSyntaxException(const std::string& expected, const std::string& received);
    InvalidSyntaxException(const E_TOKEN& expected, const E_TOKEN& received);

    virtual const char* what() const noexcept;
  };

  class LexicalException : public std::exception {
  private:
    std::string message;

  public:
    LexicalException(const std::string& message);

    virtual const char* what() const noexcept;
  };

  class CommandTrigger {
  public:
    std::string binding;
    E_TRIGGER_MODE mode;
    std::string payload;
  };

  class SignalTrigger : public CommandTrigger {
  public:
    std::string signal;
  };

  class Lexer {
  private:
    std::ostringstream buffer;
    E_TOKEN current_token;
    std::string current_token_text;
    bool escape;
    std::unique_ptr<std::stringstream>& input;

    E_TOKEN get_token();
    std::string get_token_text();

  public:
    Lexer(std::unique_ptr<std::stringstream>& input);

    void advance();
    E_TOKEN get_current_token() const;
    std::string get_current_token_text() const;
  };

  class Parser {
  private:
    std::unique_ptr<Lexer> lexer;
    std::variant<std::shared_ptr<CommandTrigger>, std::shared_ptr<SignalTrigger>> result;

    void arrow();
    void binding();
    void command_trigger();
    void extract_section(const E_TOKEN& start_token, const E_TOKEN& end_token,
                         const E_TRIGGER_SECTION& section);
    void input(const E_TRIGGER_SECTION& section);
    void mode();
    void payload();
    void signal();
    void signal_trigger();
    void trigger();

  public:
    std::variant<std::shared_ptr<CommandTrigger>, std::shared_ptr<SignalTrigger>>
    operator()(std::unique_ptr<std::stringstream>& input, E_TRIGGER_TYPE type);
  };
} // namespace ichise::plugin::trigger

