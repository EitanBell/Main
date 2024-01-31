/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Matan          	      *
*	Date: 30/04/23	                  *
*	Subject: Tree			   		  *
*				                	  *
**************************************/

#include <iostream>		// cout
#include <algorithm>    // for_each
#include <cstring>      // strcmp
#include <dirent.h>		// DIR, readdir
#include <sys/stat.h>

#include "tree.hpp"	// tree API

namespace ilrd
{

/*----------------------------------- Tree -----------------------------------*/

Tree::Tree(const std::string& dirName):
							  m_root(Tree::CompositeFolder(IsDirValid(dirName)))
{
	DIR *dir_p = opendir(dirName.c_str());
    dirent *entry;
    
    while ((entry = readdir(dir_p)))
    {
        if (std::strcmp(entry->d_name, "..") && std::strcmp(entry->d_name, "."))
        {
            m_root.Add(RecAddFS(dirName, entry->d_name));
        }
    }

    closedir(dir_p);
}

Tree::FileComponent *Tree::RecAddFS(const std::string& Path,
                                    					const std::string& Curr)
{
	DIR *dir_p;
	
    if (0 == (dir_p = opendir((Path + "/" + Curr).c_str())))
    {
        return new Tree::LeafFile(Curr);
    }

    Tree::CompositeFolder *newDir = new Tree::CompositeFolder(Curr);

    dirent *entry;
    
    while ((entry = readdir(dir_p)))
    {
        if (std::strcmp(entry->d_name, "..") && std::strcmp(entry->d_name, "."))
        {
            newDir->Add(RecAddFS(Path + "/" + Curr, entry->d_name));
        }
    }

    closedir(dir_p);

    return newDir;
}

void Tree::Print() const
{
	m_root.Print();
}

const std::string& Tree::IsDirValid(const std::string& dirName)
{
    struct stat sb;
    
    try
    {
        if (0 != (stat(dirName.c_str(), &sb)))
        {
            throw std::exception();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Failed to open directory" << std::endl;
		throw std::exception();
    }

    return dirName;
}

void Tree::PrintSpaces(size_t offset, const std::string& suffix)
{
	for (std::size_t i = 0; i < offset; ++i)
	{
		std::cout << "\t" << suffix;
    }
}

/*------------------------------ FileComponent -------------------------------*/

Tree::FileComponent::FileComponent(){}

Tree::FileComponent::~FileComponent(){}

/*-------------------------------- LeafFile ----------------------------------*/

Tree::LeafFile::LeafFile(const std::string& fileName):m_fileName(fileName){}

Tree::LeafFile::~LeafFile(){}

void Tree::LeafFile::Print(std::size_t offset) const
{
	Tree::PrintSpaces(offset, "");
	
    std::cout << "├── ";

    std::cout << m_fileName << std::endl;
}

/*---------------------------- CompositeFolder -------------------------------*/

Tree::CompositeFolder::CompositeFolder(const std::string& dirPath):
															m_dirName(dirPath){}
Tree::CompositeFolder::~CompositeFolder() 
{
    std::list<const Tree::FileComponent *>::iterator i_iter;

    for (i_iter = m_files.begin(); i_iter != m_files.end(); ++i_iter)
    {
        delete *i_iter;
    }
}

void Tree::CompositeFolder::Add(const Tree::FileComponent *fileName)
{
    m_files.push_back(fileName);
}

void Tree::CompositeFolder::Print(size_t offset) const
{
	Tree::PrintSpaces(offset, "├── ");

    std::cout << m_dirName << std::endl;

    for_each(m_files.begin(), m_files.end(),
           std::bind2nd(std::mem_fun(&Tree::FileComponent::Print), offset + 1));
}

} // ilrd namespace


