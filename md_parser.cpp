#include <iostream>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <cctype>
#include "md_parser.h"
#include "util.h"

using namespace std;

typedef enum { NORMALTEXT, LINKTEXT, ISLINK, LINKURL } PARSE_STATE_T;

// To be completed
void MDParser::parse(std::istream& istr, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks)
{

    // Remove any contents of the sets before starting to parse.
    allSearchableTerms.clear();
    allOutgoingLinks.clear();

    // Complete your code below

    // Initialize the current term and link as empty strings
    string term = "";
    string link= "";

    // Initialize the current state
    PARSE_STATE_T state=NORMALTEXT ;

    // parse the first character from the file 
    char c = istr.get();

    // Continue reading from the file until input fails.
    while(!istr.fail())
    {
          // if c is a special character, check what c is
          if (!isalnum(c) )
          { 
            if(c=='[')
            {
                //change the state
                state=LINKTEXT;
            }
            else if (c==']' && state==LINKTEXT)
            {
                state=ISLINK;               
            }
            else if ( c=='('  && state==ISLINK)
            {
                state= LINKURL;   
            }
            else if(c==')' && state==LINKURL) 
            {
                if(link != "")
                {
                    allOutgoingLinks.insert(link);
                    link="";
                }
                state= NORMALTEXT; //set state back to normal text after being done extracting the link           
            }
            if(term != "" && state!=LINKURL)
            {
                term = conv_to_lower(term);
                allSearchableTerms.insert(term);
            }
            term = "";
            
        }
        // if c is not a special character
        if (isalnum(c) && state!= LINKURL)
        {
            term+=c;
        }
        else if (state==LINKURL && c!='(')
        {
            link+=c; 

        }

        // get another character from the file
        c = istr.get();
    }
    // Since the last term in the file may not have punctuation, there may be a valid term in
    // the "term" variable, so we need to insert it into the allSearchableTerms set.
    if(term != "" )
    {
        term = conv_to_lower(term);
        allSearchableTerms.insert(term);
    }
    if(link != "")
    {
        allOutgoingLinks.insert(link);
    }
}

// To be completed - incomplete, needs to be modified
std::string MDParser::display_text(std::istream& istr)
{
    std::string retval;

    // Initialize the current state
    PARSE_STATE_T state=NORMALTEXT ;
    //PARSE_STATE_T prevState ;

    // parse the first character from the file 
    char c = istr.get();
    // Continue reading from the file until input fails.
    while(!istr.fail())
    {
         // if c is a special character, check what c is
         if (!isalnum(c) )
         { 
            if(c=='[')
            {
                //change the state
                state=LINKTEXT;
            }
            else if (c==']' && state==LINKTEXT)
            {
                state=ISLINK;               
            }
            else if ( c=='('  && state==ISLINK)
            {
                state= LINKURL;    
            }
            else if(c==')' && state==LINKURL) 
            {
                state= NORMALTEXT; //set state back to normal text after being done extracting the link           
            }
        }
        // if c is not a special character
        if (state!= LINKURL && c!=')')
        {
            retval+=c;
        }
        c = istr.get();
    }
    return (retval);
}


