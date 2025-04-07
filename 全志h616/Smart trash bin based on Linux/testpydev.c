#include "Python.h"
int main()
{
	Py_Initialize();
	PyRun_SimpleString("print('funny')");
	Py_Finalize();

}
