
#include <array>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

class TermuxAPI {
public:
  TermuxAPI() {}

  void flashlightOn() { std::system("termux-torch on"); }

  void flashlightOff() { std::system("termux-torch off"); }
  std::string fingerprint() {
    std::string result = executeShellCommand("termux-fingerprint");
    return result;
  }

  std::string termuxElfCleaner(std::string filepath, int apiLevel = 0,
                               int jobs = 1) {
    const char *character_filepath = filepath.c_str();
    char buffer[100];
    snprintf(buffer, sizeof(buffer),
             "termux-eli-cleaner --api-level %d --jobs %d --filepath %s",
             apiLevel, jobs, character_filepath);
    std::string command = buffer;

    std::string result = executeShellCommand(command.c_str());
    return result;
  };

  std::string getBatteryStatus() {
    std::string result = executeShellCommand("termux-battery-status");
    return result;
  };
  void setBrightness(int brightess = 1) {

    std::string command = "termux-brightness";
    command = handleArguments(command, brightess, "", false);

    executeShellCommand("termux-brightness");
  };

private:
  // template<typename... Args>
  std::string executeShellCommand(const char *command) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command, "r"), pclose);
    if (!pipe) {
      throw std::runtime_error("Could not execute command");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) !=
           nullptr) {
      result += buffer.data();
    }
    return result;
  }

  template <typename T>
  std::string handleArguments(const std::string &command, T value,
                              const std::string &name, bool dashed = true) {
    std::string result = command;
    result += " ";
    if (dashed) {
      result += " --";
      result += name;
      result += " ";
    } else {
      result += name;
      result += " ";
    }
    if constexpr (std::is_same_v<T, const char *>)
      result += value;
    else if constexpr (std::is_same_v<T, std::string>)
      result += value;
    else
      result += std::to_string(value);
    return result;
  }
};
