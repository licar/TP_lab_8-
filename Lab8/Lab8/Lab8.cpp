// Lab8.cpp: определяет точку входа для консольного приложения.
//
#pragma once

#include "stdafx.h"
#include "Lab8.h"

#pragma warning (disable : 4996)


FILE *OpenFile(string const &fileName, bool forRead)
{
	FILE *file;

	if (forRead)
	{
		file = fopen(fileName.c_str(), "r");
	}
	else
	{
		file = fopen(fileName.c_str(), "w");
	}

	if (!file)
	{
		fclose(file);
		return nullptr;
	}

	return file;
}

void ReadFromFile(FILE *fileInput, vector<string> &vectCode)
{
	char charSign;

	for (size_t i = 0; !feof(fileInput); ++i)
	{
		vectCode.resize(i + 1);
		charSign = fgetc(fileInput);
		while (!feof(fileInput) && (charSign != '\n'))
		{
			vectCode.at(i).push_back(charSign);
			charSign = fgetc(fileInput);
		}
	}
}

void fillDict(map<string, int> &dictionary, string const &String)
{
	const char space = ' ';
	const char point = '.';
	const char bracket = '(';

	string buf;
	string().swap(buf);

	map<string, int>::iterator it;

	bool isWord = false; bool addWord = false;
	size_t i = 0;

	while (i < String.length()){

		if (!isWord){

			if ((String.at(i) != space) && (String.at(i) != point) && (String.at(i) != bracket)){
				isWord = !isWord;
				buf += String.at(i);
			}
		}

		else{
			if (i < (String.length() - 1)){

				if ((String.at(i) != space) && (String.at(i) != point) && (String.at(i) != bracket)){
					buf += String.at(i);
				}
				else{
					isWord = !isWord;
					addWord = !addWord;
				}
			}

			else{

				buf += String.at(i);
				isWord = !isWord;
				addWord = !addWord;
			}
		}

		if (addWord){

			it = dictionary.find(buf);
			if (it == dictionary.end()){
				dictionary[buf] = 1;
			}
			else{
				dictionary[buf]++;
			}
		}

		i++;
	}
}

void printPopular(map<string, int> dictionary){

	string key;
	int maxNum = 0;

	map<string, int>::iterator it = dictionary.begin();

	for (; it != dictionary.end(); it++){

		if (it->second > maxNum){
			key = it->first;
			maxNum = it->second;
		}
	}
	cout << "Most popular is " << key << " " << maxNum << " times" << endl;
};

void removeFromCode(vector<string> const & vectCode,
	vector<Removed> &vectWithoutComments, map<string, int> &dict)
{
	int a = 0;
	string Comm;
	Removed withoutComments;
	bool isQuote = false; 
	bool isComment = false;
	

	vector<string>::const_iterator itStr = vectCode.begin();

	for (; itStr != vectCode.end(); ++ itStr)
	{
		size_t i = 0;

		if (itStr->length() == 0)
		{
			withoutComments.str = *itStr;
			withoutComments.isWrite = true;
			vectWithoutComments.push_back(withoutComments);
			withoutComments.str.clear();
		}

		else
		{
			while (i <itStr->length()){

				if (!isComment){

					if (!isQuote){

						if (itStr->at(i) == '\"'){
							isQuote = !isQuote;
							withoutComments.str.push_back(itStr->at(i));
						}

						else if (itStr->at(i) == '/'){

							if (itStr->at(i + 1) == '/'){
								i += 2;
								while (i < itStr->length()){
									Comm.push_back(itStr->at(i));
									i++;
								}
							}

							else if (itStr->at(i + 1) == '*'){
								isComment = !isComment;
								i++;
							}
						}

						else{
							withoutComments.str.push_back(itStr->at(i));
						}
					}

					else{

						if (itStr->at(i) == '\"'){
							isQuote = !isQuote;
						}
						withoutComments.str.push_back(itStr->at(i));
					}
				}

				else{

					if (itStr->at(i) == '*'){

						if (itStr->at(i + 1) == '/'){
							i++;
							isComment = !isComment;
						}
						else{
							Comm.push_back(itStr->at(i));
						}
					}

				}
				i++;
			}
			i = 0;

			if (withoutComments.str.length() == 0)
			{
				withoutComments.isWrite = false;
			}
			else
			{
				withoutComments.isWrite = true;
			}
			vectWithoutComments.push_back(withoutComments);
			withoutComments.str.clear();
			fillDict(dict, Comm);

			Comm.clear();
		}
	}
};

bool unitTest(vector<string>  & testInData, vector<string> const & testOutData)
{
	vector<Removed> testInResult;

	map<string, int> dictionary;
	removeFromCode(testInData, testInResult, dictionary);

	for(size_t i = 0; i < testInData.size(); ++i)
	{
		if (testInResult[i].str != testOutData[i]){
			cout << "Error";
			return false;
		}
		++i;
	}

	cout << "All is ok";
	return true;
}

void fillTests(vector<string> &testInData, vector<string> &testOutData)
{
	testInData.push_back("alert(\"Hello /* abc */ \");  // Comment");
	testInData.push_back("alert(\" // Test \");  // Comment");
	testInData.push_back("// 12345 \" qwerty \"   ");
	testInData.push_back("  test();  // abc");
	testInData.push_back("/* 12345  */");
	testInData.push_back("  \" /* \"  */  \" ");
	testInData.push_back("  a = 2; /*  line 1");
	testInData.push_back("	line 2	");
	testInData.push_back("	line 3  */ rest();");

	testOutData.push_back("alert(\"Hello /* abc */ \");  ");
	testOutData.push_back("alert(\" // Test \");  ");
	testOutData.push_back("");
	testOutData.push_back("  test();  ");
	testOutData.push_back("");
	testOutData.push_back("  \" /* \"  */  \" ");
	testOutData.push_back("  a = 2; ");
	testOutData.push_back("");
	testOutData.push_back(" rest();");
};

void WriteResult(vector<Removed> &vectWithoutComments, FILE *fileOut)
{
	for (auto str : vectWithoutComments)
	{
		if (str.isWrite)
		{
			fputs(str.str.c_str(), fileOut);
			fputc('\n', fileOut);
		}
	}
};

int _tmain()
{

	vector<string> testInData;
	vector<string> testOutData;

	fillTests(testInData, testOutData);
	map<string, int> dict;

	if (!unitTest(testInData, testOutData))
	{
		return 1;
	};

	string strInput = "input.js";
	string strOutput = "output.js";
	
	FILE *fileInput = OpenFile(strInput, true);
	FILE *fileOutput = OpenFile(strOutput, false);

	if (!fileInput || !fileOutput) return 0;
	vector<string> vectCode;
	ReadFromFile(fileInput, vectCode);
	vector<Removed> vectWithoutComments;
	removeFromCode(vectCode, vectWithoutComments, dict);

	printPopular(dict);

	WriteResult(vectWithoutComments, fileOutput);

	fclose(fileInput);
	fclose(fileOutput);
	return 0;
}


