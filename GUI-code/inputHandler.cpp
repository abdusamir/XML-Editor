#include <iostream>
#include <string>
#include <regex>
#include <stack>
#include <bits/stdc++.h>
#include "errorHandler.h"
#include "compressor.h"
#include "inputHandler.h"

//overloading * operator to multiply indentations 

std::string operator*(std::string & str, int rhs)   {

    std::string out{};

    for(int i=0;i < rhs;i++)
        out += str;
    return out;
    }

outputXML mainLogic (std::string input)    {
    
    //Output XML files    
    outputXML output;

    //For prettifying XML output
    std::string tab {"\t"};
    int indentator{0};

    //Regular Expressions for searching & checking the document for errors
    std::regex otag ("(<[^</>]*>)");
    std::regex ctag ("(</[^<>]*>)");
    std::regex otag_check ("<[_a-zA-Z][$_a-zA-Z0-9]*( [_a-zA-Z]*=\"[$_a-zA-Z0-9]*\")*>");
    std::regex ctag_check ("</[_a-zA-Z][$_a-zA-Z0-9]*>");
    std::smatch otag_match;
    std::smatch ctag_match;

    //Variables for error checking
    std::string text {};
    std::string opening_tag {};
    std::string closing_tag {};
    std::stack<std::string> tags_order;
    bool otag_flag {0};
    int i{0},j{0};
    int old_pos {0};
    
    //Checking for opening or closing tags
    while((otag_flag = std::regex_search(input,otag_match,otag)) + std::regex_search(input,ctag_match,ctag))   {

        //Opening tag found
        if(otag_match.position(1) < ctag_match.position(1) && otag_flag != 0)   {
            opening_tag = otag_match.str(1);
       
            /**
             * Error checking : Tag Error
             **/

            try {
                if(!(regex_match(otag_match.str(1),otag_check)))  
                    throw Error(Error::tagError,STARTERR_TE,ENDERR_TE,otag_match.str(1));
            }

            //Error handler
            catch(Error &ex)  {
                opening_tag = ex.fixTE();
            }

            /**
            *Minify & Prettify
            **/

            output.mini += opening_tag;
            output.pretty += ((tab * indentator) + opening_tag + "\n");
            indentator ++;
                       
            /**
            *Error checking : text before opening tag
            **/

            try {
            if((otag_match.prefix().str()) != "") 
                throw Error(Error::extraUselessText,STARTERR_EUL,ENDERR_EUL,otag_match.prefix().str());
            }

            //Error handler
            catch(Error &ex)  {
                ex.fixEUL();
            }
            
            /**
            *Data handeling & checking
            **/

            //Saving opening tags order
            int counter{0};
            while((opening_tag[counter] != '>') && (opening_tag[counter] != ' '))
                counter++;
            tags_order.push(opening_tag.substr(1,counter - 1));
            //Taking input suffix for next iterations
            input = otag_match.suffix().str();
            //Saving line position
            old_pos = ENDERR_EUL + (opening_tag.size()) + 1;    
        }

        //Closing tag found
        else  {
            closing_tag = ctag_match.str(1);
            text = ctag_match.prefix().str();
            i=0; j=0;

            /**
            *Error checking : tag brackets inside text
            **/            

            try {
            for(auto x : text)
                if(x == '<' ||  x == '>')
                    throw Error(Error::tagBracketsInText,STARTERR_TBIT,ENDERR_TBIT,text);
            }

            //Error handler
            catch(Error &ex)  {
                text = ex.fixTBIT();
            }
            
            /**
            *Minify & prettify
            **/

            for(i=0;i < text.size();i++)
                if(text[i] == ' ')
                    continue;
                else
                    break;
            
            for(j=(text.size()-1);j>=0;j--)
                if(text[j] == ' ')
                    continue;
                else
                    break;
            
            output.mini += text.substr(i,(j-i+1));

            if(text.substr(i,j) != "")  
                output.pretty += ((tab * indentator) + text.substr(i,(j-i+1)) + "\n");
            indentator --;

            /**
            *Error checking : closing tag error
            **/

            try {
                if(!(regex_match(ctag_match.str(1),ctag_check)))
                    throw Error(Error::closingTagError,STARTERR_TOOO,ENDERR_TOOO,ctag_match.str(1));
            }

            //Error handler            
            catch(Error &ex)  {
                closing_tag = ex.fixCTE();
            }

            /**
            *Error checking : tags out of order
            **/

            try {
                if((tags_order.top()) == (closing_tag.substr(2,((closing_tag.size())-3))))    {
                    tags_order.pop();
                    output.mini += closing_tag;
                    output.pretty += ((tab * indentator) + closing_tag + "\n");

                }
                else {
                    throw Error(Error::tagsOutOfOrder,STARTERR_TOOO,ENDERR_TOOO,ctag_match.str(1));
                }
            }

            //Error handler
            catch(Error &ex)  {
                ex.fixTOOO();
                }

        //Taking input suffix for next iterations
        input = ctag_match.suffix().str();
        }

        //check end of text
        if(input == "")
            break;
    
    }
    //Extra line removed
    output.pretty = output.pretty.substr(0,(output.pretty.size()-1));

    return output;
}