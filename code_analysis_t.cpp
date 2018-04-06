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
        std::cout << std::endl << "Separater for Collard's blank tests and my own..." << std::endl;
        std::cout << "Incorrect: " << std::endl;
        request.option_filename="bro.cweojre";
        assert(code_analysis(request)==false);
        std::cout << "Correct (no error messages): " << std::endl;
        request.option_filename="bro.cpp";
        assert(code_analysis(request)==false);
        std::cout << std::endl;
        
        std::cout << "Language from filename: " << std::endl;
		std::cout << analysis_filename(request) << std::endl;
		request.option_filename="";
		request.entry_filename="data";
		request.given_filename="howdy.c";
		std::cout << analysis_filename(request) << std::endl;
		request.entry_filename="me.java";
		std::cout << analysis_filename(request) << std::endl;
		
		
    }

    return 0;
}
