/**************************************
*				   					  *
*	Developer: Eitan Bellaiche        *	 
*	Reviewer: Matan          	      *
*	Date: 30/04/23	                  *
*	Subject: Tree			   		  *
*				                	  *
**************************************/

#ifndef ILRD_RD136_7_TREE_HPP
#define ILRD_RD136_7_TREE_HPP

#include <string>	// string
#include <list>     // list

namespace ilrd
{
class Tree												//class Tree	
{
public:
	Tree(const std::string& dirName);

	void Print() const;

private:
	class FileComponent									//class FileComponent
	{
	public:
		FileComponent();
		virtual ~FileComponent();

		virtual void Print(size_t offset = 0) const =0;
//		virtual FileComponent *Clone() const;

	protected:
		FileComponent(const FileComponent& src);
	
	private:
		FileComponent& operator=(const FileComponent& other);
	};

	class LeafFile: public FileComponent				//class LeafFile
	{
	public:
		LeafFile(const std::string& fileName);
		~LeafFile();
		
		void Print(std::size_t offset = 0) const;
		
	private:
	    std::string m_fileName;
	};

	class CompositeFolder: public FileComponent			//class CompositeFolder
	{
	public:
		CompositeFolder(const std::string &dirPath);
		~CompositeFolder();

		void Add(const FileComponent *fileName);
		void Print(size_t offset = 0) const;

	private:
	    std::string m_dirName;
	    std::list<const Tree::FileComponent *> m_files;
	};
	
    CompositeFolder m_root;
    static FileComponent *RecAddFS(const std::string& Path, 
													   const std::string& Curr);
	const std::string& IsDirValid(const std::string& dirName);			
	static void PrintSpaces(size_t offset, const std::string& suffix);									   
};

} // ilrd

#endif	// ILRD_RD136_7_TREE_HPP

