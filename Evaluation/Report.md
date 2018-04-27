
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
% srcml --xpath="//src:function[src:name='code_analysis']" project.xml | srcml | cat -v

    bool code_analysis(const analysis_request& request) {
    
        auto filename = analysis_filename(request);
    
        auto url = analysis_url(request);
    
        auto language = analysis_language(request, filename);
        
        if (language=="" && get_language_from_filename(request.option_filename)=="")
    		std::cerr << "Extension not supported" << std::endl;
    	if (request.given_filename=="-" && filename=="" && language=="")
    		std::cerr << "Using stdin requires a declared language" << std::endl;
    
        return false;
    }

% srcml --xpath="//src:function[src:name='analysis_filename']" project.xml | srcml | cat -v

    std::string analysis_filename(const analysis_request& request) {
    	
    	if (request.option_filename!="")
    		return request.option_filename;
    	if (request.option_filename=="" && request.entry_filename=="data" && request.given_filename!="")
    		return request.given_filename;
    	if (request.option_filename=="" && request.entry_filename!="")
    		return request.entry_filename;
        return "";
    }

% srcml --xpath="//src:function[src:name='analysis_url']" project.xml | srcml | cat -v

    std::string analysis_url(const analysis_request& request) {
    
    	if (request.option_url!="")
    		return request.option_url;
    	if (request.given_url!="" && request.option_url=="")
    		return request.given_url;
        return "";
    }

% srcml --xpath="//src:function[src:name='analysis_language']" project.xml | srcml | cat -v

    std::string analysis_language(const analysis_request& request, const std::string& filename) {
    
    	if (request.option_language!="")
    		return request.option_language;
    	else
    		return get_language_from_filename(filename);
        return "";
    }

## Test Cases 
% srcml code_analysis_t.cpp --xpath="//src:function[src:name='main']/src:block" | srcml | cat -v

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
error: The following untracked working tree files would be overwritten by checkout:
	code_analysis.o
	code_analysis_t
	code_analysis_t.o
	get_language_from_filename.o
	get_language_from_filename_t
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 640df0252f3cd5be6760ba10760aeb34c3fa0a97
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Fri Apr 6 12:28:23 2018 -0400
    
        Testing and Implementation for analysis_language, as well as cleaned up analysis_filename
    
    diff --git a/code_analysis.o b/code_analysis.o
    deleted file mode 100644
    index 1518ece..0000000
    Binary files a/code_analysis.o and /dev/null differ
    diff --git a/code_analysis_t b/code_analysis_t
    deleted file mode 100755
    index 1a81063..0000000
    Binary files a/code_analysis_t and /dev/null differ
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    deleted file mode 100644
    index b8ba5d1..0000000
    Binary files a/code_analysis_t.o and /dev/null differ
    diff --git a/get_language_from_filename.o b/get_language_from_filename.o
    deleted file mode 100644
    index a9eb3bf..0000000
    Binary files a/get_language_from_filename.o and /dev/null differ
    diff --git a/get_language_from_filename_t b/get_language_from_filename_t
    deleted file mode 100755
    index 66c703f..0000000
    Binary files a/get_language_from_filename_t and /dev/null differ
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ

### Commit 52ec34
% git checkout -q 52ec34  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	code_analysis_t
	code_analysis_t.o
	get_language_from_filename.o
	get_language_from_filename_t
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    make: Nothing to be done for `all'.

% git show

    commit 640df0252f3cd5be6760ba10760aeb34c3fa0a97
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Fri Apr 6 12:28:23 2018 -0400
    
        Testing and Implementation for analysis_language, as well as cleaned up analysis_filename
    
    diff --git a/code_analysis.o b/code_analysis.o
    deleted file mode 100644
    index 1518ece..0000000
    Binary files a/code_analysis.o and /dev/null differ
    diff --git a/code_analysis_t b/code_analysis_t
    deleted file mode 100755
    index 1a81063..0000000
    Binary files a/code_analysis_t and /dev/null differ
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    deleted file mode 100644
    index b8ba5d1..0000000
    Binary files a/code_analysis_t.o and /dev/null differ
    diff --git a/get_language_from_filename.o b/get_language_from_filename.o
    deleted file mode 100644
    index a9eb3bf..0000000
    Binary files a/get_language_from_filename.o and /dev/null differ
    diff --git a/get_language_from_filename_t b/get_language_from_filename_t
    deleted file mode 100755
    index 66c703f..0000000
    Binary files a/get_language_from_filename_t and /dev/null differ
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ

### Commit 5f09cd
% git checkout -q 5f09cd  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	code_analysis_t
	get_language_from_filename.o
	get_language_from_filename_t
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    make: Nothing to be done for `all'.

