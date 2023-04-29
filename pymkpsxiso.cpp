#define VERSION "2.03"
// This is kind of dirty, but the main.cpp file for both programs includes a lot of code
// They both use a Main function so we need to change the name
#define Main dumpsxiso_main
#include "mkpsxiso/src/dumpsxiso/main.cpp"
#define Main mkpsxiso_main
#include "mkpsxiso/src/mkpsxiso/main.cpp"
#undef Main

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
        char *cuename = NULL;
        char *xml = NULL;

        static char *kwlist[] = {"filename", "cuename", "xml",
                                 NULL};

        if(!PyArg_ParseTupleAndKeywords(args, kwargs, "sss", kwlist,
                                        &filename, &cuename, &xml))
            return NULL;

        int argc = 7;
        char** argv = (char**)PyMem_Malloc(sizeof(char*) * (argc + 1));
        MALLOC_CHECK(argv);
        argv[0] = "mkpsxiso";
        argv[1] = "-y";
        argv[2] = "-c";
        argv[3] = cuename;
        argv[4] = "-o";
        argv[5] = filename;
        argv[6] = xml;
        argv[7] = NULL;
        int result = mkpsxiso_main(argc, argv);
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

// Add an empty main for mkpsxiso/src/shared/common
int Main(int argc, char* argv[])
{
}
