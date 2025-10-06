#include <filesystem>
#include <string>
#include <iostream>
#include "zip_utils.h"
#include "save_project.h"

using namespace std;

void SaveProject(const string& name){
    try{
        CreateProjectZipFile(name);
    }
    catch (const runtime_error& e){
        cerr << "Error: " << e.what() << "\n";
    }
    filesystem::remove_all("user_data/projects/working");
}