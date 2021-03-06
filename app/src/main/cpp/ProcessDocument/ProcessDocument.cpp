//
// Created by kyle- on 2020-10-19.
//

#include "ProcessDocument.h"
#include <iostream>
#include <string>
#include <regex>
#include <android/log.h>
#include <map>


using std::string;
using namespace std;
using std::map;

string rawDocument;
vector<string> sentences;
int sentencesVectorSize;
map<string, int> wordFrequency;
map<string, float> sentenceRanking;
string stopwords[127];
float scoreCutOff;
int numberOfSentencesToDisplay;

ProcessDocument::ProcessDocument(string x)
{
    resetVariables();
    rawDocument = x;
    string stopwordsTemp[] = {"ourselves", "hers", "between", "yourself", "but", "again", "there", "about", "once", "during", "out", "very", "having", "with", "they", "own", "an", "be", "some", "for", "do", "its", "yours", "such", "into", "of", "most", "itself", "other", "off", "is", "s", "am", "or", "who", "as", "from", "him", "each", "the", "themselves", "until", "below", "are", "we", "these", "your", "his", "through", "don", "nor", "me", "were", "her", "more", "himself", "this", "down", "should", "our", "their", "while", "above", "both", "up", "to", "ours", "had", "she", "all", "no", "when", "at", "any", "before", "them", "same", "and", "been", "have", "in", "will", "on", "does", "yourselves", "then", "that", "because", "what", "over", "why", "so", "can", "did", "not", "now", "under", "he", "you", "herself", "has", "just", "where", "too", "only", "myself", "which", "those", "i", "after", "few", "whom", "t", "being", "if", "theirs", "my", "against", "a", "by", "doing", "it", "how", "further", "was", "here", "than"};
    for(int i=0; i<sizeof(stopwordsTemp)/sizeof(stopwordsTemp[0]);i++)
    {
        stopwords[i] = stopwordsTemp[i];
    }
    numberOfSentencesToDisplay = 6;
}

void ProcessDocument::resetVariables()
{
    sentencesVectorSize = 0;
    rawDocument = "";

}

void ProcessDocument::deallocateVariables()
{
    wordFrequency.clear();
    sentenceRanking.clear();
    sentences = vector<string>();
}

string ProcessDocument::mainLoop()
{
    removeSquareBrackets();
    removeNewLines();
    replace("&nbsp;"," ");
    replace("&amp;","&");
    removeWhiteSpace();
    breakDownSentences();
    editSentences();
    countWordFrequency();
    rankSentences();
    return summarizer();
}

void ProcessDocument::removeSquareBrackets()
{
    int i = 0;
    while(i < rawDocument.length())
    {
        if(rawDocument[i] == '[')
        {
            int j = i + 1;
            while(rawDocument[j] != ']') j++;
            j++;
            rawDocument.erase(i, j-i);
        }
        else i++;
    }
}

void ProcessDocument::replace(string original, string replacement)
{
    const string s = original;
    const string t = replacement;
    string::size_type n = 0;
    while ( ( n = rawDocument.find( s, n ) ) != string::npos )
    {
        rawDocument.replace( n, s.size(), t );
        n += t.size();
    }
}

void ProcessDocument::removeWhiteSpace()
{
    int i = 0;
    while(i < rawDocument.length()) {
        if(isspace(rawDocument[i])) {
            if(isspace(rawDocument[i+1]))
            {
                int j = i + 1;
                while (isspace(rawDocument[j])) j++;
                rawDocument.erase(i , j-1-i );
            }
        }
        i++;
    }
}

void ProcessDocument::removeNewLines(){
    rawDocument.erase(std::remove(rawDocument.begin(), rawDocument.end(), '\n'), rawDocument.end());
}

