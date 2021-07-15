#include <iostream>
#include <regex>
#include "errorHandler.h"


void Error::sendMessege(GUImessege mes)	{

}

void Error::recieveMessege()	{

}

std::string Error::fixTE()	{
	GUImessege errorLog;
	errorLog.errStart = this->start;
	errorLog.errEnd = this->end;
	int i{0},j{0},k{0},n{0};

	std::regex test1 ("<[_a-zA-Z][$_a-zA-Z0-9]*", std::regex_constants::basic);
	std::regex test2 (" [^0-9][0-9_a-zA-Z]*=\"[$_a-zA-Z0-9]*\")*>", std::regex_constants::basic);
	std::regex test3 ("([_a-zA-Z]*", std::regex_constants::basic);
	std::regex test4 ("\"[$_a-zA-Z0-9]*\"", std::regex_constants::basic);
	std::regex test5 ("[a-z]*",std::regex_constants::basic);
	
	while(this->errorLine.substr(i,1) != " ")	{
		i++;
	}

	if(!regex_match(errorLine.substr(0,i),test1))	{

		for(k=0;k<i;k++)	{
			if(regex_match((Error::errorLine.substr(k,1)),test5))	{
				break;
			}
		}
				errorLog.errorFix = "<" + this->errorLine.substr(k,(i-k));
	} 


		bool flag = 0;
		i++;
		j=i+1;
		while(!flag)	{
		j=i;
		while(this->errorLine.substr(j,1) != "=")	{
			if(this->errorLine.substr(j,1) == ">")	{
				flag = 1;
				break;
			}
			j++;
		}

		if(!regex_match(this->errorLine.substr(i,(j-i)),test2))	{

				for(k=i;k<errorLine.size();k++)	{
					if(regex_match((Error::errorLine.substr(k,1)),test5))	{
						break;
					}
				}
				if(flag != 1)
					errorLog.errorFix += " " + this->errorLine.substr(k,(j-k));
				
				i = j+1;
				n = j;
				while(this->errorLine.substr(n,1) != " " && this->errorLine.substr(n,1) != ">")	{
					n++;
				}
				j++;
				
				if(!regex_match(this->errorLine.substr(j,n-j),test4))	{
					if(flag != 1)
						errorLog.errorFix += "=\"";
					while(j<n)	{
						if(this->errorLine.substr(j,1) == "\""){
							j++;
							continue;
						}
						errorLog.errorFix += this->errorLine.substr(j,1);
	j++;
						}
						errorLog.errorFix += "\"";
				}
		}

	}
			errorLog.errorFix += ">";

	Error::sendMessege(errorLog);
	return errorLog.errorFix;

}

std::string Error::fixTBIT()	{
	GUImessege errorLog;
	errorLog.errStart = this->start;
	errorLog.errEnd = this->end;

	int i{0};
	std::cout << this->errorLine << std::endl;
	for(i=0;i < (this->errorLine.size());i++)	{	
		if(((this->errorLine.substr(i,1)) == "<") || ((this->errorLine.substr(i,1)) == ">"))
			continue;
			errorLog.errorFix += (this->errorLine.substr(i,1));
	}
	Error::sendMessege(errorLog);
	std::cout << "fixed\t" << errorLog.errorFix << std::endl;
	return errorLog.errorFix;
}

void Error::fixEUL()	{
	GUImessege errorLog;
	errorLog.errStart = this->start;
	errorLog.errEnd = this->end;
	Error::sendMessege(errorLog);
}

void Error::fixTOOO()	{
	GUImessege errorLog;
	errorLog.errStart = this->start;
	errorLog.errEnd = this->end;
	Error::sendMessege(errorLog);
}

std::string Error::fixCTE()	{
	GUImessege errorLog;
	errorLog.errStart = this->start;
	errorLog.errEnd = this->end;

	std::regex test1 ("</[_a-zA-Z][$_a-zA-Z0-9]*", std::regex_constants::basic);
	std::regex test2 ("[a-z]*",std::regex_constants::basic);
	int i{0},k{0};

	while(this->errorLine.substr(i,1) != ">")	{
		i++;
	}

	if(!regex_match(errorLine.substr(0,i),test1))	{

		for(k=0;k<i;k++)	{
			if(regex_match((Error::errorLine.substr(k,1)),test2))	{
				break;
			}
		}
				errorLog.errorFix = "</" + this->errorLine.substr(k,(i-k)) + ">";
	} 
	Error::sendMessege(errorLog);
	return errorLog.errorFix;

}