% git show

    commit 640df0252f3cd5be6760ba10760aeb34c3fa0a97
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Fri Apr 6 12:28:23 2018 -0400
    
        Testing and Implementation for analysis_language, as well as cleaned up analysis_filename
    
    diff --git a/code_analysis.o b/code_analysis.o
    deleted file mode 100644
    index 1518ece..0000000
    Binary files a/code_analysis.o and /dev/null differ
    diff --git a/code_analysis_t b/code_analysis_t
    deleted file mode 100755
    index 1a81063..0000000
    Binary files a/code_analysis_t and /dev/null differ
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    deleted file mode 100644
    index b8ba5d1..0000000
    Binary files a/code_analysis_t.o and /dev/null differ
    diff --git a/get_language_from_filename.o b/get_language_from_filename.o
    deleted file mode 100644
    index a9eb3bf..0000000
    Binary files a/get_language_from_filename.o and /dev/null differ
    diff --git a/get_language_from_filename_t b/get_language_from_filename_t
    deleted file mode 100755
    index 66c703f..0000000
    Binary files a/get_language_from_filename_t and /dev/null differ
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ

### Commit 4776cd
% git checkout -q 4776cd  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	code_analysis_t
	get_language_from_filename_t
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    make: Nothing to be done for `all'.

% git show

    commit 640df0252f3cd5be6760ba10760aeb34c3fa0a97
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Fri Apr 6 12:28:23 2018 -0400
    
        Testing and Implementation for analysis_language, as well as cleaned up analysis_filename
    
    diff --git a/code_analysis.o b/code_analysis.o
    deleted file mode 100644
    index 1518ece..0000000
    Binary files a/code_analysis.o and /dev/null differ
    diff --git a/code_analysis_t b/code_analysis_t
    deleted file mode 100755
    index 1a81063..0000000
    Binary files a/code_analysis_t and /dev/null differ
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    deleted file mode 100644
    index b8ba5d1..0000000
    Binary files a/code_analysis_t.o and /dev/null differ
    diff --git a/get_language_from_filename.o b/get_language_from_filename.o
    deleted file mode 100644
    index a9eb3bf..0000000
    Binary files a/get_language_from_filename.o and /dev/null differ
    diff --git a/get_language_from_filename_t b/get_language_from_filename_t
    deleted file mode 100755
    index 66c703f..0000000
    Binary files a/get_language_from_filename_t and /dev/null differ
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ

### Commit 5f306b
% git checkout -q 5f306b  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	code_analysis_t
	get_language_from_filename_t
Please move or remove them before you switch branches.
Aborting

    make: Nothing to be done for `all'.

% git show

    commit 640df0252f3cd5be6760ba10760aeb34c3fa0a97
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Fri Apr 6 12:28:23 2018 -0400
    
        Testing and Implementation for analysis_language, as well as cleaned up analysis_filename
    
    diff --git a/code_analysis.o b/code_analysis.o
    deleted file mode 100644
    index 1518ece..0000000
    Binary files a/code_analysis.o and /dev/null differ
    diff --git a/code_analysis_t b/code_analysis_t
    deleted file mode 100755
    index 1a81063..0000000
    Binary files a/code_analysis_t and /dev/null differ
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    deleted file mode 100644
    index b8ba5d1..0000000
    Binary files a/code_analysis_t.o and /dev/null differ
    diff --git a/get_language_from_filename.o b/get_language_from_filename.o
    deleted file mode 100644
    index a9eb3bf..0000000
    Binary files a/get_language_from_filename.o and /dev/null differ
    diff --git a/get_language_from_filename_t b/get_language_from_filename_t
    deleted file mode 100755
    index 66c703f..0000000
    Binary files a/get_language_from_filename_t and /dev/null differ
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ

