#ifndef  _COMPRESSOR_
#define  _COMPRESSOR_

class Compressor	{
	public:
		std::string input_file{""};
		std::vector<int> encode(std::string);
		std::string	codeToString(std::vector<int>);
		std::vector<int> stringToCode(std::string);
		std::string decode(std::vector<int>);
		std::string compress(std::string);
		std::string decompress(std::string);
};

#endif