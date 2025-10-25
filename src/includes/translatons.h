#pragma once

#include <string>
#include <vector>


using namespace std;

vector<string> getAvailableLanguages();
string getCurrentLanguage();
int getLanguageIndex(); 
void loadLanguage(const string& lang);
string translate(const string& key);