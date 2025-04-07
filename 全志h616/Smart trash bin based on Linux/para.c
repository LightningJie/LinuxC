#if 0 
1、包含Python.h头文件，以便使用Python API。 
2、使用void Py_Initialize()初始化Python解释器， 
3、使用PyObject *PyImport_ImportModule(const char *name)和PyObject *PyObject_GetAttrString(PyObject *o, const char *attr_name)获取sys.path对象，并利用int PyList_Append(PyObject *list, PyObject *item)将当前路径. 添加到sys.path中，以便加载当前的Python模块(当前有Python文件即python模块)。 
4、使用PyObject *PyImport_ImportModule(const char *name)函数导入Python模块，并检查是否有错误。 
5、使用PyObject *PyObject_GetAttrString(PyObject *o, const char *attr_name)函数获取Python函数对象，并检查是否可调用。 
6、使用PyObject *Py_BuildValue(const char *format, ...)函数创建一个Python元组，作为Python函数的参数,没有参数时不需要调用 
7、使用PyObject *PyObject_CallObject(PyObject *callable, PyObject *args)函数调用Python函数，并获取返回值。 
8、使用int PyArg_Parse(PyObject *args, const char *format, ...)函数将返回值转换为C类型，并检查是否有错误,没有返回值时不需要调用。 
9、使用void Py_DECREF(PyObject *o)函数释放所有引用的Python对象。 
10、结束时调用void Py_Finalize()函数关闭Python解释器。相关的函数说明参考网站（网站左上角输入函数名即可开始搜索）： https://docs.python.org/zh-cn/3/c-api/import.html 
#endif

#include "Python.h"
int main()
{
	Py_Initialize();
	PyObject *sys = PyImport_ImportModule("sys");
	PyObject *path = PyObject_GetAttrString(sys,"path");
	PyList_Append(path,PyUnicode_FromString("."));
	PyObject *pmodel=PyImport_ImportModule("para");
	if(!pmodel)
	{
		PyErr_Print();
		printf("Error : failed to load nopara\n");
	}
	PyObject *pFunc=PyObject_GetAttrString(pmodel, "say_funny");
	if(!pFunc)
	{
		PyErr_Print();
		printf("Error : failed to load say_funny\n");
	}
	char *category="comedy";
	PyObject *pArgs = Py_BuildValue("(s)",category);
	PyObject *pValue=PyObject_CallObject(pFunc,pArgs);
	if(!pValue)
	{
		PyErr_Print();
		printf("Error : failed to call failed\n");
	}
	char *result=NULL;
	if(!PyArg_Parse(pValue,"s",&result))
	{
		PyErr_Print();
		printf("Error : parse failed \n");
	}
	printf("result : %s\n",result);
	Py_DECREF(pValue);
	Py_DECREF(pFunc);
	Py_DECREF(pmodel);
	Py_DECREF(path);
	Py_DECREF(sys);

	Py_Finalize();
}
