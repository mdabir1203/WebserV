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
    parser.setCurrentState(HEADER_NAME);
    std::string header = "Connection: text/html\r\n";

    parser.parseOneHeaderLine(header);

    assert(parser.getParsedHeaders().at("connection").front() == "text/html");
    std::cout << "Test Valid Header: Passed\n";
}

void testInvalidHeaderName() {
    bool caught = false;
    HeaderFieldStateMachine parser;
    parser.setCurrentState(HEADER_NAME);
    std::string header = "Content@Type: text/html\r\n";
    
    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Invalid Header Name: Passed\n";
}

void testLeadingAndTrailingWhitespace() {
    HeaderFieldStateMachine parser;
    parser.setCurrentState(HEADER_NAME);
    std::string header = "Host:    example.com    \r\n";
    
    parser.parseOneHeaderLine(header);

    assert(parser.getParsedHeaders().at("host").front() == "example.com");
    std::cout << "Test Leading and Trailing Whitespace: Passed\n";
}


void testHeadersWithMultipleValues() {
    HeaderFieldStateMachine parser;
    parser.setCurrentState(HEADER_NAME);
    std::string headers1 = "Cookie: id=a3fWa; Expires=Wed, 21 Oct 2015 07:28:00 GMT\r\n";

    std::string headers2= "Cookie: id=12345; Expires=Thu, 22 Oct 2015 07:28:00 GMT\r\n";
    
    parser.parseOneHeaderLine(headers1);

    assert(parser.getParsedHeaders().at("cookie")[0] == "id=a3fWa; Expires=Wed, 21 Oct 2015 07:28:00 GMT");


    parser.setCurrentState(HEADER_NAME);
    parser.setInputPosition(0);
    parser.parseOneHeaderLine(headers2);

    assert(parser.getParsedHeaders().at("cookie")[1] == "id=12345; Expires=Thu, 22 Oct 2015 07:28:00 GMT");
    std::cout << "Test Headers with Multiple Values: Passed\n";
}


void testBufferOverflowProtection_1() {
    HeaderFieldStateMachine parser;
    parser.setCurrentState(HEADER_NAME);
    std::string header = "X-Custom-Header: " + std::string(9000, 'x') + "\r\n";
    bool caught = false;


    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Buffer Overflow Protection_1: Passed\n";
}

void testBufferOverflowProtection_2() {
    HeaderFieldStateMachine parser;
    parser.setCurrentState(HEADER_NAME);
    std::string header = std::string(9000, 'x') + ": asdfsdf" + "\r\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Buffer Overflow Protection_2: Passed\n";
}

//implement logic to avoid empty header name
void testUnexpectedCharactersAfterHeaderLine() { 
    HeaderFieldStateMachine parser;
    parser.setCurrentState(HEADER_NAME);
    std::string header = "Content-Type: text/html\r\nx\r\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Unexpected Characters after Header Line: Passed\n";
}


void testMultipleHeaderFieds() {
    HeaderFieldStateMachine parser;
    parser.setCurrentState(HEADER_NAME);
    std::string header = "user-agent: text/html\r\nconnection: 88\r\n";
    bool caught = false;

    parser.parseOneHeaderLine(header);


    assert(parser.getParsedHeaders().at("user-agent").front() == "text/html");
    assert(parser.getParsedHeaders().at("connection").front() == "88");
    std::cout << "Test Multiple Header Fields: Passed\n";
}

void testNoHeaderFields() {
    HeaderFieldStateMachine parser;
    parser.setCurrentState(HEADER_NAME);
    std::string header = "\r\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(!caught);
    std::cout << "Test No Header Fields: Passed\n";
}


//implement a flag which states the end of the chunk
void testInvalidCRLF_1() { 
    HeaderFieldStateMachine parser;
    parser.setCurrentState(HEADER_NAME);
    std::string header = "Content-Type: text/html\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Invalid CRLF_1: Passed\n";

}

void testInvalidCRLF_2() {
    HeaderFieldStateMachine parser;
    parser.setCurrentState(HEADER_NAME);
    std::string header = "Content-Type: text/html\ra\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Invalid CRLF_2: Passed\n";

}


