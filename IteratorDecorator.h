//---------------------------------------------------------------------------

#ifndef IteratorDecoratorH
#define IteratorDecoratorH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
//---------------------------------------------------------------------------
class Iterator
{
	public:
		virtual void First(HANDLE fileHandle, ULONGLONG startOffset, int sizeCurrentCluster, int maxCurrentCluster) = 0;
		virtual void Next(HANDLE fileHandle, ULONGLONG startOffset, int sizeCurrentCluster, int maxCurrentCluster) = 0;
		virtual bool IsDone(int clusterFrom, int clusterTo, int maxCurrentCluster) = 0;
		virtual AnsiString GetCurrent(HANDLE fileHandle, int sizeCurrentCluster) = 0;
};

class ClusterIterator : public Iterator
{
	public:
		virtual void First(HANDLE fileHandle, ULONGLONG startOffset, int sizeCurrentCluster, int maxCurrentCluster);
		virtual void Next(HANDLE fileHandle, ULONGLONG startOffset, int sizeCurrentCluster, int maxCurrentCluster);
		virtual bool IsDone(int clusterFrom, int clusterTo, int maxCurrentCluster);
		virtual AnsiString GetCurrent(HANDLE fileHandle, int sizeCurrentCluster);
};

class TxtClusterIteratorDecorator : public Iterator
{
	public:
		virtual void First(HANDLE fileHandle, ULONGLONG startOffset, int sizeCurrentCluster, int maxCurrentCluster);
		virtual void Next(HANDLE fileHandle, ULONGLONG startOffset, int sizeCurrentCluster, int maxCurrentCluster);
		virtual bool IsDone(int clusterFrom, int clusterTo, int maxCurrentCluster);
		virtual AnsiString GetCurrent(HANDLE fileHandle, int sizeCurrentCluster);
};

#endif
