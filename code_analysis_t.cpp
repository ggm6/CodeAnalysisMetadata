/*
  @file code_analysis_t.cpp

  Test program for analysis requests
*/

#include "code_analysis.hpp"

#include <iostream>
#include <string>
#include <cassert>

int main() {

    // all parts of the request are empty
    {
        analysis_request request;
        request.given_filename  = "";
        request.entry_filename  = "";
        request.given_url       = "";
        request.option_filename = "";
        request.option_url      = "";
        request.option_language = "";

        auto filename = analysis_filename(request);
        assert(filename == "");
        assert(analysis_url(request) == "");
        assert(analysis_language(request, filename) == "");
        assert(code_analysis(request) == false);
        
        // Garrett's Tests
        std::cout << std::endl << "---Separater for Collard's blank tests and my own---" << std::endl << std::endl;
        std::cout << "code_analysis..." << std::endl;
        std::cout << "Incorrect: " << std::endl;
        request.option_filename="bro.cweojre";
        assert(code_analysis(request)==false);
        std::cout << "Correct (no error messages): " << std::endl;
        request.option_filename="bro.cpp";
        assert(code_analysis(request)==false);
        std::cout << std::endl;
        
        std::cout << "Filename..." << std::endl;
		std::cout << analysis_filename(request) << std::endl;
		request.option_filename="";
		request.entry_filename="data";
		request.given_filename="howdy.c";
		std::cout << analysis_filename(request) << std::endl;
		request.entry_filename="me.java";
		std::cout << analysis_filename(request) << std::endl << std::endl;
		
		std::cout << "Url... " << std::endl;
		request.given_url="https://github.com/ggm6/CodeAnalysisMetadata";
		request.option_url="www.bro.com";
		std::cout << "With option_url: " << std::endl;
		std::cout << analysis_url(request) << std::endl;
		request.option_url="";
		std::cout << "Without option_url: " << std::endl;
		std::cout << analysis_url(request) << std::endl << std::endl;
		
		std::cout << "Language..." << std::endl;
		request.option_language="C++";
		std::cout << "With option_language: " << std::endl;
		std::cout << analysis_language(request,analysis_filename(request)) << std::endl;
		request.option_language="";
		std::cout << "Without option_language: " << std::endl;
		std::cout << analysis_language(request,analysis_filename(request)) << std::endl;
    }

    return 0;
}
