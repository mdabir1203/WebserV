#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <cstddef>


#include "HeaderField.hpp"

void testValidHeader() {
    HeaderFieldStateMachine parser;
    std::string header = "Content-Type: text/html\r\n";
    
    for (size_t i = 0; i < header.length(); ++i) {
        parser.parseChar(header[i]);
    }

    assert(parser.getParsedHeaders().at("content-type") == "text/html");
    std::cout << "Test Valid Header: Passed\n";
}

void testInvalidHeaderName() {
    bool caught = false;
    HeaderFieldStateMachine parser;
    std::string header = "Content@Type: text/html\r\n";
    
    try {
        for (size_t i = 0; i < header.length(); ++i) {
            parser.parseChar(header[i]);
        }
    } catch (const std::runtime_error& e) {
        caught = true;
    }

    assert(caught);
    std::cout << "Test Invalid Header Name: Passed\n";
}

void testLeadingAndTrailingWhitespace() {
    HeaderFieldStateMachine parser;
    std::string header = "Host:    example.com    \r\n";
    
    for (size_t i = 0; i < header.length(); ++i) {
        parser.parseChar(header[i]);
    }

    assert(parser.getParsedHeaders().at("host") == "example.com");
    std::cout << "Test Leading and Trailing Whitespace: Passed\n";
}


void testHeadersWithMultipleValues() {
    HeaderFieldStateMachine parser;
    std::string headers1 = "Set-Cookie: id=a3fWa; Expires=Wed, 21 Oct 2015 07:28:00 GMT\r\n";

    std::string headers2= "Set-Cookie: id=12345; Expires=Thu, 22 Oct 2015 07:28:00 GMT\r\n";
    
    for (size_t i = 0; i < headers1.length(); ++i) {
        std::cout << headers1[i] << std::flush ;
        parser.parseChar(headers1[i]);
        
    }

    assert(parser.getParsedHeaders().at("set-cookie") == "id=a3fWa; Expires=Wed, 21 Oct 2015 07:28:00 GMT");
    std::cout << "Test Headers with Multiple Values: Passed\n";

    parser.currentState = HEADER_NAME;
    for (size_t i = 0; i < headers2.length(); ++i) {
    std::cout << headers2[i] << std::flush ;
    parser.parseChar(headers2[i]);
    
}

    std::cout << "map-Vlaue:" << parser.headers.at("set-cookie") << std::endl;

    assert(parser.getParsedHeaders().at("set-cookie") == "id=a3fWa; Expires=Wed, 21 Oct 2015 07:28:00 GMT, id=12345; Expires=Thu, 22 Oct 2015 07:28:00 GMT");
    std::cout << "Test Headers with Multiple Values: Passed\n";


}


// void testBufferOverflowProtection() {
//     HeaderFieldStateMachine parser;
//     std::string header = "X-Custom-Header: " + std::string(9000, 'x') + "\r\n";
//     bool caught = false;

//     try {
//         for (size_t i = 0; i < header.length(); ++i) {
//             parser.parseChar(header[i]);
//         }
//     } catch (const std::length_error& e) {
//         caught = true;
//     }

//     assert(caught);
//     std::cout << "Test Buffer Overflow Protection: Passed\n";
// }


void testUnexpectedCharactersAfterHeaderLine() {
    HeaderFieldStateMachine parser;
    std::string header = "Content-Type: text/html\r\n\r\n";
    bool caught = false;

    try {
        for (size_t i = 0; i < header.length(); ++i) {
            parser.parseChar(header[i]);
            if (i > strlen("Content-Type: text/html\r\n")) {
                // Simulate unexpected characters after the header line
                parser.parseChar('x');
            }
        }
    } catch (const std::runtime_error& e) {
        caught = true;
    }

    assert(caught);
    std::cout << "Test Unexpected Characters after Header Line: Passed\n";
}








// Additional test cases would be similarly structured...

int main() {
    // testValidHeader();
    // testInvalidHeaderName();
    // testLeadingAndTrailingWhitespace();
    testHeadersWithMultipleValues();
    // testBufferOverflowProtection();
    // testUnexpectedCharactersAfterHeaderLine();

    // ... call other test functions
    
    std::cout << "All tests completed.\n";
    return 0;
}