### Commit 68bc82
% git checkout -q 68bc82  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 68bc828043cacc08491a38b2c30990d5c46b6b29
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Fri Apr 6 11:05:56 2018 -0400
    
        Cleaned up error handling with test cases
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index ffc8a02..2c34606 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -23,9 +23,9 @@ bool code_analysis(const analysis_request& request) {
     
         auto language = analysis_language(request, filename);
         
    -    if (language=="")
    +    if (language=="" && get_language_from_filename(request.option_filename)=="")
     		std::cerr << "Extension not supported" << std::endl;
    -	if (filename=="")
    +	if (filename=="" && language=="")
     		std::cerr << "Using stdin requires a declared language" << std::endl;
     
         return false;
    @@ -47,6 +47,8 @@ std::string analysis_filename(const analysis_request& request) {
      */
     std::string analysis_url(const analysis_request& request) {
     
    +	if (request.option_url!="")
    +		return request.option_url;
         return "";
     }
     
    @@ -57,5 +59,7 @@ std::string analysis_url(const analysis_request& request) {
      */
     std::string analysis_language(const analysis_request& request, const std::string& filename) {
     
    +	if (request.option_language!="")
    +		return request.option_language;
         return "";
     }
    diff --git a/code_analysis.o b/code_analysis.o
    deleted file mode 100644
    index 62277ee..0000000
    Binary files a/code_analysis.o and /dev/null differ
    diff --git a/code_analysis_t b/code_analysis_t
    deleted file mode 100755
    index d0e1f16..0000000
    Binary files a/code_analysis_t and /dev/null differ
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 820446d..1793bc0 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -6,6 +6,7 @@
     
     #include "code_analysis.hpp"
     
    +#include <iostream>
     #include <string>
     #include <cassert>
     
    @@ -26,6 +27,15 @@ int main() {
             assert(analysis_url(request) == "");
             assert(analysis_language(request, filename) == "");
             assert(code_analysis(request) == false);
    +        
    +        // Garrett's Tests
    +        std::cout << std::endl << "Separater for Collard's blank tests and my own..." << std::endl;
    +        std::cout << "Incorrect: " << std::endl;
    +        request.option_filename="bro.cweojre";
    +        assert(code_analysis(request)==false);
    +        std::cout << "Correct (no error messages): " << std::endl;
    +        request.option_filename="bro.cpp";
    +        assert(code_analysis(request)==false);
         }
     
         return 0;
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    deleted file mode 100644
    index aa8dc9b..0000000
    Binary files a/code_analysis_t.o and /dev/null differ
    diff --git a/get_language_from_filename.o b/get_language_from_filename.o
    deleted file mode 100644
    index a9eb3bf..0000000
    Binary files a/get_language_from_filename.o and /dev/null differ
    diff --git a/get_language_from_filename_t b/get_language_from_filename_t
    deleted file mode 100755
    index 66c703f..0000000
    Binary files a/get_language_from_filename_t and /dev/null differ
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ

### Commit ce9a59
% git checkout -q ce9a59  
% make  

    make: Nothing to be done for `all'.

% git show

    commit ce9a5902f445b58f40dce2b6bcd70dcc88403c41
    Merge: 68bc828 5f306b6
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Fri Apr 6 11:06:18 2018 -0400
    
        Merge branch 'master' of https://github.com/ggm6/CodeAnalysisMetadata
    

### Commit 5d3a4f
% git checkout -q 5d3a4f  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 5d3a4f0aab92d88621baa78529ec2c1145a89023
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Fri Apr 6 11:38:28 2018 -0400
    
        Testing and Implementation for analysis_filename complete
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index 2c34606..ac625d4 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -25,7 +25,7 @@ bool code_analysis(const analysis_request& request) {
         
         if (language=="" && get_language_from_filename(request.option_filename)=="")
     		std::cerr << "Extension not supported" << std::endl;
    -	if (filename=="" && language=="")
    +	if (request.given_filename=="-" && filename=="" && language=="")
     		std::cerr << "Using stdin requires a declared language" << std::endl;
     
         return false;
    @@ -36,8 +36,13 @@ bool code_analysis(const analysis_request& request) {
      * @retval filename
      */
     std::string analysis_filename(const analysis_request& request) {
    -
    -	return get_language_from_filename(request.option_filename);
    +	
    +	if (request.option_filename!="")
    +		return get_language_from_filename(request.option_filename);
    +	if (request.entry_filename=="data" && request.given_filename!="")
    +		return get_language_from_filename(request.given_filename);
    +	if (request.entry_filename!="")
    +		return get_language_from_filename(request.entry_filename);
         return "";
     }
     
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index 1793bc0..aec340c 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -36,6 +36,16 @@ int main() {
             std::cout << "Correct (no error messages): " << std::endl;
             request.option_filename="bro.cpp";
             assert(code_analysis(request)==false);
    +        std::cout << std::endl;
    +        
    +        std::cout << "Language from filename: " << std::endl;
    +		std::cout << analysis_filename(request) << std::endl;
    +		request.option_filename="";
    +		request.entry_filename="data";
    +		request.given_filename="howdy.c";
    +		std::cout << analysis_filename(request) << std::endl;
    +		request.entry_filename="me.java";
    +		std::cout << analysis_filename(request) << std::endl;
         }
     
         return 0;

### Commit 3be9ee
% git checkout -q 3be9ee  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 3be9ee064536e8a6240bc1783189e47bfef10f72
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Fri Apr 6 11:53:11 2018 -0400
    
        Analysis_filename cleaned up
    
    diff --git a/code_analysis.cpp b/code_analysis.cpp
    index ac625d4..4575cdf 100644
    --- a/code_analysis.cpp
    +++ b/code_analysis.cpp
    @@ -39,9 +39,9 @@ std::string analysis_filename(const analysis_request& request) {
     	
     	if (request.option_filename!="")
     		return get_language_from_filename(request.option_filename);
    -	if (request.entry_filename=="data" && request.given_filename!="")
    +	if (request.option_filename=="" && request.entry_filename=="data" && request.given_filename!="")
     		return get_language_from_filename(request.given_filename);
    -	if (request.entry_filename!="")
    +	if (request.option_filename=="" && request.entry_filename!="")
     		return get_language_from_filename(request.entry_filename);
         return "";
     }
    @@ -54,6 +54,8 @@ std::string analysis_url(const analysis_request& request) {
     
     	if (request.option_url!="")
     		return request.option_url;
    +	if (request.given_url!="" && request.option_url=="")
    +		return request.given_url;
         return "";
     }
     
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index aec340c..f080417 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -46,6 +46,8 @@ int main() {
     		std::cout << analysis_filename(request) << std::endl;
     		request.entry_filename="me.java";
     		std::cout << analysis_filename(request) << std::endl;
    +		
    +		
         }
     
         return 0;

### Commit 28cea5
% git checkout -q 28cea5  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 28cea5e8edbb54857b3385702870d872dba728c7
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Fri Apr 6 12:04:09 2018 -0400
    
        Adding Testing and Implementation of analysis_url
    
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index f080417..0bc9b6e 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -29,7 +29,8 @@ int main() {
             assert(code_analysis(request) == false);
             
             // Garrett's Tests
    -        std::cout << std::endl << "Separater for Collard's blank tests and my own..." << std::endl;
    +        std::cout << std::endl << "---Separater for Collard's blank tests and my own---" << std::endl;
    +        std::cout << "code_analysis..." << std::endl;
             std::cout << "Incorrect: " << std::endl;
             request.option_filename="bro.cweojre";
             assert(code_analysis(request)==false);
    @@ -38,15 +39,23 @@ int main() {
             assert(code_analysis(request)==false);
             std::cout << std::endl;
             
    -        std::cout << "Language from filename: " << std::endl;
    +        std::cout << "Language from filename..." << std::endl;
     		std::cout << analysis_filename(request) << std::endl;
     		request.option_filename="";
     		request.entry_filename="data";
     		request.given_filename="howdy.c";
     		std::cout << analysis_filename(request) << std::endl;
     		request.entry_filename="me.java";
    -		std::cout << analysis_filename(request) << std::endl;
    +		std::cout << analysis_filename(request) << std::endl << std::endl;
     		
    +		std::cout << "Url... " << std::endl;
    +		request.given_url="https://github.com/ggm6/CodeAnalysisMetadata";
    +		request.option_url="www.bro.com";
    +		std::cout << "With option_url: " << std::endl;
    +		std::cout << analysis_url(request) << std::endl;
    +		request.option_url="";
    +		std::cout << "Without option_url: " << std::endl;
    +		std::cout << analysis_url(request) << std::endl;
     		
         }
     

### Commit 3e12b4
% git checkout -q 3e12b4  
% make  
error: The following untracked working tree files would be overwritten by checkout:
	code_analysis.o
	code_analysis_t
	code_analysis_t.o
	get_language_from_filename.o
	get_language_from_filename_t
	get_language_from_filename_t.o
Please move or remove them before you switch branches.
Aborting

    make: Nothing to be done for `all'.

% git show

    commit 28cea5e8edbb54857b3385702870d872dba728c7
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Fri Apr 6 12:04:09 2018 -0400
    
        Adding Testing and Implementation of analysis_url
    
    diff --git a/code_analysis_t.cpp b/code_analysis_t.cpp
    index f080417..0bc9b6e 100644
    --- a/code_analysis_t.cpp
    +++ b/code_analysis_t.cpp
    @@ -29,7 +29,8 @@ int main() {
             assert(code_analysis(request) == false);
             
             // Garrett's Tests
    -        std::cout << std::endl << "Separater for Collard's blank tests and my own..." << std::endl;
    +        std::cout << std::endl << "---Separater for Collard's blank tests and my own---" << std::endl;
    +        std::cout << "code_analysis..." << std::endl;
             std::cout << "Incorrect: " << std::endl;
             request.option_filename="bro.cweojre";
             assert(code_analysis(request)==false);
    @@ -38,15 +39,23 @@ int main() {
             assert(code_analysis(request)==false);
             std::cout << std::endl;
             
    -        std::cout << "Language from filename: " << std::endl;
    +        std::cout << "Language from filename..." << std::endl;
     		std::cout << analysis_filename(request) << std::endl;
     		request.option_filename="";
     		request.entry_filename="data";
     		request.given_filename="howdy.c";
     		std::cout << analysis_filename(request) << std::endl;
     		request.entry_filename="me.java";
    -		std::cout << analysis_filename(request) << std::endl;
    +		std::cout << analysis_filename(request) << std::endl << std::endl;
     		
    +		std::cout << "Url... " << std::endl;
    +		request.given_url="https://github.com/ggm6/CodeAnalysisMetadata";
    +		request.option_url="www.bro.com";
    +		std::cout << "With option_url: " << std::endl;
    +		std::cout << analysis_url(request) << std::endl;
    +		request.option_url="";
    +		std::cout << "Without option_url: " << std::endl;
    +		std::cout << analysis_url(request) << std::endl;
     		
         }
     

### Commit 640df0
% git checkout -q 640df0  
% make  

    g++ -std=c++11 -c code_analysis_t.cpp
    g++ -std=c++11 -c code_analysis.cpp
    g++ code_analysis_t.o code_analysis.o get_language_from_filename.o -o code_analysis_t

% git show

    commit 640df0252f3cd5be6760ba10760aeb34c3fa0a97
    Author: Garrett <ggm6@zips.uakron.edu>
    Date:   Fri Apr 6 12:28:23 2018 -0400
    
        Testing and Implementation for analysis_language, as well as cleaned up analysis_filename
    
    diff --git a/code_analysis.o b/code_analysis.o
    deleted file mode 100644
    index 1518ece..0000000
    Binary files a/code_analysis.o and /dev/null differ
    diff --git a/code_analysis_t b/code_analysis_t
    deleted file mode 100755
    index 1a81063..0000000
    Binary files a/code_analysis_t and /dev/null differ
    diff --git a/code_analysis_t.o b/code_analysis_t.o
    deleted file mode 100644
    index b8ba5d1..0000000
    Binary files a/code_analysis_t.o and /dev/null differ
    diff --git a/get_language_from_filename.o b/get_language_from_filename.o
    deleted file mode 100644
    index a9eb3bf..0000000
    Binary files a/get_language_from_filename.o and /dev/null differ
    diff --git a/get_language_from_filename_t b/get_language_from_filename_t
    deleted file mode 100755
    index 66c703f..0000000
    Binary files a/get_language_from_filename_t and /dev/null differ
    diff --git a/get_language_from_filename_t.o b/get_language_from_filename_t.o
    deleted file mode 100644
    index ffe97e2..0000000
    Binary files a/get_language_from_filename_t.o and /dev/null differ

