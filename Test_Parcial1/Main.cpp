#include "ObjData.h"

int main()
{
	ObjData test;

	////Box
	//test.importData("TestBox.obj");
	//cout << test;
	//system("Pause");
	//system("cls");

	//Objects
	//test.importData("TestObjects.obj"); // No funciona con este archivo, desconozco la razon.
	//cout << test;
	//system("Pause");
	//system("cls");

	////Soldier
	test.importData("TF2_REF_SOLDIER.obj");
	cout << test;
	system("Pause");
	system("cls");

	/*test.importData("TestObjects.obj");
	test.importData("TF2_REF_SOLDIER.obj");*/

	system("pause");
	return 0;
}