#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Server.hpp"

class HttpResponse {
public:
    HttpResponse();
    ~HttpResponse();

    int GetStatusCode() const;
    void SetStatusCode(int statusCode);
    const std::string& GetContentType() const;
    void SetContentType(const std::string& contentType);
    const std::string& GetContent() const;
    void SetContent(const std::string& content);

private:
  int statusCode_;
  std::string contentType_;
  std::string content_;

  // ...
};

// enum class State {
//   Start,
//   ReadingRequest,
//   ParsingRequest,
//   GeneratingResponse,
//   SendingResponse,
//   Done
// };

#endif