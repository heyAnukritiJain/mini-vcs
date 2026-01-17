#include <iostream>
#include <filesystem>
#include <chrono>
#include <mysql/mysql.h>

namespace fs = std::filesystem;

/* ---------- CONFIG (EDIT ONLY THESE IF NEEDED) ---------- */
const char* DB_HOST = "localhost";
const char* DB_USER = "root";
const char* DB_PASS = "newpassword";
const char* DB_NAME = "mini_vcs";
/* -------------------------------------------------------- */

// Generate unique version name
std::string generateVersionName() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    return "version_" + std::to_string(time);
}

// Save metadata to MySQL
void saveToDatabase(const std::string& version,
                    const std::string& message) {

    MYSQL* conn = mysql_init(nullptr);

    if (!mysql_real_connect(
            conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, nullptr, 0)) {
        std::cerr << "DB connection failed: "
                  << mysql_error(conn) << std::endl;
        return;
    }

    std::string query =
        "INSERT INTO versions (version_name, message) VALUES ('" +
        version + "', '" + message + "')";

    if (mysql_query(conn, query.c_str())) {
        std::cerr << "Insert failed: "
                  << mysql_error(conn) << std::endl;
    }

    mysql_close(conn);
}

void printVersionHistory() {
    MYSQL* conn = mysql_init(nullptr);

    if (!mysql_real_connect(
            conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, nullptr, 0)) {
        std::cerr << "DB connection failed\n";
        return;
    }

    const char* query =
        "SELECT id, version_name, message, created_at FROM versions ORDER BY id";

    if (mysql_query(conn, query)) {
        std::cerr << "Query failed\n";
        mysql_close(conn);
        return;
    }

    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;

    std::cout << "\n--- Version History ---\n";

    while ((row = mysql_fetch_row(result))) {
        std::cout
            << "[" << row[0] << "] "
            << row[1] << " | "
            << row[2] << " | "
            << row[3] << std::endl;
    }

    mysql_free_result(result);
    mysql_close(conn);
}

#include <fstream>

void exportHistoryToJson() {
    MYSQL* conn = mysql_init(nullptr);
    mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, nullptr, 0);

    mysql_query(conn,
        "SELECT id, version_name, message, created_at FROM versions ORDER BY id");

    MYSQL_RES* result = mysql_store_result(conn);
    MYSQL_ROW row;

    std::ofstream file("../web/versions.json");
    file << "[\n";

    bool first = true;
    while ((row = mysql_fetch_row(result))) {
        if (!first) file << ",\n";
        first = false;

        file << "  {\n";
        file << "    \"id\": " << row[0] << ",\n";
        file << "    \"version\": \"" << row[1] << "\",\n";
        file << "    \"message\": \"" << row[2] << "\",\n";
        file << "    \"created_at\": \"" << row[3] << "\"\n";
        file << "  }";
    }

    file << "\n]";
    file.close();

    mysql_free_result(result);
    mysql_close(conn);
}

// Create snapshot
void createSnapshot(const std::string& sourcePath,
                    const std::string& versionsPath,
                    const std::string& message) {

    if (!fs::exists(sourcePath)) {
        std::cout << "Source folder does not exist.\n";
        return;
    }

    std::string versionName = generateVersionName();
    fs::path destination = fs::path(versionsPath) / versionName;

    fs::create_directories(destination);
    fs::copy(sourcePath, destination,
             fs::copy_options::recursive);

    saveToDatabase(versionName, message);

    std::cout << "Snapshot created: " << versionName << std::endl;
}

int main() {
    std::string sourceFolder = "../test_project";
    std::string versionsFolder = "../data/versions";

    std::string commitMessage;
    std::cout << "Enter commit message: ";
    std::getline(std::cin, commitMessage);

    createSnapshot(sourceFolder, versionsFolder, commitMessage);

    printVersionHistory();
    exportHistoryToJson();

    return 0;
}

