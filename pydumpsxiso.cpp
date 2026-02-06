#define VERSION "2.20"
#include "mkpsxiso/src/dumpsxiso/main.cpp"
#define MALLOC_CHECK(var) if (var == NULL) { PyErr_NoMemory(); return NULL; }

extern "C"
{
    #include <Python.h>

    static PyObject *method_run(PyObject *self, PyObject *args, PyObject *kwargs)
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
        int result = Main(argc, argv);
        PyMem_Free(argv);
        return PyBool_FromLong((long)(result == 0));
    }

    static PyMethodDef PydumpsxisoMethods[] = {
        {"run", (PyCFunction)method_run, METH_VARARGS | METH_KEYWORDS, "Call dumpsxiso command."},
        {NULL, NULL, 0, NULL}
    };

    static struct PyModuleDef pydumpsxisomodule = {
        PyModuleDef_HEAD_INIT,
        "pydumpsxiso",
        "Python interface for dumpsxiso.",
        -1,
        PydumpsxisoMethods
    };

    PyMODINIT_FUNC PyInit_pydumpsxiso(void)
    {
        return PyModule_Create(&pydumpsxisomodule);
    }
}
