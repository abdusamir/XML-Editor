#include <iostream>
#include <bits/stdc++.h>
#include <bitset>
#include "compressor.h"

/**
 * 
 * ENCODING
 * 
 **/

std::vector<int> Compressor::encode(std::string in)	{
	
	std::vector<int> result {};
	std::unordered_map<std::string,int> LUtable;
	int index {256};
	std::string prev{""},curr{""};
	std::string insert {""};

	for(int i=0;i <= 255;i++){
		insert += char(i);
		LUtable[insert] = i;
		insert = "";
	}
	
	prev += in[0];
	
	for(int i=0;i < in.size();i++)	{
		
		curr += in[i+1];
		
		if(LUtable.find(prev+curr) != LUtable.end())	{
			prev = prev + curr;
		}
		else	{
			//test = prev+curr+"" ;
			result.push_back(LUtable[prev]);
			LUtable[prev+curr] = index;
			//std::cout << LUtable[prev+curr+""] << "\t" << (test == "ma") << std::endl;
			index++;
			prev = curr;
		}
		curr = "";
	}
	//result.push_back(LUtable[prev]);
	return result;

}

std::string	Compressor::codeToString(std::vector<int> in)	{
	std::string bi1,bi2,bi,bout;
	unsigned long i,x,y,z;
	for(i=0;i<in.size();i++)	{
		bi1 = std::bitset<12>(in[i]).to_string();
		bi2 = std::bitset<12>(in[i+1]).to_string();
		bi = bi1+bi2;
		x = std::bitset<8>(bi).to_ulong();
		bi = bi.substr(8,23);
		y = std::bitset<8>(bi).to_ulong();
		bi = bi.substr(8,23);
		z = std::bitset<8>(bi).to_ulong();
		bi = bi.substr(8,23);
		bi = "";
		bout += static_cast<int8_t>(x);
		bout += static_cast<int8_t>(y);
		bout += static_cast<int8_t>(z);
	}
	if(i == (in.size() -1))	{
		std::cout << bout << std::endl;
		return bout;	
	}
	else	{
		bi1 = std::bitset<12>(in[(in.size() -1)]).to_string();
		bi2 = std::bitset<12>(0).to_string();
		bi = bi1+bi2;
		x = std::bitset<8>(bi).to_ulong();
		bi = bi.substr(8,23);
		y = std::bitset<8>(bi).to_ulong();
		z = 255;
		bout += static_cast<int8_t>(x);
		bout += static_cast<int8_t>(y);
		bout += static_cast<int8_t>(z);
		std::cout << bout << std::endl;
		return bout;
	}
}

/**
 * 
 * DECODING
 * 
 **/

std::vector<int> Compressor::stringToCode(std::string in)	{
	std::string bi,bi1,bi2,bi3;
	std::vector<int> out;
	unsigned long x,y;
	for(int i=0;i<in.size();i+=3)	{
		bi1 = std::bitset<8>(in[i]).to_string();
		bi2 = std::bitset<8>(in[i+1]).to_string();
		bi1 = bi1 + bi2.substr(0,3);
		bi3 = std::bitset<8>(in[i+2]).to_string();
		bi3 = bi2.substr(4,7) + bi3;
		bi = bi1 + bi3;
		x = std::bitset<12>(bi).to_ulong();
		bi = bi.substr(12,23);
		y = std::bitset<12>(bi).to_ulong();
		out.push_back(x);
		std::cout << x << std::endl;
		out.push_back(y);
		std::cout << y << std::endl;
		return out;
	}
}

std::string Compressor::decode(std::vector<int> in)		{

	std::unordered_map<int,std::string> LUtable;
	std::string out {""};
	std::string insert {""};

	for(int i=0;i <= 255;i++){
		insert += char(i);
		LUtable[i] = insert;
		insert = "";
	}
	int old{0}, n{0};
    std::string res {""}, curr {""};
    int index {256};
	
	old = in[0];
    curr += res[0];
    out += res;
    res = LUtable[old];
	
	for (int i=0;i<in.size()-1;i++) {
		
		n = in[i+1];
		
		if(LUtable.find(n) == LUtable.end())	{
			res = LUtable[old];
            res = res + curr;

		}
		else	{
			res = LUtable[old];
		}
		out += res;
		curr = "";
        curr += res[0];
        LUtable[index] = LUtable[old] + curr;
        index++;
        old = n;
	}
	return out;
}

std::string Compressor::compress(std::string in)	{
	return codeToString(encode(in));
}

std::string Compressor::decompress(std::string in)	{
	return decode(stringToCode(in));
}