//Do wee need to end with CRLF? if there is no body
// void testInvalidCRLF_3() {
//     HeaderFieldStateMachine parser;
//     parser.setCurrentState(HEADER_NAME);
//     std::string header = "Content-Type: text/html";
//     bool caught = false;

//     if (parser.parseOneHeaderLine(header) != OK)
//         caught = true;

//     assert(caught);
//     std::cout << "Test Invalid CRLF_3: Passed";
// }

void testEmptyHeaderFields() {
    HeaderFieldStateMachine parser;
    parser.setCurrentState(HEADER_NAME);
    std::string header = "Content-Type: \r\nContent-Length: 88\r\n\r\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Empty Header Fields: Passed\n";
}

void testEmptyInput() {
    HeaderFieldStateMachine parser;
    parser.setCurrentState(HEADER_NAME);
    std::string header = "";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(!caught);
    std::cout << "Test Empty Input: Passed\n";
}

void testHeaderRequestLineParsing() {
    HeaderFieldStateMachine parser;
    std::string header = "GET / HTTP/1.1\r\n";
    bool caught = false;

    parser.parseOneHeaderLine(header);

    assert(parser.getHeaderMethod() == GET);
    assert(parser.getHeaderUri() == "/");
    assert(parser.getIsHttpVersionRight() == true);

    std::cout << "Test Header Request Line Parsing: Passed\n";
}

void testIncorrectMethod() {
    HeaderFieldStateMachine parser;
    std::string header = "/ HTTP/1.1\r\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Wrong Method: Passed\n";
}

void testIncorrectUri() {
    HeaderFieldStateMachine parser;
    std::string header = "GET HTTP/1.1\r\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Incorrect URI: Passed\n";
}

void    testIncorrectHttpVersion_1() {
    HeaderFieldStateMachine parser;
    std::string header = "GET / HTTP/1.0\r\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Incorrect HTTP Version: Passed\n";
}

void    testIncorrectHttpVersion_2() {
    HeaderFieldStateMachine parser;
    std::string header = "GET /\r\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);
    std::cout << "Test Incorrect HTTP Version: Passed\n";
}

//Test combination of request line and header fields

void    testRequestLineAndFieldsCombiantion()
{
    HeaderFieldStateMachine parser;
    std::string header = "GET / HTTP/1.1\r\naccept: text/html\r\nuser-agent: 88\r\n\r\n";
    bool caught = false;

    parser.parseOneHeaderLine(header);

    assert(parser.getHeaderMethod() == GET);
    assert(parser.getHeaderUri() == "/");
    assert(parser.getIsHttpVersionRight() == true);
    assert(parser.getParsedHeaders().at("accept").front() == "text/html");
    assert(parser.getParsedHeaders().at("user-agent").front() == "88");

    std::cout << "Test Request Line and Header Fields Combination: Passed\n";

}

void    testWrongRequestLineAndFieldsCombiantion()
{
    HeaderFieldStateMachine parser;
    std::string header = "GET / HTTP/1.1\r\nContent-Type: text/html\r5\nContent-Length: 88\r\n\r\n";
    bool caught = false;

    if (parser.parseOneHeaderLine(header) != OK)
        caught = true;

    assert(caught);

    std::cout << "Test Wrong Request Line and Header Fields Combination: Passed\n";
}

// Additional test cases would be similarly structured...
int main() {
    //header fields
    testValidHeader();
    testInvalidHeaderName();
    testLeadingAndTrailingWhitespace();
    testHeadersWithMultipleValues();
    testBufferOverflowProtection_1();
    testBufferOverflowProtection_2();
    testUnexpectedCharactersAfterHeaderLine();
    testMultipleHeaderFieds();
    testNoHeaderFields();
    testInvalidCRLF_1();
    testInvalidCRLF_2();
    // testInvalidCRLF_3();
    testEmptyHeaderFields();
    testEmptyInput();

    //request line
    testHeaderRequestLineParsing();
    testIncorrectMethod();
    testIncorrectUri();
    testIncorrectHttpVersion_1();
    testIncorrectHttpVersion_2();

    //combination of request line and header fields
    testRequestLineAndFieldsCombiantion();
    testWrongRequestLineAndFieldsCombiantion();

    // ... call other test functions
    
    std::cout << "All tests completed.\n";
    return 0;
}