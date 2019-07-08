/*
 * Tests for minizip Conan package
 * UTF-8
 * Dmitriy Vetutnev, Odant 2019
*/


#include <mz.h>
#include <mz_strm.h>
#include <mz_zip.h>
#include <mz_zip_rw.h>

#include <Windows.h>

#include <string>
#include <fstream>
#include <iostream>


#pragma warning(push)
#pragma warning(disable:4309)


const std::string fileNameUTF8{
    static_cast<char>(0xD0), static_cast<char>(0xA4),
    static_cast<char>(0xD0), static_cast<char>(0xB0),
    static_cast<char>(0xD0), static_cast<char>(0xB9),
    static_cast<char>(0xD0), static_cast<char>(0xBB),
    '.', 't', 'x', 't'
};
const std::wstring fileNameUTF16{
    static_cast<wchar_t>(0x0424),
    static_cast<wchar_t>(0x0430),
    static_cast<wchar_t>(0x0439),
    static_cast<wchar_t>(0x043B),
    L'.', L't', L'x', L't'
};

const std::string archiveNameUTF8{
    static_cast<char>(0xD0), static_cast<char>(0x90),
    static_cast<char>(0xD1), static_cast<char>(0x80),
    static_cast<char>(0xD1), static_cast<char>(0x85),
    static_cast<char>(0xD0), static_cast<char>(0xB8),
    static_cast<char>(0xD0), static_cast<char>(0xB2),
    '.', 'z', 'i', 'p'
};
const std::wstring archiveNameUTF16{
    static_cast<wchar_t>(0x0410),
    static_cast<wchar_t>(0x0440),
    static_cast<wchar_t>(0x0445),
    static_cast<wchar_t>(0x0438),
    static_cast<wchar_t>(0x0432),
    L'.', L'z', L'i', L'p'
};


int main(int, char**) {
    std::ofstream file{fileNameUTF16};
    file << "Daaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaata";
    file.close();

    std::cout << "Create archive..." << std::endl;
    int32_t err = MZ_OK;
    void* writer = nullptr;
    ::mz_zip_writer_create(&writer);
    ::mz_zip_writer_set_compress_method(writer, MZ_COMPRESS_METHOD_DEFLATE);
    ::mz_zip_writer_set_compress_level(writer, MZ_COMPRESS_LEVEL_BEST);
    err = ::mz_zip_writer_open_file(writer, archiveNameUTF8.c_str(), 0, 0);
    if (err != MZ_OK) {
        std::cout << "Failed 'mz_zip_writer_open_file()', err: " << err << std::endl;
        return EXIT_FAILURE;
    }
    err = ::mz_zip_writer_add_path(writer, fileNameUTF8.c_str(), nullptr, 0, 1);
    if (err != MZ_OK) {
        std::cout << "Failed 'mz_zip_writer_add_path()', err: " << err << std::endl;
        return EXIT_FAILURE;
    }
    err = ::mz_zip_writer_close(writer);
    if (err != MZ_OK) {
        std::cout << "Failed 'mz_zip_writer_close()', err: " << err << std::endl;
        return EXIT_FAILURE;
    }
    ::mz_zip_writer_delete(&writer);

    std::cout << "Check exists archive..." << std::endl;
    if (::GetFileAttributesW(archiveNameUTF16.c_str()) == INVALID_FILE_ATTRIBUTES) {
        std::cout << "Archive not exists!" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Archive exists,  remove source file..." << std::endl;
    if (!::DeleteFileW(fileNameUTF16.c_str())) {
        std::cout << "Can`t remove source file!" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Unpack archive..." << std::endl;
    void* reader = nullptr;
    ::mz_zip_reader_create(&reader);
    err = ::mz_zip_reader_open_file(reader, archiveNameUTF8.c_str());
    if (err != MZ_OK) {
        std::cout << "Failed 'mz_zip_reader_open_file()', err: " << err << std::endl;
        return EXIT_FAILURE;
    }
    err = ::mz_zip_reader_save_all(reader, ".");
    if (err != MZ_OK) {
        std::cout << "Failed 'mz_zip_reader_save_all()', err: " << err << std::endl;
        return EXIT_FAILURE;
    }
    err = ::mz_zip_reader_close(reader);
    if (err != MZ_OK) {
        std::cout << "Failed 'mz_zip_reader_close()', err: " << err << std::endl;
        return EXIT_FAILURE;
    }
    ::mz_zip_reader_delete(&reader);

    std::cout << "Archive unpacked, check exists file..." << std::endl;
    if (::GetFileAttributesW(fileNameUTF16.c_str()) == INVALID_FILE_ATTRIBUTES) {
        std::cout << "File not exists!" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Ok" << std::endl;
    return EXIT_SUCCESS;
}
