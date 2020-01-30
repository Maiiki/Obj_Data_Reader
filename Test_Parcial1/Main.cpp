#include "ObjData.h"

int main()
{
	ObjData test;
	test.importData("TestBox.obj");
	cout << test;
	/*test.importData("TestObjects.obj");
	test.importData("TF2_REF_SOLDIER.obj");*/

	system("pause");
	return 0;
}