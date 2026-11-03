#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct File
{
    string name;
    string extension;
    uint64_t byteSize;

    string getFullFilename()
    {
        return name + "." + extension;
    }
};

struct Directory
{
    string name;
    vector<File *> files;
    vector<Directory *> dirs;
    Directory *parent = nullptr;


    bool Exists(string n)
    {
        for (size_t i = 0; i < files.size(); i++)
        {
            if (files[i] -> getFullFilename() == n) return true;
        }
        for (size_t i = 0; i < dirs.size(); i++)
        {
            if (dirs[i] -> name == n) return true;
        }

        return false;
    }

    uint64_t getDirectorySize()
    {
        uint64_t sz = 0;
        for (size_t i = 0; i < files.size(); i++)
        {
            sz += files[i]->byteSize;
        }
        for (size_t i = 0; i < dirs.size(); i++)
        {
            sz += dirs[i] -> getDirectorySize();
        }

        return sz;
    }

    bool moveFile(string filename, Directory &to)
    {
        if (to.Exists(filename)) return false;

        for (size_t i = 0; i < files.size(); i++)
        {
            if (filename == files[i]-> getFullFilename())
            {
                File *file = files[i];

                files.erase(files.begin() + i);
                to.files.push_back(file);

                return true;
            }
        }
        return false;
    }

    void printDir(int space)
    {
        for (size_t i = 0; i < space; i++) cout << " ";

        std::cout << "d: " << name << '\n';

        for (size_t i = 0; i < files.size(); i++)
        {
            for (int j = 0; j < space+1; j++) cout << " ";
            cout << "f: " << files[i] -> getFullFilename() << '\n';
        }
        for (size_t i = 0; i < dirs.size(); i++) dirs[i] -> printDir(space + 1);
    }

    File* findFile(string filename)
    {
        for (size_t i = 0; i < files.size(); i++)
        {
            if (filename == files[i] -> getFullFilename())
            {
                return files[i];
            }
        }
        for (size_t i = 0; i < dirs.size(); i++)
        {
            File* f = dirs[i] -> findFile(filename);
            if (f != nullptr) return f;
        }

        return nullptr;
    }

    Directory* findDir(string dirname)
    {
        for (size_t i = 0; i < dirs.size(); i++)
        {
            if (dirname == dirs[i] -> name)
            {
                return dirs[i];
            }
        }
        for (size_t i = 0; i < dirs.size(); i++)
        {
            Directory* dn = dirs[i] -> findDir(dirname);
            if (dn != nullptr) return dn;
        }

        return nullptr;
    }

    void Up()
    {
        if (parent == nullptr)  cout << "/(root)";
        else cout << parent -> name;
    }

    bool setFileName (File *f, string n, string ext)
    {
        string x = n + "." + ext;

        for (size_t i = 0; i < files.size(); i++)
        {
            if (files[i] != f && files[i] -> getFullFilename() == x)
            {
                cout << "A file with that name already exists" << "\n";
                return false;
            }
        }

        f -> name = n;
        f -> extension = ext;

        return true;
    }

    void setDirName(Directory *dirr, string n)
    {
        for (size_t i=0; i< dirs.size(); i++)
        {
            if (dirs[i] != dirr && dirs[i] -> name == n)
            {
                cout << "A directory with that name already exists" << "\n";
                return;
            }
        }

        dirr -> name = n;

    }

};


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

    root.setFileName(&a, "notes", "txt");
    root.setFileName(&b, "image", "png");
    root.setFileName(&x, "audio", "mp3");
    child.setFileName(&c, "code", "cpp");

    a.byteSize = 100;
    b.byteSize = 200;
    c.byteSize = 300;

    root.setFileName(&x, "notes", "txt");
    root. setDirName(&child1, "documents");

    root.printDir(0);

    child.Up();
    cout << '\n';
    root.Up();
    cout << '\n';

    cout << (root.findFile("code.cpp") != nullptr) << '\n';
    cout << (root.findDir("cpp") != nullptr) << '\n';

    cout << root.getDirectorySize() << '\n';

    cout << child.moveFile("code.cpp", child1) << '\n';
    cout << child.moveFile("code.cpp", child1) << '\n';

    root.printDir(0);

    return 0;
}
