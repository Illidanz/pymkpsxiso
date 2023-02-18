#define VERSION "2.02"
// This is kind of dirty, but the main.cpp file for both programs includes a lot of code
// They both use a Main function so we modify one from the setup.py script
#include "mkpsxiso/src/dumpsxiso/main.cpp"
#include "mkpsxiso/src/mkpsxiso/main.cpp"

#define MALLOC_CHECK(var) if (var == NULL) { PyErr_NoMemory(); return NULL; }

extern "C"
{
    #include <Python.h>

    static PyObject *method_dump(PyObject *self, PyObject *args, PyObject *kwargs)
    {
        char *filename = NULL;
        char *path = NULL;
        char *xml = NULL;

        static char *kwlist[] = {"filename", "path", "xml",
                                 NULL};

        if(!PyArg_ParseTupleAndKeywords(args, kwargs, "sss", kwlist,
                                        &filename, &path, &xml))
            return NULL;

        int argc = 6;
        char** argv = (char**)PyMem_Malloc(sizeof(char*) * (argc + 1));
        MALLOC_CHECK(argv);
        argv[0] = "dumpsxiso";
        argv[1] = "-x";
        argv[2] = path;
        argv[3] = "-s";
        argv[4] = xml;
        argv[5] = filename;
        argv[6] = NULL;
        int result = dumpsxiso_main(argc, argv);
        PyMem_Free(argv);
        return PyBool_FromLong((long)(result == 0));
    }

    static PyObject *method_make(PyObject *self, PyObject *args, PyObject *kwargs)
    {
        char *filename = NULL;
        char *xml = NULL;

        static char *kwlist[] = {"filename", "xml",
                                 NULL};

        if(!PyArg_ParseTupleAndKeywords(args, kwargs, "ss", kwlist,
                                        &filename, &xml))
            return NULL;

        int argc = 5;
        char** argv = (char**)PyMem_Malloc(sizeof(char*) * (argc + 1));
        MALLOC_CHECK(argv);
        argv[0] = "mkpsxiso";
        argv[1] = "-y";
        argv[2] = "-o";
        argv[3] = filename;
        argv[4] = xml;
        argv[5] = NULL;
        int result = Main(argc, argv);
        PyMem_Free(argv);
        return PyBool_FromLong((long)(result == 0));
    }

    static PyMethodDef PymkpsxisoMethods[] = {
        {"dump", (PyCFunction)method_dump, METH_VARARGS | METH_KEYWORDS, "Call dumpsxiso command."},
        {"make", (PyCFunction)method_make, METH_VARARGS | METH_KEYWORDS, "Call mkpsxiso command."},
        {NULL, NULL, 0, NULL}
    };

    static struct PyModuleDef pymkpsxisomodule = {
        PyModuleDef_HEAD_INIT,
        "pymkpsxiso",
        "Python interface for mkpsxiso.",
        -1,
        PymkpsxisoMethods
    };

    PyMODINIT_FUNC PyInit_pymkpsxiso(void)
    {
        return PyModule_Create(&pymkpsxisomodule);
    }
}
