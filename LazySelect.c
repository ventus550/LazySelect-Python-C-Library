#include <Python.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Py_ssize_t
Random(Py_ssize_t low, Py_ssize_t high) {
	return (rand() % (high - low + 1)) + low;
}

int
compare(const void* a, const void* b) {
    long arg1 = *(const long*)a;
    long arg2 = *(const long*)b;
  
    return (arg1 > arg2) - (arg1 < arg2);
}
 

long
CLazySelect(long array[], Py_ssize_t n, long k) {

	// Choose a sample
	int r = (int)pow(n, 0.75); long R[r];
	
	for (Py_ssize_t i = 0; i < r; i++) {
		R[i] = array[Random(0,n-1)];
	}

	// Sort the given sample
	qsort(R, r, sizeof(long), compare);

	//Scan the sample
	double x = 0.5 * pow(n, 0.75),
		   root = pow(n, 0.5);
	Py_ssize_t l = fmax((int)(x - root), 0),
			   h = fmin(ceil(x + root), r-1);
	
	long L = R[l], H = R[h];
	Py_ssize_t filled = 0, Lf = 0;
	for (Py_ssize_t i = 0; i < n; i++) {
		if (L <= array[i] && array[i] <= H) { filled++; }
	}

	long P[filled]; filled = 0;
	for (Py_ssize_t i = 0; i < n; i++) {
		if (L <= array[i] && array[i] <= H) {
			P[filled] = array[i];
			filled++;
		} else {
			if (array[i] < L) { Lf++; }
		}
	}

	// Check if the k-th element is in P
	if ( Lf < k && k <= Lf + filled && filled <= 4*r + 2 ) {
		qsort(P, filled, sizeof(long), compare);
		return P[k - Lf - 1];
	}

	return CLazySelect(array, n, k);
}

static PyObject*
Select(PyObject* self, PyObject* args) {
	srand(time(0));
	
	PyObject *pList; long k;
	if (!PyArg_ParseTuple(args, "O!l", &PyList_Type, &pList, &k)) {
		PyErr_SetString(PyExc_TypeError, "arguments must be a python list followed by an integer");
		return NULL;
	}
	k += 1;

	Py_ssize_t n = PyList_Size(pList);	
	if (k < 1 || k > n) {
		PyErr_SetString(PyExc_ValueError, "given position is out of range");
		return NULL;
	}


	long array[n];
	PyObject *pItem;
	for (int i = 0; i < n; i++) {
		pItem = PyList_GetItem(pList, i);
		if(!PyLong_Check(pItem)) {
			PyErr_SetString(PyExc_TypeError, "list must contain integers only");
			return NULL;
		}
		array[i] = PyLong_AsLong(pItem);
	}
	return Py_BuildValue("i", CLazySelect(array, n, k));

}

static PyMethodDef
myMethods[] = {
	{"select", Select, METH_VARARGS, "Retrieves the k-th smallest integer in the list"},
	{NULL, NULL, 0, NULL}
};

static struct PyModuleDef
LazySelect = {
	PyModuleDef_HEAD_INIT,
	"LazySelect",
	"LazySelect Module",
	-1,
	myMethods
};

PyMODINIT_FUNC
PyInit_LazySelect(void) {
	return PyModule_Create(&LazySelect);
}