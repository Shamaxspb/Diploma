
#pragma once
//#include "ListOfCPoint3D.h"
//#include "PolygonPoints.h"
#include "Frank_lib.h"
#include <cstdio>
#include <string>
class CStlFILE
{
public:
	CStlFILE() {}
	CStlFILE(const std::string& fName);
	virtual ~CStlFILE();
	CStlFILE* Copy();
        PolygonPoints GetMesh()
         {
                return  PolygonMesh;
                }
	PolygonPoints PolygonMesh;

private:
	bool Read();
	std::string fileName;
};

bool ifvertex (char vertex[40])
{
        char vertex_mask[7]="vertex";

        for (int i=0;i<6;i++)
                if (vertex[i]!=vertex_mask[i])
                        return false;
return true;
}


CStlFILE::CStlFILE(const std::string& fName)
{
	fileName=fName;
	//pointList = new CListOfCPoint3D;
	Read();
}

CStlFILE::~CStlFILE()
{
	//pointList->Clear();
//	delete pointList;
}

CStlFILE* CStlFILE::Copy()
{
	CStlFILE* R = new CStlFILE(fileName);
	return R;
}



bool CStlFILE::Read()
{
	FILE* stlfp;
	int i = 0, j = 0, cnt = 0, pCnt = 0;
	char a[100];
	char str[40];
		char vertex[40];
	double x = 0, y = 0, z = 0;
	CPoint3D P[3];
	   //	char* pStr = CT2A((LPCTSTR)fileName);

	std::fopen_s(&stlfp,fileName.c_str(), "r");
	if (!stlfp)
		return false;
	else
	{
		do
		{
			i = 0;
			cnt = 0;
			fgets(a, 100, stlfp);
                        while (a[i] != ' '&& a[i]!= '\0')
                        {
                          vertex[i]=a[i];
                        i++;
                        }
                        vertex[i] = '\0';
                        i=0;
			while (a[i] != '\0')
			{
				if (!islower((int)a[i]) && !isupper((int)a[i]) && a[i] != ' ')
					break;
				cnt++;
				i++;
			}
			while (a[cnt] != '\0')
			{
				str[j] = a[cnt];
				cnt++;
				j++;
			}
			str[j] = '\0';
			j = 0;

			if (ifvertex(vertex)&&sscanf(str, "%lf%lf%lf", &x, &y, &z) == 3)
			{
				P[pCnt++].Set(x,y,z);

			}
			if (pCnt==3)
			{
				       PolygonMesh.AddPolygon(P[0],P[1],P[2]);
					pCnt=0;
			}

		} while (!feof(stlfp));
		fclose(stlfp);
		return true;
	}
}

class CObjFILE
{
public:
	CObjFILE() {}
	CObjFILE(const std::string& fName);
	virtual ~CObjFILE();
	CObjFILE* Copy();
           PolygonPoints GetMesh()
         {
                return  PolygonMesh;
                }
	PolygonPoints PolygonMesh;

private:
	bool Read();
	std::string fileName;
};


CObjFILE::CObjFILE(const std::string& fName)
{
	fileName=fName;
	//pointList = new CListOfCPoint3D;
	Read();
}

CObjFILE::~CObjFILE()
{
	//pointList->Clear();
//	delete pointList;
}

CObjFILE* CObjFILE::Copy()
{
	CObjFILE* R = new CObjFILE(fileName);
	return R;
}

bool CObjFILE::Read()
{
	std::ifstream stream(fileName.c_str());
        double x = 0, y = 0, z = 0;
	CPoint3D P;


	if (stream.is_open())
	{
		std::string line;
		while (std::getline(stream, line))
		{
			if (   line.length() >= 1 && (line[0]=='o' || line[0]=='s' || line[0]=='#')
				|| line.length() >= 6 && (line.substr(0, 6) == "mtllib" || line.substr(0, 6) == "usemtl")
				|| line.length() >= 2 && (line.substr(0, 2) == "vn") )
				continue;

			if (line.length() >= 1 && line[0] == 'v')
			{
				std::istringstream str(line);
				char c;
				double x, y, z;
				str >> c >> x >> y >> z;
				P.Set(x,y,-z);
                                PolygonMesh.AddPoint( P);
			}

			if (line.length() >= 1 && line[0] == 'f')
			{
				std::istringstream str(line);
				std::string word,temp;
				str >> word;
                                //AnsiString S=word.;

				str >> word;  temp=word.substr(0, word.find('/')) ;
                                int i0 = atoi(temp.c_str());
				str >> word;   temp=word.substr(0, word.find('/')) ;
                                int i1 = atoi(temp.c_str());
				str >> word;  temp=word.substr(0, word.find('/')) ;
                                int i2 = atoi(temp.c_str());
				/*Fs.push_back(i0 - 1);
				Fs.push_back(i1 - 1);
				Fs.push_back(i2 - 1);  */
                                PolygonMesh.AddPoygonIndexes(i0 - 1, i1 - 1, i2 - 1);
			}
		}
		stream.close();
	}
}

