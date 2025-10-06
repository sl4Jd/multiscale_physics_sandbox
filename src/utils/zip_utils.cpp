#include <miniz.h>
#include <string>
#include <stdexcept>
#include "zip_utils.h"
#include <filesystem>

using namespace std;

void CreateProjectZipFile(const string& zipName) {

    string scene = "{\n  \"name\": \"" + zipName + "\"\n}";

    mz_zip_archive zipArchive;
    memset(&zipArchive, 0, sizeof(zipArchive));

    if (!mz_zip_writer_init_file(&zipArchive, ("user_data/projects/" + zipName + ".msps").c_str(), 0)) {
        throw runtime_error("Failed to initialize zip writer");
    }

    if (!mz_zip_writer_add_file(&zipArchive, "scene.json","user_data/projects/working/scene.json", nullptr, 0, 6)) {
        mz_zip_writer_end(&zipArchive);
        throw runtime_error("Failed to add scene.json to zip");
    }

    if (!mz_zip_writer_finalize_archive(&zipArchive)) {
        mz_zip_writer_end(&zipArchive);
        throw runtime_error("Failed to finalize zip archive");
    }

    mz_zip_writer_end(&zipArchive);
}
void UnzipFile(const std::string& zipName) {
    mz_zip_archive zipArchive;
    memset(&zipArchive, 0,sizeof(zipArchive));
    if (!mz_zip_reader_init_file(&zipArchive, ("user_data/projects/" + zipName + ".msps").c_str(), 0)) {
        throw runtime_error("Failed to open zip file");
    }
    int file_count = (int)mz_zip_reader_get_num_files(&zipArchive);

    // Extract each file
    for (int i = 0; i < file_count; i++) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zipArchive, i, &file_stat)) {
            mz_zip_reader_end(&zipArchive);
            throw runtime_error("Failed to get file stat for index" + to_string(i));
        }

        // Skip directories
        if (mz_zip_reader_is_file_a_directory(&zipArchive, i)) {
            continue;
        }

        // Build full output path
        string output_dir = "user_data/projects/working";
        string out_path = output_dir + "/" + file_stat.m_filename;

        //make subdirs
        filesystem::create_directories(std::filesystem::path(out_path).parent_path());
        // Extract the file
        if (!mz_zip_reader_extract_to_file(&zipArchive, i, out_path.c_str(), 0)) {
            mz_zip_reader_end(&zipArchive);
            throw runtime_error("Failed to extract file " + string(file_stat.m_filename));
        }
    }

    // Close the archive
    mz_zip_reader_end(&zipArchive);
}