#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

// Homework for 21.10.2026
// To complete the homework, it MUST be uploaded to your oop repo on Github!!

// 0. Enforce that no file/directory with the same names can be created in the same dir.
// 1. When printing the directory, print the contents of all child directories!!!
// 2. Implement 'Up' function that return the parent directory of the current directory
//    If no parent is available, print '/(root)'
// 3. Implement "Find" that searches for a file or directory inside a specific directory.
//    The file/directory can be nested inside a subdirectory
// 4. DirectorySize function includes the size of child directories DONE

using namespace std;

struct File
{
    string name;
    string extension;
    uint64_t byteSize;
};

string getFullFilename(File &f) { return f.name + '.' + f.extension; }

struct Directory
{
    string name;
    vector<File *> files;
    vector<Directory *> dirs;
    Directory *parent = nullptr;
};

bool Exists(Directory *d, string n)
{
    for (size_t i = 0; i < d -> files.size(); i++)
    {
        if (getFullFilename(*d -> files[i]) == n) return true;
    }
    for (size_t i = 0; i < d -> dirs.size(); i++)
    {
        if (d -> dirs[i] -> name == n) return true;
    }

    return false;
}

uint64_t getDirectorySize(Directory &d)
{
    uint64_t sz = 0;
    for (size_t i = 0; i < d.files.size(); i++) {
        sz += d.files[i]->byteSize;
    }
    for (size_t i = 0; i < d.dirs.size(); i++)
    {
        sz += getDirectorySize(*d.dirs[i]);
    }

    return sz;
}

bool moveFile(const string &filename, Directory &from, Directory &to)
{
    if (Exists(&to, filename)) return false;

    File *file = nullptr;
    for (size_t i = 0; i < from.files.size(); i++) {
        if (filename == getFullFilename(*from.files[i])) {
            file = from.files[i];
            for (size_t j = i; j < from.files.size() - 1; j++) {
                from.files[j] = from.files[j + 1];
            }
            from.files.pop_back();

            to.files.push_back(file);
            return true;
        }
    }
    return false;
}

void printDir(Directory &d, int space)
{
    for (int i = 0; i < space; i++) std::cout << " ";
    std::cout << "d: " << d.name << '\n';
    for (size_t i = 0; i < d.files.size(); i++) {
        for (int i = 0; i < space+1; i++) std::cout << " ";
        std::cout << "f: " << getFullFilename(*d.files[i]) << '\n';
    }
    for (size_t i = 0; i < d.dirs.size(); i++) {
        printDir(*d.dirs[i], space + 1);
    }
    //std::cout << "Size: " << getDirectorySize(d) << " B\n";
}

File* findFile(string filename, Directory &d)
{
    for (size_t i = 0; i < d.files.size(); i++)
    {
        if (filename == getFullFilename(*d.files[i]))
        {
            return d.files[i];
        }
    }
    for (size_t i = 0; i < d.dirs.size(); i++)
    {
        File* f = findFile(filename, *d.dirs[i]);
        if (f != nullptr) return f;
    }

    return nullptr;
}

Directory* findDir(string dirname, Directory &d)
{
    for (size_t i = 0; i < d.dirs.size(); i++)
    {
        if (dirname == d.dirs[i] -> name)
        {
            return d.dirs[i];
        }
    }
    for (size_t i = 0; i < d.dirs.size(); i++)
    {
        Directory* dn = findDir(dirname, *d.dirs[i]);
        if (dn != nullptr) return dn;
    }

    return nullptr;
}

void Up(Directory *d)
{
    if (d -> parent == nullptr)  cout << "/(root)";
    else cout << d -> parent -> name;
}

void setFileName(Directory *d, File *f, string n, string ext)
{
    string x = n + "." + ext;
    if (Exists(d, x)) cout << "A file with that name already exists" << "\n";
    else
    {
        f -> name = n;
        f -> extension = ext;
    }
}
void setDirName(Directory *d, Directory *dirr, string n)
{
    if (Exists(d, n)) cout << "A directory with that name already exists" << "\n";
    else dirr -> name = n;
}

int main()
{
    Directory root, child, child1;
    root.name = "documents";
    child.name = "projects";
    child1.name = "cpp";

    child.parent = &root;
    child1.parent = &child;

    root.dirs.push_back(&child);
    child.dirs.push_back(&child1);

    File a, b, c, x;

    root.files.push_back(&a);
    root.files.push_back(&b);
    root.files.push_back(&x);
    child.files.push_back(&c);

    setFileName(&root, &a, "notes", "txt");
    setFileName(&root, &b, "image", "png");
    setFileName(&root, &x, "audio", "mp3");
    setFileName(&child, &c, "code", "cpp");

    a.byteSize = 100;
    b.byteSize = 200;
    c.byteSize = 300;

    setFileName(&root, &x, "notes", "txt");
    setDirName(&root, &child1, "documents");

    printDir(root, 0);

    Up(&child);
    cout << '\n';
    Up(&root);
    cout << '\n';

    cout << (findFile("code.cpp", root) != nullptr) << '\n';
    cout << (findDir("cpp", root) != nullptr) << '\n';

    cout << getDirectorySize(root) << '\n';

    cout << moveFile("code.cpp", child, child1) << '\n';
    cout << moveFile("code.cpp", child, child1) << '\n';

    printDir(root, 0);

    return 0;
}
