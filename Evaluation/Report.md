
# ggm6

## Files
% ls

    Makefile
    README.md
    analysis_request.hpp
    code_analysis.cpp
    code_analysis.hpp
    code_analysis_t.cpp
    get_language_from_filename.cpp
    get_language_from_filename.hpp
    get_language_from_filename_t.cpp

## Changed files
% git diff baa2b2fa58cb52ada94597a9b2e5d687f6a3261f --name-only

    code_analysis.cpp
    code_analysis_t.cpp

## Build
% make

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ -std=c++11 -c get_language_from_filename.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    g++ -std=c++11 -c get_language_from_filename_t.cpp
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t

## Test
% make test

    ./code_analysis_t
    Extension not supported
    
    ---Separater for Collard's blank tests and my own---
    
    code_analysis...
    Incorrect: 
    Extension not supported
    Correct (no error messages): 
    
    Filename...
    bro.cpp
    howdy.c
    me.java
    
    Url... 
    With option_url: 
    www.bro.com
    Without option_url: 
    https://github.com/ggm6/CodeAnalysisMetadata
    
    Language...
    With option_language: 
    C++
    Without option_language: 
    Java

## Test Oracle: code\_analysis\_t.cpp
% cp ~collard/CodeAnalysisMetadataEvaluation/code_analysis_t.cpp .  
% make

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ -std=c++11 -c get_language_from_filename.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    g++ -std=c++11 -c get_language_from_filename_t.cpp
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t

% make test

    ./code_analysis_t
    Extension not supported
    ./code_analysis_t: code_analysis_t.cpp:62: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:79: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:96: main Check `code_analysis(request) == true' failed.
    Extension not supported
    Using stdin requires a declared language
    ./code_analysis_t: code_analysis_t.cpp:130: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:147: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:164: main Check `code_analysis(request) == true' failed.
    ./code_analysis_t: code_analysis_t.cpp:181: main Check `code_analysis(request) == true' failed.

## Test Oracle: code\_analysis.cpp
% cp ~collard/CodeAnalysisMetadataEvaluation/code_analysis.cpp  .  
% make

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ -std=c++11 -c get_language_from_filename.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    g++ -std=c++11 -c get_language_from_filename_t.cpp
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t

% make test

    ./code_analysis_t
    Extension not supported
    
    ---Separater for Collard's blank tests and my own---
    
    code_analysis...
    Incorrect: 
    Extension not supported
    Correct (no error messages): 
    Assertion failed: (code_analysis(request)==false), function main, file code_analysis_t.cpp, line 39.
    make: *** [test] Abort trap: 6

## Code
% srcml code_analysis.cpp code_analysis_t.cpp -o project.xml
% srcml --xpath="//src:function[src:name='code_analysis']" project.xml | srcml

     1 bool code_analysis(const analysis_request& request) {
     2 
     3     auto filename = analysis_filename(request);
     4 
     5     auto url = analysis_url(request);
     6 
     7     auto language = analysis_language(request, filename);
     8     
     9     if (language=="" && get_language_from_filename(request.option_filename)=="")
    10        std::cerr << "Extension not supported" << std::endl;
    11     if (request.given_filename=="-" && filename=="" && language=="")
    12        std::cerr << "Using stdin requires a declared language" << std::endl;
    13 
    14     return false;
    15 }

% srcml --xpath="//src:function[src:name='analysis_filename']" project.xml | srcml

     1 std::string analysis_filename(const analysis_request& request) {
     2     
     3     if (request.option_filename!="")
     4        return request.option_filename;
     5     if (request.option_filename=="" && request.entry_filename=="data" && request.given_filename!="")
     6        return request.given_filename;
     7     if (request.option_filename=="" && request.entry_filename!="")
     8        return request.entry_filename;
     9     return "";
    10 }

% srcml --xpath="//src:function[src:name='analysis_url']" project.xml | srcml

     1 std::string analysis_url(const analysis_request& request) {
     2 
     3     if (request.option_url!="")
     4        return request.option_url;
     5     if (request.given_url!="" && request.option_url=="")
     6        return request.given_url;
     7     return "";
     8 }

% srcml --xpath="//src:function[src:name='analysis_language']" project.xml | srcml

     1 std::string analysis_language(const analysis_request& request, const std::string& filename) {
     2 
     3     if (request.option_language!="")
     4        return request.option_language;
     5     else
     6        return get_language_from_filename(filename);
     7     return "";
     8 }

## Test Cases 
% srcml code_analysis_t.cpp --xpath="//src:function[src:name='main']/src:block" | srcml

    {
    
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

## Literal Values used in Testing
% srcml code\_analysis_t.cpp --xpath="//src:literal" | srcml | tr '\0' '\n' |grep '"' | sort -u | tr '\n' ','

    "---Separater for Collard's blank tests and my own---" "C++" "Correct 
    (no error messages): " "Filename..." "Incorrect: " "Language..." 
    "Url... " "With option_language: " "With option_url: " "Without 
    option_language: " "Without option_url: " "bro.cpp" "bro.cweojre" 
    "code_analysis..." "data" "howdy.c" 
    "https://github.com/ggm6/CodeAnalysisMetadata" "me.java" "www.bro.com" 

## Error Messages
% srcml code_analysis.cpp --xpath="//src:function[src:name='code_analysis']//src:expr[contains(., 'cerr')]//src:literal" | srcml | tr '\0'  '\n'

    "Extension not supported"
    "Using stdin requires a declared language"

## Commits

### Commit a2993d
% git checkout -q a2993d  
% make  

    make: Nothing to be done for `all'.

