// bool acquireLock(const std::string& lockFilePath) {
//     int lockFile = open(lockFilePath.c_str(), O_CREAT | O_WRONLY | O_EXCL, 0666);
//     return lockFile != -1;
// }

// void releaseLock(const std::string& lockFilePath) {
//     // Use remove instead of unlink
//     if (remove(lockFilePath.c_str()) != 0) {
//         //error
//     }
// }

