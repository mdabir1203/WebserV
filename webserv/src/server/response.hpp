class HttpResponseSerializer {
public:
  static int serialize(const HttpResponse& response, char* buffer, int bufferSize) {
    // Serialize the status code
    int bytesWritten = snprintf(buffer, bufferSize, "HTTP/1.1 %d %s\r\n", response.getStatusCode(), response.getReasonPhrase().c_str());
    buffer += bytesWritten;

    // Serialize the content type
    bytesWritten = snprintf(buffer, bufferSize, "Content-Type: %s\r\n", response.getContentType().c_str());
    buffer += bytesWritten;

    // Serialize the content length
    bytesWritten = snprintf(buffer, bufferSize, "Content-Length: %d\r\n\r\n", response.getContentLength());
    buffer += bytesWritten;

    // Serialize the content
    memcpy(buffer, response.getContent().data(), response.getContentLength());

    return bytesWritten + response.getContentLength();
  }
};