% git show

    commit a2993db4ef1c544f885db7f35449cb889023d09e
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Thu Apr 5 18:40:47 2018 -0400
    
        Adding error messages
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index b874829..ffc8a02 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -8,6 +8,7 @@
     #include "get_language_from_filename.hpp"
     
     #include <string>
    +#include <iostream>
     
     /** Generate source analysis based on the request
      * @param request Data that forms the request
    @@ -21,8 +22,11 @@ bool code_analysis(const analysis_request& request) {
         auto url = analysis_url(request);
     
         auto language = analysis_language(request, filename);
    -
    -    // code analysis processing that is not yet implemented
    +    
    +    if (language=="")
    +		std::cerr << "Extension not supported" << std::endl;
    +	if (filename=="")
    +		std::cerr << "Using stdin requires a declared language" << std::endl;
     
         return false;
     }
    @@ -33,6 +37,7 @@ bool code_analysis(const analysis_request& request) {
      */
     std::string analysis_filename(const analysis_request& request) {
     
    +	return get_language_from_filename(request.option_filename);
         return "";
     }
     
    diff --git a/code_analysis.o b/code_analysis.o
    new file mode 100644
    index 0000000..62277ee
    Binary files /dev/null and b/code_analysis.o differ
    diff --git a/code_analysis_t b/code_analysis_t
    new file mode 100755
    index 0000000..d0e1f16
    Binary files /dev/null and b/code_analysis_t differ
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    new file mode 100644
    index 0000000..aa8dc9b
    Binary files /dev/null and b/code_analysis_t.o differ
    diff --git a/get_language_from_filename.o b/get_language_from_filename.o
    new file mode 100644
    index 0000000..a9eb3bf
    Binary files /dev/null and b/get_language_from_filename.o differ
    diff --git a/get_language_from_filename_t b/get_language_from_filename_t
    new file mode 100755
    index 0000000..66c703f
    Binary files /dev/null and b/get_language_from_filename_t differ
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    new file mode 100644
    index 0000000..ffe97e2
    Binary files /dev/null and b/get_language_from_filename_t.o differ


### Commit 52ec34
% git checkout -q 52ec34  
% make  

    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    ld: warning: ignoring file code_analysis_t.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): code_analysis_t.o
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          analysis_filename(analysis_request const&) in code_analysis.o
      "_main", referenced from:
         implicit entry/start for main executable
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [code_analysis_t] Error 1

% git show

    commit 52ec34cf96ef94964ab79b11f9c1d80f3fffd6e7
    Author: ggm6 <35551740+ggm6@users.noreply.github.com>
    Date:   Fri Apr 6 11:03:44 2018 -0400
    
        Delete code_analysis.o
    
    diff --git a/code_analysis.o b/code_analysis.o
    deleted file mode 100644
    index 62277ee..0000000
    Binary files a/code_analysis.o and /dev/null differ


### Commit 5f09cd
% git checkout -q 5f09cd  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    ld: warning: ignoring file get_language_from_filename.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename.o
    Undefined symbols for architecture x86_64:
      "get_language_from_filename(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)", referenced from:
          analysis_filename(analysis_request const&) in code_analysis.o
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [code_analysis_t] Error 1

% git show

    commit 5f09cdf8e6886a78182a84baa71feb4036bc67e6
    Author: ggm6 <35551740+ggm6@users.noreply.github.com>
    Date:   Fri Apr 6 11:04:02 2018 -0400
    
        Delete code_analysis_t.o
    
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    deleted file mode 100644
    index aa8dc9b..0000000
    Binary files a/code_analysis_t.o and /dev/null differ


### Commit 4776cd
% git checkout -q 4776cd  
% make  

    g++ -std=c++11 -c get_language_from_filename.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t
    ld: warning: ignoring file get_language_from_filename_t.o, file was built for unsupported file format ( 0x7F 0x45 0x4C 0x46 0x02 0x01 0x01 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 ) which is not the architecture being linked (x86_64): get_language_from_filename_t.o
    Undefined symbols for architecture x86_64:
      "_main", referenced from:
         implicit entry/start for main executable
    ld: symbol(s) not found for architecture x86_64
    clang: error: linker command failed with exit code 1 (use -v to see invocation)
    make: *** [get_language_from_filename_t] Error 1

