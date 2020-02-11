#include "ObjData.h"

#define LINE_MAX 150

ObjData::ObjData()
	:m_numVertex{ 0 }, m_numNormals{ 0 }, m_numUVs{ 0 }, m_numTris{ 0 },
	m_vertexData{ nullptr }, m_normalData{ nullptr }, m_UVData{ nullptr },
	m_vertexID{ 0 }, m_normalID{ 0 }, m_UVID{ 0 }
{}


ObjData::~ObjData()
{
	clear();
}

void ObjData::clear()
{
	if (m_vertexData != nullptr)
	{
		delete[]m_vertexData;
	}

	delete[]m_normalData;
	delete[]m_UVData;
	delete[]m_vertexID;
	delete[]m_normalID;
	delete[]m_UVID;

	m_vertexData = nullptr;
	m_normalData = nullptr;
	m_UVData = nullptr;
	m_vertexID = nullptr;
	m_normalID = nullptr;
	m_UVID = nullptr;
}

bool ObjData::importData(const char * filename) // Comprobar que sea archivo valido, llenar la informacion y regresar true
{
	clear();
	bool bDone = true;
	ifstream ist{ filename };
	if (!ist)cout << "File not found." << filename << endl; bDone = false;

	// Primero cuento el total de elementos para saber cuanta memoria voy a reservar

	char * ptrString = new char[LINE_MAX];
	char * ptrToken = nullptr;
	char * ptrToken2 = nullptr;
	char * ptrToken3 = nullptr;
	char *ptrNextToken = nullptr;
	char *ptrNextToken2 = nullptr;

	ist.getline(ptrString, sizeof(char) * (LINE_MAX-1));

	while (!ist.eof()) //Mientras ist no regrese NULL, es decir, no llegue al final del archivo.
	{
		if (*ptrString == 'v' || *ptrString == 'f')
		{
			if (*ptrString == 'v') // Si la primer letra del token es v reviso la demás información
			{
				if (*(ptrString + 1) == 'n') //Si el siguiente caracter es n entonces la linea corresponde a un vertice de las normales
				{
					m_numNormals++;
				}
				else if (*(ptrString + 1) == 't') // Si el siguiente caracter es t entonces la linea corresponde a una coordenada de UVs
				{
					m_numUVs++;
				}
				else if (*(ptrString + 1) == ' ') // Si contine un caracter nulo significa que es un vertice.
				{
					m_numVertex++;
				}
			}
			else if (*ptrString == 'f'&&*(ptrString + 1) == ' ') //Si la primer letra del token es f y esta seguida del caracter nulo significa que es una linea con los IDs de la cara
			{
				m_numTris++;
			}
		}
		
		ist.getline(ptrString, sizeof(char) * (LINE_MAX - 1));
	}

	//Una vez contados los elementos reservo la memoria necesaria para guardar la información
	m_vertexData = new float[m_numVertex * 3];
	m_normalData = new float[m_numNormals * 3];
	m_UVData = new float[m_numUVs * 3];
	m_vertexID = new unsigned short[m_numTris * 3];
	m_normalID = new unsigned short[m_numTris * 3];
	m_UVID = new unsigned short[m_numTris * 3];

	//Regreso al principio del archivo
	ist.clear();
	ist.seekg(0, ios::beg);

	float * tempVertexData = m_vertexData;
	float * tempNormalData = m_normalData;
	float * tempUVData = m_UVData;

	unsigned short * tempVertexID = m_vertexID;
	unsigned short * tempNormalID = m_normalID;
	unsigned short * tempUVID = m_UVID;

	ist.getline(ptrString, sizeof(char) * (LINE_MAX - 1));
	while (!ist.eof())
	{
		if (*ptrString == 'v' || *ptrString == 'f')
		{
			ptrToken = strtok_s(ptrString, " ", &ptrNextToken); //Separo el archivo usando los espacios en blanco como token
			if (*ptrToken == 'v') // Si la primer letra del token es v reviso la demás información
			{
				if (*(ptrToken + 1) == '\0') //Si el siguiente caracter es n entonces la linea corresponde a un vertice de las normales
				{
					ptrToken2 = strtok_s(ptrNextToken," ", &ptrNextToken2);
					while (ptrToken2 != NULL)
					{
						*tempVertexData = stof(ptrToken2);
						tempVertexData++;
						ptrToken2 = strtok_s(NULL, " ", &ptrNextToken2);
					}
				}
				else if (*(ptrToken + 1) == 'n') // Si el siguiente caracter es t entonces la linea corresponde a una coordenada de UVs
				{
					ptrToken2 = strtok_s(ptrNextToken, " ", &ptrNextToken2);
					while (ptrToken2 != NULL)
					{
						*tempNormalData = stof(ptrToken2);
						tempNormalData++;
						ptrToken2 = strtok_s(NULL, " ", &ptrNextToken2);
					}
				}
				else if (*(ptrToken + 1) == 't') // Si contine un caracter nulo significa que es un vertice.
				{
					ptrToken2 = strtok_s(ptrNextToken, " ", &ptrNextToken2);
					while (ptrToken2 != NULL)
					{
						*tempUVData = stof(ptrToken2);
						tempUVData++;
						ptrToken2 = strtok_s(NULL, " ", &ptrNextToken2);
					}
				}
			}
			else if (*ptrToken == 'f'&&*(ptrToken + 1) == '\0') //Si la primer letra del token es f y esta seguida del caracter nulo significa que es una linea con los IDs de la cara
			{
				ptrToken2 = strtok_s(ptrNextToken, " ", &ptrNextToken2);
				while (ptrToken2 != NULL)
				{
					ptrToken3 = strtok_s(ptrToken2, "/", &ptrNextToken);
					*tempVertexID = stoi(ptrToken3);
					tempVertexID++;

					ptrToken3 = strtok_s(NULL, "/", &ptrNextToken);
					*tempUVID = stoi(ptrToken3);
					tempUVID++;

					*tempNormalID = stoi(ptrNextToken);
					tempNormalID++;

					ptrToken2 = strtok_s(NULL, " ", &ptrNextToken2);
				}
			}
		}

		ist.getline(ptrString, sizeof(char) * (LINE_MAX - 1));
	}
	
	if (bDone) { cout << "Data loaded." << endl; }
	if (saveData(filename)) { cout << "Data saved in a .txt file" << endl; }
	ist.close();
	delete[] ptrString;
	ptrString = nullptr;
	return bDone;
}

