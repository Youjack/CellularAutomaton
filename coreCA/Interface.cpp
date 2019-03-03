#include <Python.h>
#include "coreAutomaton.h"
using namespace std;

PyObject* Controller(PyObject *self, PyObject* pInputList)
{
	// 0: d
	// 1: cMapExt, 2: MapExt, 3: lMapExt
	// 4: Map
	// 5: RuleExt, 6: nStates, 7: nRule
	// 8: cRuleMap, 9: RuleOut
	// 10: nStep, 11: WithAMP

	const int d = PyLong_AsLong(PyList_GetItem(pInputList, 0));

	// Prepare Map ------------------------

	int *cMapExt = new int[d];
	PyObject* pcMapExt = PyList_GetItem(pInputList, 1);
	for (int i = 0; i < d; i++)
		cMapExt[i] = PyLong_AsLong(PyList_GetItem(pcMapExt, i));

	const int MapExt = PyLong_AsLong(PyList_GetItem(pInputList, 2));

	int *lMapExt = new int[d];
	PyObject* plMapExt = PyList_GetItem(pInputList, 3);
	for (int i = 0; i < d; i++)
		lMapExt[i] = PyLong_AsLong(PyList_GetItem(plMapExt, i));

	int *Map = new int[MapExt];
	PyObject* pMap = PyList_GetItem(pInputList, 4);
	for (int i = 0; i < MapExt; i++)
		Map[i] = PyLong_AsLong(PyList_GetItem(pMap, i));

	// Prepare Rule ------------------------

	const int RuleExt = PyLong_AsLong(PyList_GetItem(pInputList, 5));
	const int nStates = PyLong_AsLong(PyList_GetItem(pInputList, 6));
	const int nRule = PyLong_AsLong(PyList_GetItem(pInputList, 7));

	int *cRuleMap = new int[RuleExt*d];
	PyObject* pcRuleMap = PyList_GetItem(pInputList, 8);
	for (int i = 0; i < RuleExt; i++)
	{
		PyObject* pRuleCoo = PyList_GetItem(pcRuleMap, i);
		for (int j = 0; j < d; j++)
			cRuleMap[d*i+j] = PyLong_AsLong(PyList_GetItem(pRuleCoo, j));
	}

	int *RuleOut = new int[nRule];
	PyObject* pRuleOut = PyList_GetItem(pInputList, 9);
	for (int i = 0; i < nRule; i++)
		RuleOut[i] = PyLong_AsLong(PyList_GetItem(pRuleOut, i));
	
	// Generate ------------------------
	const int nStep = (int)PyLong_AsLong(PyList_GetItem(pInputList, 10));
	const int withAMP = (int)PyLong_AsLong(PyList_GetItem(pInputList, 11));
	unsigned long t;
	if (withAMP == 1)
		t = GenerateWithAMP(
			d,
			cMapExt, MapExt, lMapExt,
			Map,
			RuleExt, nStates, nRule,
			cRuleMap, RuleOut,
			nStep);
	else
		t = Generate(
			d,
			cMapExt, MapExt, lMapExt,
			Map,
			RuleExt, nStates,
			cRuleMap, RuleOut,
			nStep);

	// Return and Release Memory ------------------------
	PyObject *pMapOut = PyList_New(MapExt + 1);
	// the last term in pMapOut is TIME spent
	for (int i = 0; i < MapExt; i++)
		PyList_SetItem(pMapOut, i, PyLong_FromLong(Map[i]));
	PyList_SetItem(pMapOut, MapExt, PyLong_FromUnsignedLong(t));
	delete[] cMapExt;
	delete[] lMapExt;
	delete[] Map;
	delete[] cRuleMap;
	delete[] RuleOut;
	return pMapOut;
}

static PyMethodDef coreMethod[] = 
{
	{ "coreAutomaton", (PyCFunction)Controller, METH_O, nullptr },
    { nullptr, nullptr, 0, nullptr }
};

static PyModuleDef coreModule =
{
	PyModuleDef_HEAD_INIT,
	"coreCA",
	"Cellular Automaton core algorithm",
	0,
	coreMethod
};

PyMODINIT_FUNC PyInit_coreCA() 
{
	return PyModule_Create(&coreModule);
}