% git show

    commit 4776cd31e573cdf8a4fff5d2b689b880bd62b1ef
    Author: ggm6 <35551740+ggm6@users.noreply.github.com>
    Date:   Fri Apr 6 11:04:32 2018 -0400
    
        Delete get_language_from_filename.o
    
    diff --git a/get_language_from_filename.o b/get_language_from_filename.o
    deleted file mode 100644
    index a9eb3bf..0000000
    Binary files a/get_language_from_filename.o and /dev/null differ


### Commit 5f306b
% git checkout -q 5f306b  
% make  

    g++ -std=c++11 -c get_language_from_filename_t.cpp
    g++ get_language_from_filename_t.o get_language_from_filename.o -o get_language_from_filename_t

% git show

    commit 5f306b6ad50246a17efaf3095781a8feb8f34245
    Author: ggm6 <35551740+ggm6@users.noreply.github.com>
    Date:   Fri Apr 6 11:04:58 2018 -0400
    
        Delete get_language_from_filename_t.o
    
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ


### Commit 68bc82
% git checkout -q 68bc82  
% make  
error: Your local changes to the following files would be overwritten by checkout:
	code_analysis_t
	get_language_from_filename_t
Please commit your changes or stash them before you switch branches.
Aborting

    make: Nothing to be done for `all'.

% git show

    commit 5f306b6ad50246a17efaf3095781a8feb8f34245
    Author: ggm6 <35551740+ggm6@users.noreply.github.com>
    Date:   Fri Apr 6 11:04:58 2018 -0400
    
        Delete get_language_from_filename_t.o
    
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ


### Commit ce9a59
% git checkout -q ce9a59  
% make  
error: Your local changes to the following files would be overwritten by checkout:
	code_analysis_t
	get_language_from_filename_t
Please commit your changes or stash them before you switch branches.
Aborting

    make: Nothing to be done for `all'.

% git show

    commit 5f306b6ad50246a17efaf3095781a8feb8f34245
    Author: ggm6 <35551740+ggm6@users.noreply.github.com>
    Date:   Fri Apr 6 11:04:58 2018 -0400
    
        Delete get_language_from_filename_t.o
    
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ


### Commit 5d3a4f
% git checkout -q 5d3a4f  
% make  
error: Your local changes to the following files would be overwritten by checkout:
	code_analysis_t
	get_language_from_filename_t
Please commit your changes or stash them before you switch branches.
Aborting

    make: Nothing to be done for `all'.

% git show

    commit 5f306b6ad50246a17efaf3095781a8feb8f34245
    Author: ggm6 <35551740+ggm6@users.noreply.github.com>
    Date:   Fri Apr 6 11:04:58 2018 -0400
    
        Delete get_language_from_filename_t.o
    
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ


### Commit 3be9ee
% git checkout -q 3be9ee  
% make  
error: Your local changes to the following files would be overwritten by checkout:
	code_analysis_t
	get_language_from_filename_t
Please commit your changes or stash them before you switch branches.
Aborting

    make: Nothing to be done for `all'.

% git show

    commit 5f306b6ad50246a17efaf3095781a8feb8f34245
    Author: ggm6 <35551740+ggm6@users.noreply.github.com>
    Date:   Fri Apr 6 11:04:58 2018 -0400
    
        Delete get_language_from_filename_t.o
    
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ


### Commit 28cea5
% git checkout -q 28cea5  
% make  
error: Your local changes to the following files would be overwritten by checkout:
	code_analysis_t
	get_language_from_filename_t
Please commit your changes or stash them before you switch branches.
Aborting

    make: Nothing to be done for `all'.

% git show

    commit 5f306b6ad50246a17efaf3095781a8feb8f34245
    Author: ggm6 <35551740+ggm6@users.noreply.github.com>
    Date:   Fri Apr 6 11:04:58 2018 -0400
    
        Delete get_language_from_filename_t.o
    
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ


### Commit 3e12b4
% git checkout -q 3e12b4  
% make  
error: Your local changes to the following files would be overwritten by checkout:
	code_analysis_t
Please commit your changes or stash them before you switch branches.
error: The following untracked working tree files would be overwritten by checkout:
	code_analysis.o
	code_analysis_t.o
	get_language_from_filename.o
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    make: Nothing to be done for `all'.

% git show

    commit 5f306b6ad50246a17efaf3095781a8feb8f34245
    Author: ggm6 <35551740+ggm6@users.noreply.github.com>
    Date:   Fri Apr 6 11:04:58 2018 -0400
    
        Delete get_language_from_filename_t.o
    
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ


### Commit 640df0
% git checkout -q 640df0  
% make  
error: Your local changes to the following files would be overwritten by checkout:
	code_analysis_t
	get_language_from_filename_t
Please commit your changes or stash them before you switch branches.
Aborting

    make: Nothing to be done for `all'.

% git show

    commit 5f306b6ad50246a17efaf3095781a8feb8f34245
    Author: ggm6 <35551740+ggm6@users.noreply.github.com>
    Date:   Fri Apr 6 11:04:58 2018 -0400
    
        Delete get_language_from_filename_t.o
    
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ

