#include <string>
#include <fstream>
#include <json.hpp>
#include "translatons.h"
#include <vector>


using namespace std;
using json = nlohmann::json;

static string language = "english";

vector<string> getAvailableLanguages() {
    return {"english", "srpski"};
}

int getLanguageIndex() {
    auto langs = getAvailableLanguages();
    for (size_t i = 0; i < langs.size(); ++i) {
        if (langs[i] == language) return static_cast<int>(i);
    }
    return 0;
}


string getCurrentLanguage() {
    return language;
}

static unordered_map<string, string> translations;

void loadLanguage(const string& lang) {
    ifstream file("assets/local/" + lang + ".json");
    if (!file.is_open()) return;

    json j;
    file >> j;

    translations.clear();
    for (auto it = j.begin(); it != j.end(); ++it) {
        translations[it.key()] = it.value();
    }
    language = lang;
}

string translate(const string& key) {
    if (translations.count(key)) return translations[key];
    return key; // fallback
}

