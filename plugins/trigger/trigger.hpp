#pragma once

#include <exception>
#include <memory>
#include <optional>
#include <sstream>
#include <string>

namespace ichise::plugin::trigger {
  enum class E_TOKEN : char {
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

  class Trigger {
  public:
    std::string binding;
    E_TRIGGER_MODE mode;
    std::string payload;
    std::optional<std::string> signal;
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
    std::unique_ptr<Trigger> result;

    void arrow();
    void binding();
    void command();
    void extract_section(const E_TOKEN& start_token, const E_TOKEN& end_token,
                         const E_TRIGGER_SECTION& section);
    void input(const E_TRIGGER_SECTION& section);
    void mode();
    void payload();
    void signal();
    void signal_or_command();
    void trigger();

  public:
    std::shared_ptr<const Trigger> operator()(std::unique_ptr<std::stringstream>& input);
  };
} // namespace ichise::plugin::trigger

