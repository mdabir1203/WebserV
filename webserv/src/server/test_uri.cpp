#include <cassert>
#include "inc\RequestParser.hpp" // Include your class header file

void testValidURIs() {
    HeaderFieldStateMachine parser;

    // Test with valid URIs
    assert(parser.parseURI("http://example.com") == "http://example.com");
    assert(parser.parseURI("http://example.com/path") == "http://example.com/path");
    assert(parser.parseURI("http://example.com/path?query=value") == "http://example.com/path?query=value");
}

void testInvalidURIs() {
    HeaderFieldStateMachine parser;

    // Test with invalid URIs
    try {
        parser.parseURI("http:/example.com");
        assert(false); // Should not reach this line
    } catch (const std::runtime_error&) {
        assert(true); // Expected to catch an exception
    }

    try {
        parser.parseURI("http://example.com/path?query");
        assert(false); // Should not reach this line
    } catch (const std::runtime_error&) {
        assert(true); // Expected to catch an exception
    }
}

int main() {
    testValidURIs();
    testInvalidURIs();
    return 0;
}