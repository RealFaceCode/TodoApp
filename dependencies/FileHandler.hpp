#ifndef FILE_HANDLER_HPP
#define FILE_HANDLER_HPP

#include <iostream>
#include <memory>
#include <fstream>
#include <cstring>
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace FileHandler
{
    [[maybe_unused]] std::size_t GetFileSize(const fs::path& path)
    {
        if(!fs::exists(path))
        {
            return 0;
        }

        std::ifstream inStream(path);
        inStream.seekg(0, std::ios::end);
        std::size_t fileSize = inStream.tellg();
        inStream.seekg(0, std::ios::beg);

        return fileSize;
    }

    [[maybe_unused]] std::size_t GetFileSize(std::ifstream& inStream)
    {
        inStream.seekg(0, std::ios::end);
        std::size_t fileSize = inStream.tellg();
        inStream.seekg(0, std::ios::beg);

        return fileSize;
    }

    [[maybe_unused]] bool CreateFile(const std::filesystem::path& path)
    {
        if(fs::exists(path)) return true;

        if(!fs::exists(path.parent_path()) && !fs::create_directories(path.parent_path()))
        {
            std::cerr << "Failed to create directories: " << path.parent_path() << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        std::ofstream create;
        create.open(path);
        if(!create.is_open())
        {
            std::cerr << "Failed to create file: " << path.filename() << " in: " << path.parent_path() << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        create.close();
        return true;
    }

    [[maybe_unused]] bool DeleteFile(const fs::path& path)
    {
        if(!fs::exists(path) || !fs::remove(path))
        {
            std::cerr << "Failed to delete file: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }
        return true;
    }

    [[maybe_unused]] bool RenameFile(const fs::path& path, const std::string& newFileName)
    {
        if(!fs::exists(path))
        {
            std::cerr << "Failed to rename file: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        fs::path newPath = path.parent_path();
        newPath += "/" + newFileName;
        fs::rename(path, newPath);

        return true;
    }

    [[maybe_unused]] bool CopyFile(const fs::path& src, const fs::path& dst)
    {
        if(!fs::exists(src))
        {
            std::cerr << "Failed to copy file: " << src << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        fs::path dest;
        if(dst.has_extension())
        {
            dest = dst.parent_path();
            dest += "/";
        }
        else
            dest = dst;
        if(!dest.string().ends_with('/') || !dest.string().ends_with('\\'))
            dest += "/";
        
        if(!fs::exists(dest))
        {
            fs::create_directories(dest);
        }

        dest += src.filename();

        if(fs::exists(dest) && !DeleteFile(dest))
            return false;

        if(!fs::copy_file(src, dest))
        {
            std::cerr << "Failed to copy file: " << src << " : " << std::strerror(errno) << std::endl;
            return false;
        }
        return true;
    }

    [[maybe_unused]] bool MoveFile(const fs::path& src, const fs::path& dst)
    {
        if(!fs::exists(src))
        {
            std::cerr << "Failed to move file: " << src << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        fs::path dest;
        if(dst.has_extension())
        {
            dest = dst.parent_path();
            dest += "/";
        }
        else
            dest = dst;
        if(!dest.string().ends_with('/') || !dest.string().ends_with('\\'))
            dest += "/";
        
        if(!fs::exists(dest))
        {
            fs::create_directories(dest);
        }
        
        dest += src.filename();

        if(fs::exists(dest) && !DeleteFile(dest))
            return false;

        if(!fs::copy_file(src, dest))
        {
            std::cerr << "Failed to move file: " << src << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        if(!DeleteFile(src))
            return false;
        return true;
    }

    [[maybe_unused]] bool WriteToFile(const std::filesystem::path& path, const std::string& buffer,
                                      const std::ios_base::openmode openMode = std::ios::out)
    {
        if(!CreateFile(path))
            return false;

        std::ofstream oStream(path, openMode);
        if(!oStream.is_open())
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        oStream << buffer;
        if(oStream.fail())
        {
            std::cerr << "Failed to write to file: " << path << " : " << std::strerror(errno) << std::endl;
            oStream.close();
            return false;
        }

        oStream.close();
        return true;
    }

    [[maybe_unused]] bool ReadFromFile(const fs::path& path, std::string& buffer)
    {
        if(!fs::exists(path))
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        std::ifstream inStream(path);
        if(!inStream.is_open())
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        inStream >> buffer;
        if(inStream.fail() || buffer.empty())
        {
            std::cerr << "Failed to read: " << path << " : " << std::strerror(errno) << std::endl;
            inStream.close();
            return false;
        }

        inStream.close();
        return true;
    }

    [[maybe_unused]] bool WriteBinaryToFile(const std::filesystem::path& path, const std::vector<uint8_t>& buffer,
                                            const std::ios_base::openmode openMode = std::ios::binary)
    {
        if(!CreateFile(path))
            return false;

        std::ofstream oStream(path, openMode);
        if(!oStream.is_open())
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        oStream.write(reinterpret_cast<const char *>(buffer.data()), static_cast<std::streamsize>(buffer.size()));
        if(oStream.fail())
        {
            std::cerr << "Failed to write to file: " << path << " : " << std::strerror(errno) << std::endl;
            oStream.close();
            return false;
        }

        oStream.close();
        return true;
    }

    [[maybe_unused]] bool WriteBinaryToFile(const std::filesystem::path& path, const void* buffer,
                                            const std::streamsize& streamSize, const std::ios_base::openmode openMode = std::ios::binary)
    {
        if(!buffer)
        {
            std::cerr << "Failed to write: " << path << " : buffer == nullptr" << std::endl;
            return false;
        }

        if(!CreateFile(path))
            return false;

        std::ofstream oStream(path, openMode);
        if(!oStream.is_open())
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        oStream.write(static_cast<const char *>(buffer), streamSize);
        if(oStream.fail())
        {
            std::cerr << "Failed to write to file: " << path << " : " << std::strerror(errno) << std::endl;
            oStream.close();
            return false;
        }

        oStream.close();
        return true;
    }

    [[maybe_unused]] bool ReadBinaryFromFile(const std::filesystem::path& path, std::vector<uint8_t>& buffer)
    {
        if(!fs::exists(path))
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        std::ifstream inStream(path, std::ios::binary);
        if(!inStream.is_open())
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        std::size_t fileSize = GetFileSize(inStream);

        buffer.resize(fileSize);

        inStream.read(reinterpret_cast<char *>(buffer.data()), static_cast<std::streamsize>(fileSize));
        if(inStream.fail() || buffer.empty())
        {
            std::cerr << "Failed to read: " << path << " : " << std::strerror(errno) << std::endl;
            inStream.close();
            return false;
        }

        inStream.close();
        return true;
    }

    [[maybe_unused]] bool ReadBinaryFromFile(const std::filesystem::path& path, void* buffer, const std::streamsize& streamSize)
    {
        if(!buffer)
        {
            std::cerr << "Failed to read: " << path << " : buffer == nullptr" << std::endl;
            return false;
        }

        if(!fs::exists(path))
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        std::ifstream inStream(path, std::ios::binary);
        if(!inStream.is_open())
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        inStream.seekg(0, std::ios::end);
        std::streampos fileSize = inStream.tellg();
        inStream.seekg(0, std::ios::beg);

        if(streamSize > fileSize)
        {
            std::cerr << "Failed to read: " << path << " : "
            << "stream size[" << streamSize << "] > fileSize[" << fileSize << "]" << std::endl;
            return false;
        }

        inStream.read(reinterpret_cast<char *>(buffer), streamSize);
        if(inStream.fail())
        {
            std::cerr << "Failed to read: " << path << " : " << std::strerror(errno) << std::endl;
            inStream.close();
            return false;
        }

        inStream.close();
        return true;
    }

    [[maybe_unused]] bool SimpleEncryptFile(const fs::path& inputPath, const fs::path& outputPath, const std::string& key)
    {
        if(!fs::exists(inputPath))
        {
            std::cerr << "Failed to open: " << inputPath << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        std::ifstream inputFile(inputPath, std::ios::binary);
        std::ofstream outputFile(outputPath, std::ios::binary);

        std::size_t keyValue = std::hash<std::string>()(key);

        char buffer;
        while (inputFile.get(buffer))
        {
            buffer ^= keyValue;
            outputFile.put(buffer);
        }

        inputFile.close();
        outputFile.close();
        return true;
    }

    [[maybe_unused]] bool SimpleDecryptFile(const fs::path& inputPath, const fs::path& outputPath, const std::string& key)
    {
        return SimpleEncryptFile(inputPath, outputPath, key);
    }

    [[maybe_unused]] bool CompareFiles(const fs::path& firstPath, const fs::path& secondPath)
    {
        bool bothFilesExists = true;
        if(!fs::exists(firstPath))
        {
            std::cerr << "Failed to open: " << firstPath << " : " << std::strerror(errno) << std::endl;
            bothFilesExists =  false;
        }

        if(!fs::exists(secondPath))
        {
            std::cerr << "Failed to open: " << secondPath << " : " << std::strerror(errno) << std::endl;
            bothFilesExists =  false;
        }

        if(!bothFilesExists)
            return false;

        std::string f, s;
        if(!ReadFromFile(firstPath, f))
            return false;
        if(!ReadFromFile(secondPath, s))
            return false;

        if(f == s)
            return true;
        return false;
    }

    [[maybe_unused]] bool CreateBackupFromFile(const fs::path& path, const bool& dontOverride = false)
    {
        if(!fs::exists(path))
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        std::vector<uint8_t> buffer;

        if(!ReadBinaryFromFile(path, buffer))
            return false;

        fs::path bPath = path.parent_path();
        bPath += "/";
        bPath += path.filename();

        if(dontOverride)
        {
            bPath += ".";
            std::time_t currentTime = std::time(nullptr);
            std::tm* localTime = std::localtime(&currentTime);
            std::string year = std::to_string(localTime->tm_year + 1900);
            std::string month = std::to_string(localTime->tm_mon + 1);
            if(month.length() == 1) month.insert(month.begin(), '0');
            std::string day = std::to_string(localTime->tm_mday);
            if(day.length() == 1) day.insert(day.begin(), '0');
            std::string hour = std::to_string(localTime->tm_hour);
            if(hour.length() == 1) hour.insert(hour.begin(), '0');
            std::string minute = std::to_string(localTime->tm_min);
            if(minute.length() == 1) minute.insert(minute.begin(), '0');
            std::string second = std::to_string(localTime->tm_sec);
            if(second.length() == 1) second.insert(second.begin(), '0');
            std::string date = year + month + day + hour + minute + second;
            bPath += date;
        }

        bPath += ".bak";

        if(!WriteBinaryToFile(bPath, buffer))
            return false;
        return true;
    }

    [[maybe_unused]] bool CreateBackupFromFile(const fs::path& path, const fs::path& backupPath,
                                               const bool& dontOverride = false)
    {
        if(!fs::exists(path))
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        std::vector<uint8_t> buffer;

        if(!ReadBinaryFromFile(path, buffer))
            return false;

        fs::path bPath;
        if(backupPath.has_extension())
        {
            bPath = backupPath.parent_path();
            bPath += "/";
        }
        else
            bPath = backupPath;
        if(!bPath.string().ends_with('/') || !bPath.string().ends_with('\\'))
            bPath += "/";
        bPath += path.filename();

        if(dontOverride)
        {
            bPath += ".";
            std::time_t currentTime = std::time(nullptr);
            std::tm* localTime = std::localtime(&currentTime);
            std::string year = std::to_string(localTime->tm_year + 1900);
            std::string month = std::to_string(localTime->tm_mon + 1);
            if(month.length() == 1) month.insert(month.begin(), '0');
            std::string day = std::to_string(localTime->tm_mday);
            if(day.length() == 1) day.insert(day.begin(), '0');
            std::string hour = std::to_string(localTime->tm_hour);
            if(hour.length() == 1) hour.insert(hour.begin(), '0');
            std::string minute = std::to_string(localTime->tm_min);
            if(minute.length() == 1) minute.insert(minute.begin(), '0');
            std::string second = std::to_string(localTime->tm_sec);
            if(second.length() == 1) second.insert(second.begin(), '0');
            std::string date = year + month + day + hour + minute + second;
            bPath += date;
        }

        bPath += ".bak";

        if(!WriteBinaryToFile(bPath, buffer))
            return false;
        return true;
    }

    [[maybe_unused]] bool GetLinesFromFile(const fs::path& path, std::vector<std::string>& buffer)
    {
        if(!fs::exists(path))
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        std::ifstream inStream(path);
        if (!inStream.is_open()) {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(inStream, line))
            buffer.emplace_back(line);
        return true;
    }

    [[maybe_unused]] bool GetLineFromFile(const fs::path& path, std::string& buffer, const std::size_t& line)
    {
        if(!fs::exists(path))
        {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        std::ifstream inStream(path);
        if (!inStream.is_open()) {
            std::cerr << "Failed to open: " << path << " : " << std::strerror(errno) << std::endl;
            return false;
        }

        std::size_t lineCount = 0;
        std::string l;
        while (lineCount <= line && std::getline(inStream, l))
            lineCount++;
        buffer = l;
        return true;
    }
}
#endif // FILE_HANDLER_HPP
