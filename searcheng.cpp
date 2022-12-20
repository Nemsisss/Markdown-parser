#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include "searcheng.h"


using namespace std;

// Helper function that will extract the extension of a filename
std::string extract_extension(const std::string& filename);

std::string extract_extension(const std::string& filename)
{
    size_t idx = filename.rfind(".");
    if (idx == std::string::npos) {
        return std::string();
    }
    return filename.substr(idx + 1);
}


// To be updated as needed 
SearchEng::SearchEng()
{
    
}

// To be completed
SearchEng::~SearchEng()
{
     std::map<std::string, WebPage*>::iterator it;
     std::map<std::string, PageParser*>::iterator it2;
     for(it= fileNames_of_webpages.begin(); it!= fileNames_of_webpages.end(); ++it)
     {
         delete (it->second);
     }
     for(it2=parsers_.begin(); it2!=parsers_.end(); ++it2)
     {
         delete(it2->second);
     }
}

// Complete
void SearchEng::register_parser(const std::string& extension, PageParser* parser)
{
    if (parsers_.find(extension) != parsers_.end())
    {
        throw std::invalid_argument("parser for provided extension already exists");
    }
    parsers_.insert(make_pair(extension, parser));
}

// Complete
void SearchEng::read_pages_from_index(const std::string& index_file)
{
    ifstream ifile(index_file.c_str());
    if(ifile.fail()) {
        cerr << "Unable to open index file: " << index_file << endl;
    }

    // Parse all the files
    string filename;
    while(ifile >> filename) {
#ifdef DEBUG
        cout << "Reading " << filename << endl;
#endif
        read_page(filename);
    }
    ifile.close();
}

// To be completed
void SearchEng::read_page(const string& filename)
{
    string ext = extract_extension(filename);
    std::set<std::string> terms;
    std::set<std::string> links;
   
    if(ext!="" && parsers_.find(ext)==parsers_.end())
    {
        throw std::logic_error("No parser is registered for that extension");
    }
    ifstream ifile(filename);
    if(ifile.fail())
    {
        throw std:: invalid_argument("The filename does not exit");
    }
    std::map<std::string, PageParser*>::iterator it= parsers_.find(ext); //get the correct type of iterator
    PageParser* pageParser = it->second; //set the type of the parser to be used
    WebPage* pageToBeRetrieved=retrieve_page(filename); //get the webPage* to update/add

    if(pageToBeRetrieved==NULL) //if the page does not exist
    {
        pageToBeRetrieved = add_new_webpage(filename);
    }
    
    pageParser->parse(ifile, terms,links );
    std::set<std::string>::const_iterator itr;
    pageToBeRetrieved->all_terms(terms);//update the all terms of the webpage
    //loop through terms to add terms to the map terms_w_webpages
    for(itr= terms.begin(); itr!=terms.end(); ++itr)
    {
        terms_w_webpagaes[*itr].insert(pageToBeRetrieved); //wont insert if the page is already in the term's set, also wont add the term if the term already exists
    }
    //loop through links to update outgoing and incoming links of pagetobeparsed
    WebPage* currPage ;
    for(itr= links.begin(); itr!=links.end();++itr)
    {
        currPage=retrieve_page(*itr); //check if the link already exists in the filename_of_webpages
        if(currPage==NULL) 
        {
            currPage= add_new_webpage(*itr);
        }
        pageToBeRetrieved->add_outgoing_link(currPage);
        currPage->add_incoming_link(pageToBeRetrieved);
    }
}
WebPage* SearchEng:: add_new_webpage(const std::string& page_name )
{
    WebPage* newPage= new WebPage(page_name);
    fileNames_of_webpages.insert(make_pair(page_name, newPage));
    return newPage;
}

// To be completed
WebPage* SearchEng::retrieve_page(const std::string& page_name) const
{
    std::map<std::string, WebPage*>:: const_iterator it= fileNames_of_webpages.find(page_name);
    if(it==fileNames_of_webpages.end())
    {
        return(NULL);
    }
    return it->second;
}

// To be completed
void SearchEng::display_page(std::ostream& ostr, const std::string& page_name) const
{
    string ext = extract_extension(page_name);
    if(ext!="" && parsers_.find(ext)==parsers_.end())
    {
        throw std::logic_error("No parser is registered for that extension");
    }
    ifstream ifile(page_name);
    if(ifile.fail())
    {
        throw std:: invalid_argument("The filename does not exit");
    }
    std::map<std::string, PageParser*>::const_iterator it= parsers_.find(ext); //get the correct type of iterator
    PageParser* pageParser = it->second; //set the type of the parser to be used
    ostr<<pageParser->display_text(ifile);
}

// To be completed
WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) const
{
    WebPageSet toBeReturned;
    WebPageSet prevSet;
    std::map<string, WebPageSet>::const_iterator it2;
     if(!terms.empty())
     {
        for(long unsigned int i=0; i<terms.size(); i++) 
        {
            it2 =terms_w_webpagaes.find(terms[i]); //O(klogn), k = number of query search terms, n=total number of terms in the map
            if(it2!=terms_w_webpagaes.end())
            {
                if(prevSet.empty() && toBeReturned.empty()) //if this is the first term found with webpage matches
                {
                    prevSet= it2->second;
                    if(terms.size()==1)
                    {
                        return prevSet; // if there's only one term, return the WebPageSet that contains that term
                    }
                }
                else if((!prevSet.empty()) && toBeReturned.empty()) //if this is the second term found with webpage matches
                {
                    toBeReturned=combiner->combine(prevSet, it2->second); //O(kmlogm)
                }
                else
                {
                    toBeReturned=combiner->combine(toBeReturned, it2->second); //O(kmlogm) , m = max # of webpages that match each term 
                }
            } 
        }
     }
    
    return(toBeReturned);
}

// Add private helper function implementations here