ostream& operator<<(ostream& os, const ObjData& e)
{
	os << "Vertex [ " << e.m_numVertex << " ]" << endl;
	for (size_t i = 0; i < e.m_numVertex; i++)
	{
		os << "[" << i << "]\t" << e.m_vertexData[3 * i] << "\t" << e.m_vertexData[(3 * i) + 1] << "\t" << e.m_vertexData[(3 * i) + 2] << endl;
	}

	os << "\n";

	os << "Normals [ " << e.m_numNormals << " ]" << endl;
	for (size_t i = 0; i < e.m_numNormals; i++)
	{
		os << "[" << i << "]\t" << e.m_normalData[3 * i] << "\t" << e.m_normalData[(3 * i) + 1] << "\t" << e.m_normalData[(3 * i) + 2] << endl;
	}

	os << "\n";

	os << "UVs [ " << e.m_numUVs << " ]" << endl;
	for (size_t i = 0; i < e.m_numUVs; i++)
	{
		os << "[" << i << "]\t" << e.m_UVData[3 * i] << "\t" << e.m_UVData[(3 * i) + 1] << "\t" << e.m_UVData[(3 * i) + 2] << endl;
	}

	os << "\n";

	os << "Tris [ " << e.m_numTris << " ]" << endl;
	for (size_t i = 0; i < e.m_numTris; i++)
	{
		os << "[" << i << "]\t" << e.m_vertexID[3 * i] << "\t" << e.m_vertexID[(3 * i) + 1] << "\t" << e.m_vertexID[(3 * i) + 2] << endl;
		os << "[" << i << "]\t" << e.m_normalID[3 * i] << "\t" << e.m_normalID[(3 * i) + 1] << "\t" << e.m_normalID[(3 * i) + 2] << endl;
		os << "[" << i << "]\t" << e.m_UVID[3 * i] << "\t" << e.m_UVID[(3 * i) + 1] << "\t" << e.m_UVID[(3 * i) + 2] << endl;
		os << endl;
	}

	return os;
}

bool ObjData::saveData(const char * filename)
{
	bool bSaved = true;
	string outFileName = (string)filename + ".DATA.txt";
	ofstream ost{ outFileName };
	if (!ost)cout << "Couldn't create file." << endl; bSaved = false;

	ost << "Vertex [ " << m_numVertex << " ]" << endl;
	for (size_t i = 0; i < m_numVertex; i++)
	{
		ost << "[" << i << "]\t" << m_vertexData[3 * i] << "\t" << m_vertexData[(3 * i) + 1] << "\t" << m_vertexData[(3 * i) + 2] << endl;
	}

	ost << "\n";

	ost << "Normals [ " << m_numNormals << " ]" << endl;
	for (size_t i = 0; i < m_numNormals; i++)
	{
		ost << "[" << i << "]\t" << m_normalData[3 * i] << "\t" << m_normalData[(3 * i) + 1] << "\t" << m_normalData[(3 * i) + 2] << endl;
	}

	ost << "\n";

	ost << "UVs [ " << m_numUVs << " ]" << endl;
	for (size_t i = 0; i < m_numUVs; i++)
	{
		ost << "[" << i << "]\t" << m_UVData[3 * i] << "\t" << m_UVData[(3 * i) + 1] << "\t" << m_UVData[(3 * i) + 2] << endl;
	}

	ost << "\n";

	ost << "Tris [ " << m_numTris << " ]" << endl;
	for (size_t i = 0; i < m_numTris; i++)
	{
		ost << "[" << i << "]\t" << m_vertexID[3 * i] << "\t" << m_vertexID[(3 * i) + 1] << "\t" << m_vertexID[(3 * i) + 2] << endl;
		ost << "[" << i << "]\t" << m_normalID[3 * i] << "\t" << m_normalID[(3 * i) + 1] << "\t" << m_normalID[(3 * i) + 2] << endl;
		ost << "[" << i << "]\t" << m_UVID[3 * i] << "\t" << m_UVID[(3 * i) + 1] << "\t" << m_UVID[(3 * i) + 2] << endl;
		ost << endl;
	}

	ost.close();
	return bSaved;
}