#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class ObjData
{
private:
	int m_numVertex; //4
	int m_numNormals; //1
	int m_numUVs; //4
	int m_numTris; //2

	float * m_vertexData; // numVertex x 3 | Informacion de vectores
	float * m_normalData; // numNormals x 3
	float * m_UVData; // numUVs x 2

	unsigned short * m_vertexID; //numTris x 3 | 
	unsigned short * m_normalID; //numTris x 3 |  
	unsigned short * m_UVID; //numTris x 3     |

public:
	ObjData(); // Inicializa todo en 0 o nullptr
	bool importData(const char * filename); // Comprobar que sea archivo valido, llenar la informacion y regresar true
	friend ostream& operator<<(ostream& os, const ObjData& e);
	~ObjData();

private:
	void clear();
	bool saveData(const char * filename);
};

