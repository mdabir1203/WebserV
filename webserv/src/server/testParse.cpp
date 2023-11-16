#include <cassert>
#include <exception>
#include <iostream>
#include <string>
#include <map>
#include <cctype>
#include <cstddef>
#include <cstring>


#include "HeaderField.hpp"

void testValidHeader() {
    HeaderFieldStateMachine parser;
    std::string header = "Content-Type: text/html\r\n";

    parser.parseOneHeaderLine(header);

    assert(parser.getParsedHeaders().at("content-type") == "text/html");
    std::cout << "Test Valid Header: Passed\n";
}

void testInvalidHeaderName() {
    bool caught = false;
    HeaderFieldStateMachine parser;
    std::string header = "Content@Type: text/html\r\n";
    
    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Invalid Header Name: Passed\n";
}

void testLeadingAndTrailingWhitespace() {
    HeaderFieldStateMachine parser;
    std::string header = "Host:    example.com    \r\n";
    
    parser.parseOneHeaderLine(header);

    assert(parser.getParsedHeaders().at("host") == "example.com");
    std::cout << "Test Leading and Trailing Whitespace: Passed\n";
}


void testHeadersWithMultipleValues() {
    HeaderFieldStateMachine parser;
    std::string headers1 = "Set-Cookie: id=a3fWa; Expires=Wed, 21 Oct 2015 07:28:00 GMT\r\n";

    std::string headers2= "Set-Cookie: id=12345; Expires=Thu, 22 Oct 2015 07:28:00 GMT\r\n";
    
    parser.parseOneHeaderLine(headers1);

    assert(parser.getParsedHeaders().at("set-cookie") == "id=a3fWa; Expires=Wed, 21 Oct 2015 07:28:00 GMT");
    std::cout << "Test Headers with Multiple Values: Passed\n";

    parser.setCurrentState(HEADER_NAME);
    parser.setInputPosition(0);

    bool caught = false;
    if (parser.parseOneHeaderLine(headers2) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Duplicate Header Name: Passed\n";

    // parser.parseOneHeaderLine(headers2);
    // std::cout << "map-Vlaue:" << parser.getParsedHeaders().at("set-cookie") << std::endl;

    // assert(parser.getParsedHeaders().at("set-cookie") == "id=a3fWa; Expires=Wed, 21 Oct 2015 07:28:00 GMT, id=12345; Expires=Thu, 22 Oct 2015 07:28:00 GMT");
    // std::cout << "Test Headers with Multiple Values: Passed\n";


}


void testBufferOverflowProtection() {
    HeaderFieldStateMachine parser;
    std::string header = "X-Custom-Header: " + std::string(9000, 'x') + "\r\n";
    bool caught = false;


    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Buffer Overflow Protection: Passed\n";
}


//implement logic to avoid empty header name
void testUnexpectedCharactersAfterHeaderLine() { 
    HeaderFieldStateMachine parser;
    std::string header = "Content-Type: text/html\r\nx\r\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Unexpected Characters after Header Line: Passed\n";
}


void testMultipleHeaderFieds() {
    HeaderFieldStateMachine parser;
    std::string header = "Content-Type: text/html\r\nContent-Length: 88\r\n";
    bool caught = false;

    parser.parseOneHeaderLine(header);


    assert(parser.getParsedHeaders().at("content-type") == "text/html");
    assert(parser.getParsedHeaders().at("content-length") == "88");
    std::cout << "Test Multiple Header Fields: Passed\n";
}

void testNoHeaderFields() {
    HeaderFieldStateMachine parser;
    std::string header = "\r\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(!caught);
    std::cout << "Test No Header Fields: Passed\n";

}

// Additional test cases would be similarly structured...
int main() {
    testValidHeader();
    testInvalidHeaderName();
    testLeadingAndTrailingWhitespace();
    testHeadersWithMultipleValues();
    testBufferOverflowProtection();
    testUnexpectedCharactersAfterHeaderLine();
    testMultipleHeaderFieds();
    testNoHeaderFields();

    // ... call other test functions
    
    std::cout << "All tests completed.\n";
    return 0;
}