#include "miniz/miniz.h"
#include <string>
#include "zip_utils.h"

using namespace std;

void CreateZipFile(const std::string& zipFile) {

    std::string scene = "{\n  \"name\": \"" + zipFile + "\"\n}";

    mz_zip_archive zipArchive;
    memset(&zipArchive, 0, sizeof(zipArchive));

    if (!mz_zip_writer_init_file(&zipArchive, ("user_data/projects/" + zipFile + ".msps").c_str(), 0)) {
        printf("Failed to initialize zip writer");
    }

    mz_zip_writer_add_mem(&zipArchive, "scene.json", scene.c_str(), scene.size(), 0);

    if (!mz_zip_writer_finalize_archive(&zipArchive)) {
        mz_zip_writer_end(&zipArchive);
        printf("Failed to finalize zip archive");
    }

    mz_zip_writer_end(&zipArchive);
}