#include "searcheng.h"
#include "combiners.h"
#include <set>

// Complete the necessary code
AndWebPageSetCombiner::AndWebPageSetCombiner()
{

}
WebPageSet AndWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB) //O(mlogn) , m=size of setA, n=size of setB
{
    WebPageSet::iterator it;
    WebPageSet anded_set;

    for (it= setA.begin(); it!=setA.end(); ++it)
    {
        
        if(setB.find(*it)!= setB.end()) //if setB has the same webpage also, meaning the webpage that is both in setA and setB has both the termA and termB terms in it 
        {
            anded_set.insert(*it); //add it to the anded_set to be returned
        }
    
        
    }
    
    return(anded_set);
}
OrWebPageSetCombiner::OrWebPageSetCombiner()
{
    
}
WebPageSet OrWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB) //O(m+n), m=size of setA, n=size of setB
{
    WebPageSet::iterator it;
    WebPageSet ored_set;
    WebPageSet::iterator it2;
    
    for (it= setA.begin(); it!=setA.end(); ++it)
    {
        ored_set.insert(*it);
    }
    for (it= setB.begin(); it!=setB.end(); ++it)
    {
        ored_set.insert(*it);   
    }
    return(ored_set);
}
DiffWebPageSetCombiner::DiffWebPageSetCombiner()
{

}
WebPageSet DiffWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB) //O(mlogn), m=size of setA, n=size of setB
{
    WebPageSet::iterator it;
    WebPageSet diffed_set;

    for (it= setA.begin(); it!=setA.end(); ++it)
    {
        if(setB.find(*it)== setB.end()) //if setB does not contain that WebPage
        {
            diffed_set.insert(*it); 
        }
    }
    return(diffed_set);
}