void ProcessDocument::breakDownSentences()
{
    int sentencesCount = 2;
    int i = 0;
    while(rawDocument[i] != '\0' && rawDocument[i+1] != '\0')
    {
        if(rawDocument[i] == '.' && isspace(rawDocument[i+1]))
        {
            if(i > 2 && rawDocument[i - 1] != 'o' && rawDocument[i - 2] != 'N') sentencesCount++;
        }
        i++;
    }
    string currentSentence = "";

    int sentenceVectorSize = 0;
    for(int i=0;i<rawDocument.length() - 1;i++)
    {
        if(rawDocument[i] == '.' && isspace(rawDocument[i+1]) )
        {
            sentenceVectorSize ++;
        }

    }

    sentences.clear();
    sentences.resize(sentenceVectorSize+1);

    int j = 0;
    for(int i=0;i<rawDocument.length() - 1;i++)
    {
        if(rawDocument[i] == '.' && isspace(rawDocument[i+1]) )
        {
            currentSentence += '.';
            sentences[j] = currentSentence;
            currentSentence = "";
            j++;
        }
        else
        {
            currentSentence += rawDocument[i];
        }

    }
    sentences[j] = currentSentence;
}

void ProcessDocument::editSentences()
{
    string unwantedSentences[] = {"Notable people with the surname include:", "This time is used in:", "may also refer to:"};
    for(int i=0;i<sentences.size(); i++)
    {
        for(int j=0;j<sizeof(unwantedSentences)/sizeof(unwantedSentences[0]);j++)
        {
            if(sentences[i].find(unwantedSentences[j]) != std::string::npos)
            {
                sentences.erase(sentences.begin() + i);
                i --;
                j = unwantedSentences->size();
            }
        }
    }

    // modifying sentences

    // - glossary of x
    if(sentences.size() <= 2 && (sentences[0].find("This page is a glossary of") != std::string::npos || sentences[0].find("is the name of some places in the United States of America:") != std::string::npos)) {
        sentences[0] = sentences[0].substr(0, sentences[0].length() - 1) + ", however, I'm not able to display it here. Please click the 'Open In Browser' button instead.";
    }
}

void ProcessDocument::countWordFrequency()
{
    string wordTemp = "";
    for(int i=0;i<rawDocument.length();i++)
    {
        if(isspace(rawDocument[i]) || rawDocument[i] == '.' || rawDocument[i] == ',' || rawDocument[i] == '(' || rawDocument[i] == ')' || rawDocument[i] == '"')
        {
            if(wordTemp != "")
            {
                if(wordFrequency[wordTemp])
                {
                    wordFrequency[wordTemp] += 1;
                } else{
                    wordFrequency[wordTemp] = 1;
                }
            }
            wordTemp = "";
        }
        else wordTemp += tolower(rawDocument[i]);
    }

}

void ProcessDocument::rankSentences()
{
    for(int i=0;i<sentences.size();i++)
    {
        for (auto const& pair: wordFrequency) {
            if(sentences[i].find(pair.first) != string::npos)
            {
                if(sentenceRanking[sentences[i]] )
                {

                    sentenceRanking[sentences[i]] += 1.0f;
                } else{
                    sentenceRanking[sentences[i]] = 1.0f;
                }
            }
        }
    }

    int longerSentenceBias;
    for(int i=0;i<sentences.size();i++)
    {
        longerSentenceBias = 1;
        for(char& c:sentences[i])
        {
            if(c == ',') longerSentenceBias = 2;
        }
        if(longerSentenceBias > 1) sentenceRanking[sentences[i]] /= (0.75 * longerSentenceBias);
    }

    int sentenceVectorSize = 0;
    for(int i=0;i<rawDocument.length() - 1;i++)
    {
        if(rawDocument[i] == '.' && isspace(rawDocument[i+1]) )
        {
            sentenceVectorSize ++;
        }

    }

    vector<int> tempScoreArray;
    tempScoreArray.resize(sentences.size());
    int k = 0;
    for (auto const& pair: sentenceRanking)
    {
        tempScoreArray[k] = pair.second;
        k++;
    }
    sort(tempScoreArray.begin(), tempScoreArray.end(), greater<int>());
    scoreCutOff = tempScoreArray[numberOfSentencesToDisplay];
}

string ProcessDocument::summarizer()
{
    string processedDocument = sentences[0];
    for(int i=1; i<sentences.size();i++)
    {
        if(sentenceRanking[sentences[i]] > (scoreCutOff)){
            processedDocument += sentences[i];
        }
    }
    deallocateVariables();
    return processedDocument;
}